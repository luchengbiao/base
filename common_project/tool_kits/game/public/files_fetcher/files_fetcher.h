#ifndef __GAME_PUBLIC_FILES_FETCHER_FILES_FETCHER_H__
#define __GAME_PUBLIC_FILES_FETCHER_FILES_FETCHER_H__
#include <functional>
#include <string>
#include <unordered_map>
#include "base/base_types.h"
#include "common/weak_callback/weak_callback.h"
#include "game/macros.h"

GAME_NAMESPACE_BEGIN

class FilesFetcher;
typedef std::shared_ptr<FilesFetcher> FilesFetcherSharedPtr;

typedef std::function<void()> FilesFetchedCallback;
typedef std::function<void(double, double)> FilesFetchProgressCallback;

class FilesFetcher : public wcb::SupportWeakCallback<FilesFetcher>
{
	typedef wcb::WeakCallbackFlagCancelable<FilesFetcher> MyWeakCallbackFlag;

	typedef std::unordered_map<std::string, std::wstring> Url2FileMap;
	typedef std::unordered_map<std::string, std::wstring> Url2DirMap;
	typedef std::pair<double, double> ProgressInfo;
	typedef std::unordered_map<std::string, ProgressInfo> Url2ProgressMap;

public:
	static FilesFetcherSharedPtr	Create();

public:
	template<typename FileUrls>
	void							Fetch(const FileUrls& file_urls,
										  const std::wstring& dir,
										  const FilesFetchedCallback& fetched_callback,
										  const FilesFetchProgressCallback& progress_callback = nullptr);

	void							Fetch(const Url2DirMap& file_url_2_dir,
										  const FilesFetchedCallback& fetched_callback,
										  const FilesFetchProgressCallback& progress_callback = nullptr);

	void							Cancel();

	Url2FileMap						GetUrlToFilePathMap() const;

protected:
	FilesFetcher();
	void							Init();

protected:
	MyWeakCallbackFlag				fetch_cllback_flag_;
	Url2FileMap						map_url_to_file_;
	mutable std::recursive_mutex	mutex_;

	Url2ProgressMap					map_url_to_progress_;
};

template<typename FileUrls>
void FilesFetcher::Fetch(const FileUrls& file_urls, 
						 const std::wstring& dir,
						 const FilesFetchedCallback& fetched_callback,
						 const FilesFetchProgressCallback& progress_callback)
{
	Url2DirMap file_url_2_dir;
	for (const auto& file_url : file_urls)
	{
		file_url_2_dir.emplace(file_url, dir);
	}

	this->Fetch(file_url_2_dir, fetched_callback, progress_callback);
}

GAME_NAMESPACE_END

#endif