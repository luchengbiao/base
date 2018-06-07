#include "base/base_types.h"
#include "base/macros.h"
#include "base/memory/lazy_instance.h"
#include "framework_qthread.h"
#include "base/thread/thread_local.h"
//#include "base/log/log.h"

namespace nbase
{

	// We use this thread-local variable to record whether or not a thread exited
	// because its Stop method was called.  This allows us to catch cases where
	// MessageLoop::Quit() is called directly, which is unexpected when using a
	// Thread to setup and run a MessageLoop.
	nbase::LazyInstance<nbase::ThreadLocalPointer<FrameworkQThreadTlsData> > lazy_tls_data;

	FrameworkQThread::FrameworkQThread(const char* name)
		: started_(false),
		stopping_(false),
		loop_type_(MessageLoop::kDefaultMessageLoop),
		message_loop_(NULL),
		event_(false, false),
		name_(name)
	{

	}

	FrameworkQThread::~FrameworkQThread()
	{
		Stop();
	}

	bool FrameworkQThread::Start()
	{
		return StartWithLoop(MessageLoop::kDefaultMessageLoop);
	}

#if defined(OS_WIN)
	bool FrameworkQThread::StartWithLoop(const MessageLoop::Type type, Dispatcher *dispatcher)
#else
	bool FrameworkQThread::StartWithLoop(const MessageLoop::Type type)
#endif
	{
		if (message_loop_ != NULL)
			return false;

		loop_type_ = type;
#if defined(OS_WIN)
		dispatcher_ = dispatcher;
#endif

		if (Create() == false)
			return false;

		// Wait for the thread to start and initialize message_loop_
		event_.Wait();

		started_ = true;
		return true;
	}

#if defined(OS_WIN)
	bool FrameworkQThread::StartWithLoop(CustomMessageLoopFactory *factory, Dispatcher *dispatcher)
#else
	bool FrameworkQThread::StartWithLoop(CustomMessageLoopFactory *factory)
#endif
	{
		if (factory == NULL)
			return false;

		factory_.reset(factory);
		loop_type_ = MessageLoop::kCustomMessageLoop;
#if defined(OS_WIN)
		dispatcher_ = dispatcher;
#endif

		if (!Create())
			return false;

		// Wait for the thread to start and initialize message_loop_
		event_.Wait();

		started_ = true;

		return true;
	}

#if defined(OS_WIN)
	void FrameworkQThread::RunOnCurrentThreadWithLoop(const MessageLoop::Type type, Dispatcher *dispatcher)
#else
	void FrameworkQThread::RunOnCurrentThreadWithLoop(const MessageLoop::Type type)
#endif
	{
		loop_type_ = type;
		started_ = true;
#if defined(OS_WIN)
		dispatcher_ = dispatcher;
#endif
		Run();
	}

	void FrameworkQThread::Stop()
	{
		if (!thread_was_started())
			return;

		StopSoon();

		// Wait for the thread to exit.
		Close();

		started_ = false;
		stopping_ = false;
	}

	void FrameworkQThread::StopSoon()
	{
		if (stopping_ || !message_loop_)
			return;

		stopping_ = true;
		message_loop_->PostNonNestableTask(
			nbase::Bind(&FrameworkQThread::DoStopSoon, this));
	}

	void FrameworkQThread::DoStopSoon()
	{
		MessageLoop::current()->Quit();
		SetThreadWasQuitProperly(true);
	}

