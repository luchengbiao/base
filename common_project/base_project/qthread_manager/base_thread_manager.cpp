#include "base_thread_manager.h"
#include "base\thread\thread_manager.h"
#include "base\util\util.h"
#include "closure.h"


void BaseThreadManager::Init()
{
	PreMessageLoop();
}

void BaseThreadManager::Cleanup()
{
	PostMessageLoop();
	nbase::ThreadManager::UnregisterThread();
}

void BaseThreadManager::PreMessageLoop()
{
	api_task_thread_.reset(new BaseTaskThread(kThreadApiTask, "Api Task Thread"));
	api_task_thread_->Start();

	heartbeat_task_thread_.reset(new BaseTaskThread(kThreadHeartBeat, "Heartbeat Task Thread"));
	heartbeat_task_thread_->Start();

	http_task_thread_.reset(new BaseTaskThread(kThreadHttpTask, "Http Task Thread"));
	http_task_thread_->Start();

	uihelper_task_thread_.reset(new BaseTaskThread(kThreadUIHelper, "UiHelper Task Thread"));
	uihelper_task_thread_->Start();

	more_task_helper_thread_.reset(new BaseTaskThread(kThreadMoreTaskHelper, "More Task Helper Thread"));
	more_task_helper_thread_->Start();

	ws_task_thread_.reset(new BaseTaskThread(kThreadWebSocket, "WebSocket Task Thread"));
	ws_task_thread_->Start();
}

void BaseThreadManager::PostMessageLoop()
{
	api_task_thread_->Stop();
	api_task_thread_.reset(NULL);

	heartbeat_task_thread_->Stop();
	heartbeat_task_thread_.reset(NULL);

	http_task_thread_->Stop();
	http_task_thread_.reset(NULL);

	uihelper_task_thread_->Stop();
	uihelper_task_thread_.reset(NULL);

	more_task_helper_thread_->Stop();
	more_task_helper_thread_.reset(NULL);

	ws_task_thread_->Stop();
	ws_task_thread_.reset(NULL);
}
