#pragma once
#include "base\callback\callback.h"
#include "base\memory\singleton.h"
#include "process_task_result.h"
#include <vector>
#include <WTypes.h>

class DetectSystemManager : public nbase::SupportWeakCallback
{
public:
	SINGLETON_DEFINE(DetectSystemManager);
	DetectSystemManager();
	~DetectSystemManager();

	CpuResult GetTaskResult(int process_id);
	void StopTask();

private:
	bool AdjustPurview();//获取权限
	bool TraverseProcesses();	//获取CPU和内存信息
	__int64 CalcPrivateWorkSet(HANDLE hProcess, __int64 page);	//根据句柄和pagesize计算内存
	CpuProcess GetMaxMemory();
	double TotalCPU();
	__int64 CompareFileTime2(FILETIME time1, FILETIME time2);
	void Entry();	//启动入口

private:
	std::vector<CpuProcess> pro_;
	FILETIME m_preidleTime_;
	FILETIME m_prekernelTime_;
	FILETIME m_preuserTime_;
	CpuProcess current_process_;
	int process_id_;
	bool b_checking_;
};