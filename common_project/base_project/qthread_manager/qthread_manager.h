#pragma once

#include <map>
#include <memory>
#include "base/memory/singleton.h"
#include "framework_qthread.h"

namespace nbase
{
	class MessageLoop;
	class MessageLoopProxy;

	class QThreadMap
	{
	public:
		SINGLETON_DEFINE(QThreadMap);
		static bool AquireAccess();

		bool RegisterThread(int self_identifier);
		bool UnregisterThread();
		int QueryThreadId(const FrameworkQThread *thread);
		std::shared_ptr<MessageLoopProxy> GetMessageLoop(int identifier) const;
		FrameworkQThread* QueryThreadInternal(int identifier) const;

	private:
		QThreadMap() { }
		NLock lock_;
		std::map<int, FrameworkQThread*> threads_;
	};

	// 使用ThreadManager可以极大地方便线程间通信
	// 注意：只有受ThreadManager托管的线程（通过Register托管）才允许调用除Register和Post族外的成员函数
	class BASE_EXPORT QThreadManager
	{
	public:

		// 托管当前FrameworkQThread线程
		// identifier >= 0
		// 必须在self的线程过程内被调用
		static bool RegisterThread(int self_identifier);
		// 取消当前线程托管
		// 线程运行结束之前必须调用UnregisterThread取消托管
		static bool UnregisterThread();

		// 查找
		static FrameworkQThread* CurrentThread();
		template<typename T> static T* CurrentThreadT();
		static int QueryThreadId(const FrameworkQThread *thread);

		static bool PostTask(int identifier, const StdClosure &task);

		static bool PostDelayedTask(int identifier, const StdClosure &task, TimeDelta delay);

		static const int TIMES_FOREVER = -1;
		static void PostRepeatedTask(int thread_id, const WeakCallback<StdClosure>& task, const TimeDelta& delay, int times = TIMES_FOREVER);

		template<typename T1, typename T2>
		static bool Await(int identifier, const std::function<T1> &task, const std::function<T2> &reply)
		{
			std::shared_ptr<MessageLoopProxy> message_loop =
				ThreadMap::GetInstance()->GetMessageLoop(identifier);
			if (message_loop == NULL)
				return false;
			message_loop->PostTaskAndReply(task, reply);
			return true;
		}

	private:
		static void RunRepeatedly2(int thread_id, const WeakCallback<StdClosure>& task, const TimeDelta& delay, int times);

		DISALLOW_COPY_AND_ASSIGN(QThreadManager);
	};

	template<typename T>
	T* QThreadManager::CurrentThreadT()
	{
		return static_cast<T *>(CurrentThread());
	}

}

