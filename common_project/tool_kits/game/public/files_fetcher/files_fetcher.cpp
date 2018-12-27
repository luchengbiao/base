#include "files_fetcher.h"
#include "base/util/string_util.h"
#include "http_request/file_fetch.h"

GAME_NAMESPACE_BEGIN

FilesFetcherSharedPtr FilesFetcher::Create()
{
	FilesFetcherSharedPtr ptr(new FilesFetcher);

	if (ptr)
	{
		ptr->Init();
	}

	return ptr;
}

FilesFetcher::FilesFetcher()
{}

void FilesFetcher::Init()
{
	fetch_cllback_flag_.SetWeakPtr(GetWeakPtr());
}

void FilesFetcher::Fetch(const std::unordered_map<std::string, std::wstring>& file_url_2_dir,
						 const FilesFetchedCallback& fetched_callback,
						 const FilesFetchProgressCallback& progress_callback)
{
	fetch_cllback_flag_.Cancel();

	{
		std::lock_guard<std::recursive_mutex> lock(mutex_);
		map_url_to_file_.clear();
		map_url_to_progress_.clear();
	}

	if (!file_url_2_dir.empty())
	{
		for (auto& pair : file_url_2_dir)
		{
			auto url = pair.first;
			auto dir = nbase::UTF16ToUTF8(pair.second);

			auto cbCompleted = fetch_cllback_flag_.ToWeakCallback([=](const std::string& file_path){
				std::wstring file_path_w = std::move(nbase::UTF8ToUTF16(file_path));
				bool all_fetched = false;
				
				{
					std::lock_guard<std::recursive_mutex> lock(mutex_);
					map_url_to_file_.emplace(url, file_path_w);
					all_fetched = (map_url_to_file_.size() == file_url_2_dir.size());
				}

				if (all_fetched && fetched_callback)
				{
					fetched_callback();
				}
			});

			auto cbProgress = fetch_cllback_flag_.ToWeakCallback([=](double d1, double d2, double d3, double d4)
			{
				//double progress = (d4 * 100 / d3);

				if (progress_callback)
				{
					double fetched = 0;
					double total = 0;

					{
						std::lock_guard<std::recursive_mutex> lock(mutex_);
						auto it = map_url_to_progress_.find(url);
						if (it == map_url_to_progress_.end())
						{
							map_url_to_progress_.emplace(url, std::make_pair(0, 0));

							it = map_url_to_progress_.find(url);
						}

						it->second.first = d4;
						it->second.second = d3;

						for (const auto& pair : map_url_to_progress_)
						{
							fetched += pair.second.first;
							total += pair.second.second;
						}
					}

					progress_callback(fetched, total);
				}
			});

			FileFetcher ffetcher;
			ffetcher.AsyncFetchFile(const_cast<std::string&>(url), dir, cbCompleted, true, cbProgress, "", "");
		}
	}
	else
	{
		if (fetched_callback)
		{
			fetched_callback();
		}
	}
}

void FilesFetcher::Cancel()
{
	fetch_cllback_flag_.Cancel();

	std::lock_guard<std::recursive_mutex> lock(mutex_);
	map_url_to_file_.clear();
	map_url_to_progress_.clear();
}

std::unordered_map<std::string, std::wstring> FilesFetcher::GetUrlToFilePathMap() const
{
	std::lock_guard<std::recursive_mutex> lock(mutex_);

	return map_url_to_file_;
}

GAME_NAMESPACE_END