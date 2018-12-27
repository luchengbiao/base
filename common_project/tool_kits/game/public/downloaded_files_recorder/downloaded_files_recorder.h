#ifndef __GAME_PUBLIC_DOWNLOADED_FILES_RECORDER_DOWNLOADED_FILES_RECORDER_H__
#define __GAME_PUBLIC_DOWNLOADED_FILES_RECORDER_DOWNLOADED_FILES_RECORDER_H__
#include <mutex>
#include <unordered_map>
#include <string>
#include "base/memory/singleton.h"
#include "game/macros.h"

GAME_NAMESPACE_BEGIN

class DownloadedFilesRecorder
{
	typedef std::unordered_map<std::string, std::wstring> Url2FileMap;

public:
	SINGLETON_DEFINE(DownloadedFilesRecorder);

	void					Record(const std::string& file_url, const std::wstring& file_path);

	bool					ExistingFileOfUrl(const std::string& file_url, std::wstring* file_path = nullptr);

private:
	DownloadedFilesRecorder() = default;

private:
	std::recursive_mutex	mutex_;
	Url2FileMap				url_2_file_;
};

#define DOWNLOADED_FILES_RECORDER DownloadedFilesRecorder::GetInstance()

inline bool					ExistingFileOfUrl(const std::string& file_url, std::wstring* file_path = nullptr) 
{ 
	return DOWNLOADED_FILES_RECORDER->ExistingFileOfUrl(file_url, file_path);
}

GAME_NAMESPACE_END

#endif