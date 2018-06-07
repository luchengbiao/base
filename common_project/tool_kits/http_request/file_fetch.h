#pragma once
#include <functional>
#include <string>
#include <stdio.h>

typedef std::function<void(std::string)> file_fetcher_complete_cb;
typedef std::function<void(double, double, double, double)> file_fetcher_progress_cb;

class FileFetcher 
{
public:
	FileFetcher(){}
	virtual ~FileFetcher(){}

public:
	int AsyncFetchFile(std::string& url, std::string dir_path, const file_fetcher_complete_cb& comp_cb, bool b_use_cache = true, 
								const file_fetcher_progress_cb& prog_cb = nullptr, std::string file_name = "", std::string suffix = "");

	int AsyncFetchImage(std::string& url, std::string dir_path, const file_fetcher_complete_cb& comp_cb, bool b_use_cache = true,
		const file_fetcher_progress_cb& prog_cb = nullptr, std::string file_name = "");

	bool IsValidImage(std::wstring file_path);

private:
	//bool IsValidImage(std::wstring file_path);
	bool IsCompletedImage(std::wstring file_path);
};