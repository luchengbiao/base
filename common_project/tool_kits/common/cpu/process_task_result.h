#pragma once
#include <string>
#include <vector>

struct CpuProcess
{
	int id_;
	int memory_;//单位为MB;
	float CPU_;	//CPU占用率平均值
	std::string name_;
	
	CpuProcess() : id_(0), memory_(0), CPU_(0), name_("")
	{

	}

	bool operator<(const CpuProcess& other) const
	{
		return memory_ > other.memory_;
	}
};

struct CpuResult
{
	CpuProcess current_process_;	//包含家有学霸的内存和CPU占用率
	double total_cpu;	//总的CPU
	CpuProcess worst_pro;	//占用内存最大的进程
	std::vector<CpuProcess> pro_;//所有进程
	int total_memory_;	//系统内存
	int memory_percent_; //内存使用率
	CpuResult() : total_cpu(0), total_memory_(0), memory_percent_(0)
	{

	}
};
typedef std::function<void(int, std::wstring, CpuResult)> API_GetCpuResCb;