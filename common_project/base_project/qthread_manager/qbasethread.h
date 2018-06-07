// Thread is the low-level platform-specific abstraction
// to the OS's threading interface. 

#pragma once

#include "base/base_config.h"
#include "base/base_export.h"

#if defined(OS_WIN)
#include <process.h>
#include <windows.h>
#else
#include <pthread.h>
#endif

#include "base/base_types.h"
#include "QObject"

namespace nbase
{

#if defined(OS_WIN)
	typedef uint32_t   QThreadId;
	typedef void*      QThreadHandle;
#elif defined(OS_POSIX)
	typedef pid_t      QThreadId;
	typedef pthread_t  QThreadHandle;
#endif

	const QThreadId kInvalidQThreadId = 0;

	// Valid values for thread priority
	enum QThreadPriority
	{
		kQThreadPriorityDefault,
		kQThreadPriorityLow,
		kQThreadPriorityNormal,
		kQThreadPriorityHigh,
		kQThreadPriorityRealtime
	};

	class BASE_EXPORT QBaseThread : public QObject
	{
	public:
		QBaseThread();
		virtual ~QBaseThread();

		// Create a thread with a option, if the option is null, thread will start with
		// a default option
		bool Create();

		// Close the thread
		void Close();

		// Terminate the thread
		void Terminate();

		// Sleeps for the specified duration (units are milliseconds)
		static void Sleep(int duration_ms);

		// Yield the current thread so another thread can be scheduled
		static void YieldThread();

		// Gets the current thread id
		static QThreadId CurrentId();

		QThreadId thread_id() const { return thread_id_; }

		void set_thread_id(QThreadId thread_id) { thread_id_ = thread_id; }

		// Gets the current thread handle
		QThreadHandle thread_handle() const  { return thread_handle_; }

		// Set the thread's priority
		void set_thread_priority(QThreadPriority priority) { thread_priority_ = priority; }

	protected:
		// Thread process function
		virtual void Run() {}

	private:
#if defined(OS_WIN)
		static uint32_t __stdcall ThreadProcFunc(void *arg);
#else
		static void* ThreadProcFunc(void *arg);
#endif

		QThreadId       thread_id_;
		QThreadHandle   thread_handle_;
		QThreadPriority thread_priority_;
	};

#if defined(OS_WIN) && defined(COMPILER_MSVC)
	BASE_EXPORT void SetQThreadName(DWORD thread_id, const char* name);
#endif

}
