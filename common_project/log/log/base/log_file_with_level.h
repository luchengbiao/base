#ifndef __LOG_FILE_WITH_LEVEL_H__
#define __LOG_FILE_WITH_LEVEL_H__
#include <string>
#include <type_traits>
#include "base\synchronization\lock.h"

class LogFileWithLevel
{
public:
	void					SetLogFilePath(const std::wstring& file_path);
	std::wstring			GetLogFilePath() const;
	void					GetLogContent(std::string& content);
	void					WriteLog(const std::string& log);

	void					RemoveHalfIfLongerThan(long max);

	template< typename LogLevelT >
	void SetLogLevel(LogLevelT log_level)
	{
		static_assert(std::is_integral<LogLevelT>::value || std::is_enum<LogLevelT>::value, "LogLevelT must be integral or enum");

		log_level_ = static_cast<int>(log_level);
	}

	template< typename LogLevelT >
	LogLevelT GetLogLevel() const { return static_cast<LogLevelT>(log_level_); }

private:
	std::wstring			file_path_;
	int						log_level_ = 0;

	mutable nbase::NLock	lock_;
};

#endif