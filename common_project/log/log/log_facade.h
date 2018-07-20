#ifndef __LOG_TOOL_H__
#define __LOG_TOOL_H__
#include <stdint.h> // for int64_t
#include <memory>
#include <string>
#include "base/memory/singleton.h"

struct CustomLogItem;

class LogFacade
{
public:
	SINGLETON_DEFINE(LogFacade);

public:
	void				SetUploadApi(const std::string& api_upload_log, const std::string& api_get_upload_token);

	void				SetCommonLogDirectory(const std::wstring& common_log_dir);

	void				SetClassOnlineLogFilePath(const std::wstring& class_online_log_path);

	void				UploadClassOnlineLog();

	void				UploadCrashLog(int64_t biz_id, const std::string& content = "pc_crash_log");

	void				UploadCustomLog(const CustomLogItem& log_item);

	void				HandleConsoleCommand(const std::string& command);

protected:
	LogFacade();

private:
	friend class LogFacadeImpl;
	std::shared_ptr<LogFacadeImpl> impl_;
};

#define LOG_FACADE LogFacade::GetInstance()

#endif