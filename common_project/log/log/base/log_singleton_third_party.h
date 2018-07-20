#ifndef __LOG_SINGLE_THIRD_PARTY_H__
#define __LOG_SINGLE_THIRD_PARTY_H__
#include <vector>
#include "sys/stat.h"
#include "base/file/file_util.h"
#include "base/util/string_util.h"
#include "base/win32/platform_string_util.h"
#include "log_singleton_interface.h"
#include "../util/log_file_util.h"

template < typename DerivedT >
class LogSingletonThirdParty : public LogSingletonInterface
{
public:
	static DerivedT* GetInstance()
	{
		static DerivedT instance;

		return &instance;
	}

	virtual ~LogSingletonThirdParty() = default;

	template<typename FilePathContainer>
	void			SetLogFilePaths(FilePathContainer&& log_file_paths)
	{
		log_file_paths_ = std::forward<FilePathContainer>(log_file_paths);
	}

	virtual void	GetLogContent(std::string& content) override
	{
		for (const auto& file_path : log_file_paths_)
		{
			log_file_util::GetFileContent(file_path, content);
		}
	}

	virtual void RemoveHalfIfLongerThan(long max) 
	{ 
		time_t loc_time = 0;
		time(&loc_time);

		struct _stat buf;

		for (const auto& file_path : log_file_paths_)
		{
			std::string log_file;
			nbase::win32::UnicodeToMBCS(file_path, log_file);
			if (_stat(log_file.c_str(), &buf) == 0)
			{
				if (buf.st_ctime + 3600 * 24 * 5 < loc_time)
				{
					log_file_util::RemoveHalfIfLongerThan(file_path, max);
				}
			}
		}
	}

private:
	std::vector<std::wstring>	log_file_paths_;
};

#endif