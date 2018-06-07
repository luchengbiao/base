#include <assert.h>
#include "qbasethread.h"

namespace nbase
{

	// Set a Thread Name in Native Code
	// http://msdn.microsoft.com/en-us/library/xcb2z8hs.aspx
#if defined(OS_WIN) && defined(COMPILER_MSVC)
	const DWORD MS_VC_EXCEPTION = 0x406D1388;
#pragma pack(push,8)
	typedef struct tagTHREADNAME_INFO
	{
		DWORD dwType; // Must be 0x1000.
		LPCSTR szName; // Pointer to name (in user addr space).
		DWORD dwThreadID; // Thread ID (-1=caller thread).
		DWORD dwFlags; // Reserved for future use, must be zero.
	} THREADNAME_INFO;
#pragma pack(pop)

	// Note that the thread name is copied to the thread so that the memory for the
	// threadName parameter can be released.
	void SetQThreadName(DWORD thread_id, const char* name)
	{
		THREADNAME_INFO info;
		info.dwType = 0x1000;
		info.szName = name;
		info.dwThreadID = thread_id;
		info.dwFlags = 0;

		__try {
			RaiseException(MS_VC_EXCEPTION,
				0,
				sizeof(info) / sizeof(ULONG_PTR),
				(ULONG_PTR*)&info);
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			//
		}
	}
#endif

	QBaseThread::QBaseThread()
	{
		thread_id_ = 0;
		thread_handle_ = NULL;
		thread_priority_ = kQThreadPriorityDefault;
	}

	QBaseThread::~QBaseThread()
	{
		Close();
	}

	bool QBaseThread::Create()
	{
		// create QBaseThread first
		thread_handle_ = (HANDLE)_beginthreadex(NULL,
			0, ThreadProcFunc, this, 0, (unsigned*)&thread_id_);

		if (thread_handle_ < (HANDLE)2)
		{
			thread_handle_ = NULL;
			thread_id_ = 0;
		}

		// if thread need set priority
		if (thread_priority_ > kQThreadPriorityDefault)
		{
			// set the thread priority
			int pri = THREAD_PRIORITY_BELOW_NORMAL;
			if (thread_priority_ == kQThreadPriorityLow)
				pri = THREAD_PRIORITY_LOWEST;
			else if (thread_priority_ == kQThreadPriorityNormal)
				pri = THREAD_PRIORITY_BELOW_NORMAL;
			else if (thread_priority_ == kQThreadPriorityHigh)
				pri = THREAD_PRIORITY_HIGHEST;
			else if (thread_priority_ == kQThreadPriorityRealtime)
				pri = THREAD_PRIORITY_TIME_CRITICAL;
			SetThreadPriority(thread_handle_, pri);
		}

		return thread_handle_ > (HANDLE)1;
	}

	void QBaseThread::Close()
	{
		if (thread_handle_)
		{
			if (WAIT_OBJECT_0 == WaitForSingleObject(thread_handle_, INFINITE))
			{
				::CloseHandle(thread_handle_);
				thread_handle_ = NULL;
				thread_id_ = 0;
			}
			else
			{
				Terminate();
			}
		}
	}

	void QBaseThread::Terminate()
	{
		if (thread_handle_)
		{
			::TerminateThread(thread_handle_, 0);
			::CloseHandle(thread_handle_);
			thread_handle_ = NULL;
			thread_id_ = 0;
		}
	}

	// static
	void QBaseThread::Sleep(int duration_ms)
	{
		::Sleep(duration_ms);
	}

	// static
	void QBaseThread::YieldThread()
	{
		::Sleep(0);
	}

	// static
	QThreadId QBaseThread::CurrentId()
	{
		return GetCurrentThreadId();
	}

	uint32_t __stdcall QBaseThread::ThreadProcFunc(void *arg)
	{
		QBaseThread* the_thread = (QBaseThread*)arg;
		assert(the_thread);
		if (the_thread != NULL)
		{
			the_thread->Run();
		}
		_endthreadex(0);
		return 0;
	}

}  // namespace nbase
