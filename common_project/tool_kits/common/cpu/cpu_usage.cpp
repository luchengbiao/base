#include "cpu_usage.h"

CPUusage::CPUusage(DWORD process_id) {
	Init();
	processor_ = GetProcessorNumber();
	Setpid(process_id);
}

CPUusage::CPUusage()
{
	Init(); 
	processor_ = GetProcessorNumber();
}

CPUusage::~CPUusage()
{
	Clear();
}

HANDLE CPUusage::Setpid(DWORD process_id)
{
	Clear();    //如果之前监视过另一个进程，就先关闭它的句柄  
	Init();
	return hProcess_ = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, false, process_id);
}

float CPUusage::GetCpuUsage()
{

	FILETIME now;
	FILETIME creation_time;
	FILETIME exit_time;
	FILETIME kernel_time;
	FILETIME user_time;
	int64_t system_time;
	int64_t time;
	int64_t system_time_delta;
	int64_t time_delta;

	DWORD exitcode;

	float cpu = -1;

	if (!hProcess_) {
		return -1;
	}

	GetSystemTimeAsFileTime(&now);

	//判断进程是否已经退出  
	GetExitCodeProcess(hProcess_, &exitcode);
	if (exitcode != STILL_ACTIVE) {
		Clear();
		return -1;
	}

	//计算占用CPU的百分比  
	if (!GetProcessTimes(hProcess_, &creation_time, &exit_time, &kernel_time, &user_time))
	{
		Clear();
		return -1;
	}
	system_time = (FileTime2Utc(&kernel_time) + FileTime2Utc(&user_time))
		/ processor_;
	time = FileTime2Utc(&now);

	//判断是否为首次计算  
	if ((last_system_time_ == 0) || (last_time_ == 0))
	{
		last_system_time_ = system_time;
		last_time_ = time;
		return -2;
	}

	system_time_delta = system_time - last_system_time_;
	time_delta = time - last_time_;

	if (time_delta == 0) {
		return -1;
	}

	cpu = (float)system_time_delta * 100 / (float)time_delta;
	last_system_time_ = system_time;
	last_time_ = time;
	return cpu;
}

uint64_t CPUusage::FileTime2Utc(const FILETIME* ftime)
{
	LARGE_INTEGER li;

	li.LowPart = ftime->dwLowDateTime;
	li.HighPart = ftime->dwHighDateTime;
	return li.QuadPart;
}

int CPUusage::GetProcessorNumber()
{
	SYSTEM_INFO info;
	GetSystemInfo(&info);
	return info.dwNumberOfProcessors;
}

void CPUusage::Init()
{
	last_system_time_ = 0;
	last_time_ = 0;
	hProcess_ = 0;
}

void CPUusage::Clear()
{
	if (hProcess_) {
		CloseHandle(hProcess_);
		hProcess_ = 0;
	}
}