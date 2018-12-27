#pragma once
#include "base\callback\callback.h"
#include "base\time\time.h"

enum ThreadId
{
	kThreadApiTask = 0,	//api任务线程
	kThreadHeartBeat, // 心跳任务线程
	kThreadHttpTask, // http 回调请求线程
	kThreadUIHelper, // 通知主线程的辅助线程
	kThreadMoreTaskHelper, // 更多定时任务通知主线程的辅助线程
	kThreadWebSocket, // 建立websocket 连接时候的线程
};

namespace qtbase
{
	/** @fn Post2Task(int identifier, const QtTaskClosure &closure)
	* 把函数放到某个线程执行
	* @param[in] identifier 线程的id
	* @param[in] closure 被执行的函数
	* @return void 无返回值
	*/
	void Post2Task(int identifier, const StdClosure &closure);

	/** @fn void Post2DelayedTask(int identifier, const QtTaskClosure &closure, int delay)
	* 把函数放到某个线程延时执行
	* @param[in] identifier 线程的id
	* @param[in] closure 被执行的函数
	* @param[in] delay 延时执行的时间
	* @return void 无返回值
	*/
	void Post2DelayedTask(int identifier, const StdClosure &closure, nbase::TimeDelta delay);

	/** @fn void Post2RepeatedTask(int identifier, const QtTaskClosure &closure, int delay)
	* 把函数放到某个线程循环执行
	* @param[in] identifier 线程的id
	* @param[in] closure 被执行的函数
	* @param[in] delay 每次循环执行的间隔
	* @return void 无返回值
	*/
	void Post2RepeatedTask(int identifier, const nbase::WeakCallback<StdClosure> &closure, nbase::TimeDelta delay);
}