#pragma once
#include "base_task_thread.h"
#include <memory>
#include "base\memory\singleton.h"

class BaseThreadManager
{
public:
	SINGLETON_DEFINE(BaseThreadManager);
	BaseThreadManager(){}
	virtual ~BaseThreadManager() {}

	void Init();
	/**
	* 主线程退出时，做一些清理工作
	* @return void	无返回值
	*/
	void Cleanup();

private:
	/**
	* 主线程开始循环前，misc线程和db线程先开始循环
	* @return void	无返回值
	*/
	void PreMessageLoop();

	/**
	* 主线程结束循环前，misc线程和db线程先结束循环
	* @return void	无返回值
	*/
	void PostMessageLoop();
private:
	std::unique_ptr<BaseTaskThread>	  api_task_thread_;
	std::unique_ptr<BaseTaskThread>	  heartbeat_task_thread_;
	std::unique_ptr<BaseTaskThread>	  http_task_thread_;
	std::unique_ptr<BaseTaskThread>	  uihelper_task_thread_;
	std::unique_ptr<BaseTaskThread>	  more_task_helper_thread_;
	std::unique_ptr<BaseTaskThread>	  ws_task_thread_;
};