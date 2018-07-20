#include "log_file_with_level.h"
#include "..\util\log_file_util.h"

void LogFileWithLevel::SetLogFilePath(const std::wstring& file_path)
{
	nbase::NAutoLock auto_lock(&lock_);

	file_path_ = file_path;
}

std::wstring LogFileWithLevel::GetLogFilePath() const
{
	nbase::NAutoLock auto_lock(&lock_);

	return file_path_;
}

void LogFileWithLevel::GetLogContent(std::string& content)
{
	nbase::NAutoLock auto_lock(&lock_);

	log_file_util::GetFileContent(file_path_, content);
}

void LogFileWithLevel::WriteLog(const std::string& log)
{
	nbase::NAutoLock auto_lock(&lock_);

	log_file_util::WriteLogToFile(file_path_, log);
}

void LogFileWithLevel::RemoveHalfIfLongerThan(long max)
{
	nbase::NAutoLock auto_lock(&lock_);

	log_file_util::RemoveHalfIfLongerThan(file_path_, max);
}