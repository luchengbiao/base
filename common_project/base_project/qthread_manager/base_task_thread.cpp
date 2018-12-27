#include "base_task_thread.h"
#include "qthread_manager.h"
BaseTaskThread::BaseTaskThread(nbase::QThreadId thread_id, const char* name)
: FrameworkQThread(name), thread_id_(thread_id)
{

}

BaseTaskThread::~BaseTaskThread(void)
{

}

void BaseTaskThread::Init()
{
	nbase::QThreadManager::RegisterThread(thread_id_);
}

void BaseTaskThread::Cleanup()
{
	nbase::QThreadManager::UnregisterThread();

	//LOG_INFO(L"ApiTaskThread Cleanup");
}