#ifndef __LOG_FILE_UTIL_H__
#define __LOG_FILE_UTIL_H__
#include <string>

namespace log_file_util
{
	void	WriteLogToFile(const std::wstring& file_path, const std::string& log);

	void	GetFileContent(const std::wstring& file_path, std::string& content);

	void	RemoveHalfIfLongerThan(const std::wstring& file_path, long max);
}

#endif