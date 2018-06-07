#pragma once
#include "base\synchronization\lock.h"
#include "base\memory\singleton.h"
#include <string>
/*  仿照log.h
*   author:
*   zsj
*/

enum LOG_CLASS
{
	LV_CALL = 0,
	LV_O2O = 1,
	LV_BANKE = 2,
};

class QClassOnlineLogHelper;

class QClassOnlineLogImpl
{
	friend class QClassOnlineLogHelper;
public:
	SINGLETON_DEFINE(QClassOnlineLogImpl);
	QClassOnlineLogImpl() { }
public:
	void SetLogFile(const std::wstring &file_path); //full path
	std::wstring GetLogFile();
	void SetLoginLog(bool b_login);
	bool IsLoginLog();
	void SetLogLevel(LOG_CLASS lv);
	void HalfTo(long max, long to);
private:
	void WriteLog(LOG_CLASS lv, const std::wstring &log);
private:
	std::wstring full_log_file_;
	LOG_CLASS	 log_level_;
	bool b_login_log_;
	nbase::NLock lock_;
};

class QClassOnlineLogHelper
{
public:
	QClassOnlineLogHelper(const char* file, long line);
	~QClassOnlineLogHelper();
	QClassOnlineLogHelper& VLog(LOG_CLASS lv, const std::wstring &fmt);
	QClassOnlineLogHelper& operator<<(const std::wstring &str);
	QClassOnlineLogHelper& operator<<(const std::string &str);
	QClassOnlineLogHelper& operator<<(const long long lld);
private:
	size_t FindArg(int &len); //返回"{x}"的索引，len代表"{x}"的长度
private:
	std::wstring fmt_;
	std::wstring string_;
	int			 index_;

	std::wstring time_;
	std::wstring file_line_;
	LOG_CLASS	 level_;
};

#define QONLINELOG_CALL(fmt) QClassOnlineLogHelper(__FILE__, __LINE__).VLog(LV_CALL, fmt)
#define QONLINELOG_O2O(fmt) QClassOnlineLogHelper(__FILE__, __LINE__).VLog(LV_O2O, fmt)
#define QONLINELOG_BANKE(fmt) QClassOnlineLogHelper(__FILE__, __LINE__).VLog(LV_BANKE, fmt)