	void FrameworkQThread::Run()
	{
#ifndef NDEBUG
#if defined(OS_WIN) && defined(COMPILER_MSVC)
		SetQThreadName(GetCurrentThreadId(), name_.c_str());
#endif
#endif

		InitTlsData(this);
		SetThreadWasQuitProperly(false);
		{
			ThreadId thread_id = Thread::CurrentId();
			set_thread_id(thread_id);

			// The message loop for this thread.
			MessageLoop *message_loop;
			if (loop_type_ == MessageLoop::kCustomMessageLoop)
				message_loop = factory_->CreateMessageLoop();
			else {
				if (loop_type_ == MessageLoop::kIOMessageLoop)
					message_loop = new IOMessageLoop;
#if defined(OS_WIN)
				else if (loop_type_ == MessageLoop::kUIMessageLoop)
					message_loop = new UIMessageLoop;
#endif
				else
					message_loop = new MessageLoop;
			}
			message_loop_ = message_loop;

			// Let the thread do extra initialization.
			// Let's do this before signaling we are started.
			Init();

			event_.Signal();

#if defined(OS_WIN)
			message_loop_->RunWithDispatcher(dispatcher_);
#else
			message_loop_->Run();
#endif // OS_WIN

			// Let the thread do extra cleanup.
			Cleanup();

			//DCHECK(GetThreadWasQuitProperly());

			// We can't receive messages anymore.
			if (loop_type_ != MessageLoop::kCustomMessageLoop)
				delete message_loop_;
			else
			{
				delete message_loop_;
				factory_.reset();
			}
			message_loop_ = NULL;
		}
		set_thread_id(kInvalidQThreadId);
		{
			FrameworkQThreadTlsData *tls = GetTlsData();
			if (tls != NULL)
			{
			}
			//DCHECK(tls->managed == 0); // you must call ThreadManager::UnregisterThread before come there
		}
		FreeTlsData();
	}

	void FrameworkQThread::InitTlsData(FrameworkQThread *self)
	{
		FrameworkQThreadTlsData *tls = GetTlsData();
		//DCHECK(tls == NULL);
		if (tls != NULL)
			return;
		tls = new FrameworkQThreadTlsData;
		tls->self = self;
		tls->managed = 0;
		tls->managed_thread_id = -1;
		tls->quit_properly = false;
		tls->custom_data = NULL;
		lazy_tls_data.Pointer()->Set(tls);
	}

	void FrameworkQThread::FreeTlsData()
	{
		FrameworkQThreadTlsData *tls = GetTlsData();
		//DCHECK(tls != NULL);
		if (tls == NULL)
			return;
		lazy_tls_data.Pointer()->Set(NULL);
		delete tls;
	}

	FrameworkQThreadTlsData* FrameworkQThread::GetTlsData()
	{
		return lazy_tls_data.Pointer()->Get();
	}

	bool FrameworkQThread::GetThreadWasQuitProperly()
	{
		FrameworkQThreadTlsData *tls = GetTlsData();
		//DCHECK(tls != NULL);
		if (tls == NULL)
			return false;
		return tls->quit_properly;
	}

	void FrameworkQThread::SetThreadWasQuitProperly(bool flag)
	{
		FrameworkQThreadTlsData *tls = GetTlsData();
		//DCHECK(tls != NULL);
		if (tls == NULL)
			return;
		tls->quit_properly = flag;
	}

	FrameworkQThread* FrameworkQThread::current()
	{
		FrameworkQThreadTlsData *tls = GetTlsData();
		//DCHECK(tls != NULL);
		if (tls == NULL)
			return NULL;
		return tls->self;
	}

	int FrameworkQThread::GetManagedThreadId()
	{
		FrameworkQThreadTlsData *tls = GetTlsData();
		//DCHECK(tls != NULL);
		if (tls == NULL)
			return -1;
		return tls->managed_thread_id;
	}

	void* FrameworkQThread::GetCustomTlsData()
	{
		FrameworkQThreadTlsData *tls = GetTlsData();
		//DCHECK(tls != NULL);
		if (tls == NULL)
			return NULL;
		return tls->custom_data;
	}

	void FrameworkQThread::SetCustomTlsData(void *data)
	{
		FrameworkQThreadTlsData *tls = GetTlsData();
		//DCHECK(tls != NULL);
		if (tls == NULL)
			return;
		tls->custom_data = data;
	}

}  // namespace nbase