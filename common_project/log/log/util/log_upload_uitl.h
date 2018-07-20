#ifndef __LOG_UPLOAD_UTIL_H__
#define __LOG_UPLOAD_UTIL_H__
#include <stdint.h> // for int64_t
#include <functional>
#include <map>
#include <vector>

class LogSingletonInterface;

namespace log_upload_util
{
	using UploadedCallback = std::function<void(bool, const std::string&)>;

	void				SetUploadApi(const std::string& api_upload_log, const std::string& api_get_upload_token);

	void				UploadLogsInSingleOneFile(const std::vector<LogSingletonInterface*>& vec_log_singleton,
												  const std::string& biz_id,
												  const std::string& content,
												  const std::wstring& temp_upload_file_path, 
												  const UploadedCallback& uploaded_cb = nullptr);

	bool				UploadFile(const std::wstring& file_path, const UploadedCallback& uploaded_cb = nullptr);

	void				UploadCustomLog(const std::map<std::string, std::string>& custom_log);
}

#endif