#pragma once

#include <Windows.h> 
#include <stdint.h>
//原理：调用GetProcessTimes()，并与上次调用得到的结果相减，即得到某段时间内CPU的使用时间  
//C++ 获取特定进程规定CPU使用率  原文：http://blog.csdn.net/liuqx97bb/article/details/52058657  
class CPUusage 
{
public:
	CPUusage(DWORD process_id);
	CPUusage();
	~CPUusage();

	//返回值为进程句柄，可判断OpenProcess是否成功  
	HANDLE Setpid(DWORD process_id);

	//-1 即为失败或进程已退出； 如果成功，首次调用会返回-2（中途用setpid更改了PID后首次调用也会返回-2）  
	float GetCpuUsage();

private:
	// 时间转换    
	uint64_t FileTime2Utc(const FILETIME* ftime);

	// 获得CPU的核数    
	int GetProcessorNumber();

	//初始化  
	void Init();

	//关闭进程句柄  
	void Clear();

private:
	HANDLE hProcess_;
	int processor_;    //cpu数量    
	int64_t last_time_;         //上一次的时间    
	int64_t last_system_time_;
};