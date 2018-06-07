#include "closure.h"
#include "qthread_manager.h"

namespace qtbase
{
	void Post2Task(int identifier, const StdClosure &closure)
	{
		nbase::QThreadManager::PostTask(identifier, closure);
	}

	void Post2DelayedTask(int identifier, const StdClosure &closure, nbase::TimeDelta delay)
	{
		nbase::QThreadManager::PostDelayedTask(identifier, closure, delay);
	}

	void Post2RepeatedTask(int identifier, const nbase::WeakCallback<StdClosure> &closure, nbase::TimeDelta delay)
	{
		nbase::QThreadManager::PostRepeatedTask(identifier, closure, delay);
	}

}