#include "downloaded_files_recorder.h"
#include "base/file/file_util.h"

GAME_NAMESPACE_BEGIN

void DownloadedFilesRecorder::Record(const std::string& file_url, const std::wstring& file_path)
{
	std::lock_guard<std::recursive_mutex> lock(mutex_);

	url_2_file_.emplace(file_url, file_path);
}

bool DownloadedFilesRecorder::ExistingFileOfUrl(const std::string& file_url, std::wstring* file_path)
{
	bool existing = false;

	{
		std::lock_guard<std::recursive_mutex> lock(mutex_);

		auto it = url_2_file_.find(file_url);
		if (it != url_2_file_.cend())
		{
			if (nbase::FilePathIsExist(it->second, false))
			{
				existing = true;
				if (file_path) { *file_path = it->second; }
			}
			else
			{
				url_2_file_.erase(it);
			}
		}
	}

	return existing;
}

GAME_NAMESPACE_END