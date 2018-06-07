#include "qthread_manager.h"
#include "base/framework/message_loop.h"
#include "base/memory/singleton.h"
#include "base/third_party/chrome/atomicops.h"
#include "framework_qthread.h"

#define AUTO_MAP_LOCK() NAutoLock __l(&GetInstance()->lock_);
#define AQUIRE_ACCESS() {if (!AquireAccess()) { /*DCHECK(false);*/ return false; }}

namespace nbase
{

	bool QThreadMap::AquireAccess()
	{
		FrameworkQThreadTlsData *tls = FrameworkQThread::GetTlsData();
		if (!tls || tls->managed < 1)
			return false;
		return true;
	}

	bool QThreadMap::RegisterThread(int self_identifier)
	{
		//DCHECK(self_identifier >= 0);
		if (self_identifier < 0)
			return false;

		FrameworkQThreadTlsData *tls = FrameworkQThread::GetTlsData();
		//DCHECK(tls); // should be called by a Framework thread
		if (tls == NULL)
			return false;

		AUTO_MAP_LOCK()
			std::pair<std::map<int, FrameworkQThread*>::iterator, bool> pr =
			QThreadMap::GetInstance()->threads_.insert(
			std::make_pair(self_identifier, tls->self));
		if (!pr.second)
		{
			if (pr.first->second != tls->self)
			{
				//DCHECK(false); // another thread has registered with the same id
				return false;
			}
			// yes, it's me, check logic error
			//DCHECK(tls->managed > 0);
			//DCHECK(tls->managed_thread_id == self_identifier);
		}
		// 'self' is registered
		tls->managed++;
		tls->managed_thread_id = self_identifier;
		return true;
	}

	bool QThreadMap::UnregisterThread()
	{
		FrameworkQThreadTlsData *tls = FrameworkQThread::GetTlsData();
		//DCHECK(tls); // should be called by a Framework thread
		//DCHECK(tls->managed > 0); // should be managed
		if (!tls || tls->managed < 1)
			return false;

		// remove from internal thread map
		// here, since tls->managed is greater than zero,
		// we must have a reference of the glabal QThreadManager instance (see RegisterThread)
		if (--tls->managed == 0)
		{
			AUTO_MAP_LOCK()
				std::map<int, FrameworkQThread*>::iterator iter = GetInstance()->threads_.find(tls->managed_thread_id);
			if (iter != GetInstance()->threads_.end())
				GetInstance()->threads_.erase(iter);
			else {}
			//DCHECK(false);	// logic error, we should not come here
			tls->managed_thread_id = -1;
		}

		return true;
	}

	// no lock
	FrameworkQThread* QThreadMap::QueryThreadInternal(int identifier) const
	{
		std::map<int, FrameworkQThread*>::iterator iter
			= GetInstance()->threads_.find(identifier);
		if (iter == GetInstance()->threads_.end())
			return NULL;
		return iter->second;
	}

	int QThreadMap::QueryThreadId(const FrameworkQThread *thread)
	{
		AQUIRE_ACCESS()
			AUTO_MAP_LOCK()
			std::map<int, FrameworkQThread*>::iterator iter;
		for (iter = GetInstance()->threads_.begin();
			iter != GetInstance()->threads_.end(); iter++) {
			if (iter->second == thread)
				return iter->first;
		}
		return -1;
	}

	std::shared_ptr<MessageLoopProxy> QThreadMap::GetMessageLoop(int identifier) const
	{
		FrameworkQThread *thread = QueryThreadInternal(identifier);
		if (thread == NULL)
			return NULL;
		MessageLoop *message_loop = thread->message_loop();
		if (message_loop == NULL)
			return NULL;
		return message_loop->message_loop_proxy();
	}

	bool QThreadManager::RegisterThread(int self_identifier)
	{
		return QThreadMap::GetInstance()->RegisterThread(self_identifier);
	}

	bool QThreadManager::UnregisterThread()
	{
		return QThreadMap::GetInstance()->UnregisterThread();
	}

	int QThreadManager::QueryThreadId(const FrameworkQThread *thread)
	{
		return QThreadMap::GetInstance()->QueryThreadId(thread);
	}

	FrameworkQThread* QThreadManager::CurrentThread()
	{
		FrameworkQThreadTlsData *tls = FrameworkQThread::GetTlsData();
		//DCHECK(tls); // should be called by a Framework thread
		//DCHECK(tls->managed > 0); // should be managed

		if (!tls || tls->managed < 1)
			return NULL;
		return tls->self;
	}

	bool QThreadManager::PostTask(int identifier, const StdClosure &task)
	{
		std::shared_ptr<MessageLoopProxy> message_loop =
			QThreadMap::GetInstance()->GetMessageLoop(identifier);
		if (message_loop == NULL)
			return false;
		message_loop->PostTask(task);
		return true;
	}

	bool QThreadManager::PostDelayedTask(int identifier, const StdClosure &task, TimeDelta delay)
	{
		std::shared_ptr<MessageLoopProxy> message_loop =
			QThreadMap::GetInstance()->GetMessageLoop(identifier);
		if (message_loop == NULL)
			return false;
		message_loop->PostDelayedTask(task, delay);
		return true;
	}

	void QThreadManager::PostRepeatedTask(int thread_id, const WeakCallback<StdClosure>& task, const TimeDelta& delay, int times)
	{
		StdClosure closure = nbase::Bind(&QThreadManager::RunRepeatedly2, thread_id, task, delay, times);
		nbase::QThreadManager::PostDelayedTask(thread_id, closure, delay);
	}

	void QThreadManager::RunRepeatedly2(int thread_id, const WeakCallback<StdClosure>& task, const TimeDelta& delay, int times)
	{
		if (task.Expired()) {
			return;
		}
		task();
		if (task.Expired()) {
			return;
		}
		if (times != TIMES_FOREVER) {
			times--;
		}
		if (times != 0) {
			PostRepeatedTask(thread_id, task, delay, times);
		}
	}


}
