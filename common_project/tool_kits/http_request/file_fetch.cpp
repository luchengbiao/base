#include "file_fetch.h"
#include "curl\curl.h"
#include "nim_sdk_manager/api/nim_tools_http_cpp.h"
#include "base\util\string_number_conversions.h"
#include "log_manager\log.h"
#include "base\file\file_util.h"
#include "base\util\string_util.h"
#include "qthread_manager\closure.h"
#include <string>
#include "QString"
#include "QFileInfo"
#include "QtGui\QImage"
#include <xutility>

using namespace nim_http;
#define MIN_IMAGE_SIZE 20
void RemoveInvalidCharFromFilename(std::string& file_name)
{
	for (auto it = file_name.begin(); it != file_name.end();)
	{
		if ((*it) == '-' || (*it) == '_' || (*it) == '.' || isdigit(*it) || isalpha(*it))
		{
			++it;
		}
		else
		{
			file_name.erase(it);
		}
	}
}

std::string GetFileNameFromUrl(std::string url_, std::string suffix_)
{
	int param_pos = url_.find_last_of("?");
	if (param_pos != std::string::npos)
	{
		url_ = url_.substr(0, param_pos);
	}
	int n = url_.find_last_of("/");

	std::string p;
	if (n > 0)
	{
		p = url_.substr(n + 1);
	}
	else
	{
		p = nbase::Int64ToString(time(0)).append("_").append(nbase::Int64ToString(rand()));
	}

	size_t pos = p.rfind(".");
	if (pos == std::string::npos)
	{
		p.append(suffix_);
	}

	return p;
}

int FileFetcher::AsyncFetchFile(std::string& url, std::string dir_path, const file_fetcher_complete_cb& comp_cb, bool b_use_cache /*= true*/, 
									const file_fetcher_progress_cb& prog_cb /*= nullptr*/, std::string file_name, std::string suffix /*= ""*/)
{
	if (url.empty())
	{
		if (comp_cb)
		{
			qtbase::Post2Task(kThreadUIHelper, std::bind(comp_cb, std::string("")));
		}
		return -1;
	}

	std::string file_dir = dir_path;
	if (file_name.empty())
	{
		file_name = GetFileNameFromUrl(url, suffix);
		RemoveInvalidCharFromFilename(file_name);
	}
	else
	{
		file_name.append(suffix);
	}
	std::string full_path = file_dir.append(file_name);

	auto complete_cb_ = [comp_cb, full_path, url, suffix](bool b_success, int response_code)
	{
		if (b_success && comp_cb)
		{
			if (nbase::FilePathIsExist(nbase::UTF8ToUTF16(full_path), false))
			{
				qtbase::Post2Task(kThreadUIHelper, std::bind(comp_cb, std::string(full_path)));
			}
			else
			{
				auto task = [=]()
				{
					if (nbase::FilePathIsExist(nbase::UTF8ToUTF16(full_path), false))
					{
						comp_cb(std::string(full_path));
					}
					else
					{
						comp_cb("");
					}
				};

				qtbase::Post2DelayedTask(kThreadUIHelper, task, nbase::TimeDelta::FromSeconds(2));
			}
		}
		else
		{
			if (comp_cb)
			{
				qtbase::Post2Task(kThreadUIHelper, std::bind(comp_cb, std::string("")));
			}
			QLOG_WAR(L"async fetch file failed {0} reason code {1}") << url << response_code;
		}
	};

	auto progress_cb_ = [=](double d1, double d2, double d3, double d4)
	{
		if ((d1 > 0 || d2 > 0 || d3 > 0 || d4 > 0) 
			&& prog_cb)
		{
			qtbase::Post2Task(kThreadUIHelper, std::bind(prog_cb, d1, d2, d3, d4));
		}

	};

	nbase::PathString path = nbase::UTF8ToUTF16(full_path);
	if (b_use_cache && nbase::FilePathIsExist(path, false))
	{
		complete_cb_(true, 200);
		return 1;
	}
	else
	{
		HttpRequest http_request(url, full_path, complete_cb_, progress_cb_);
		http_request.AddHeader("Accept", "*/*");

		return PostRequest(http_request);
	}
}

int FileFetcher::AsyncFetchImage(std::string& url, std::string dir_path, const file_fetcher_complete_cb& comp_cb, bool b_use_cache /*= true*/, const file_fetcher_progress_cb& prog_cb /*= nullptr*/, std::string file_name /*= ""*/)
{
	if (url.empty())
	{
		if (comp_cb)
		{
			qtbase::Post2Task(kThreadUIHelper, std::bind(comp_cb, std::string("")));
		}
		return -1;
	}

	std::string file_dir = dir_path;
	if (file_name.empty())
	{
		file_name = GetFileNameFromUrl(url, "");
		RemoveInvalidCharFromFilename(file_name);
	}
	std::string full_path = file_dir.append(file_name);

	auto complete_cb_ = [this, comp_cb, full_path, url](bool b_success, int response_code)
	{
		if (b_success && comp_cb)
		{
			if (IsValidImage(nbase::UTF8ToUTF16(full_path)))
			{
				qtbase::Post2Task(kThreadUIHelper, std::bind(comp_cb, std::string(full_path)));
			}
			else
			{
				auto task = [=]()
				{
					if (IsValidImage(nbase::UTF8ToUTF16(full_path)))
					{
						comp_cb(std::string(full_path));
					}
					else
					{
						comp_cb("");
					}
				};

				qtbase::Post2DelayedTask(kThreadUIHelper, task, nbase::TimeDelta::FromSeconds(2));
			}
		}
		else
		{
			if (comp_cb)
			{
				qtbase::Post2Task(kThreadUIHelper, std::bind(comp_cb, std::string("")));
			}
			QLOG_WAR(L"async fetch file failed {0} reason code {1}") << url << response_code;
		}
	};

	auto progress_cb_ = [=](double d1, double d2, double d3, double d4)
	{
		if ((d1 > 0 || d2 > 0 || d3 > 0 || d4 > 0)
			&& prog_cb)
		{
			qtbase::Post2Task(kThreadUIHelper, std::bind(prog_cb, d1, d2, d3, d4));
		}

	};

	nbase::PathString path = nbase::UTF8ToUTF16(full_path);
	if (b_use_cache && IsValidImage(path))
	{
		complete_cb_(true, 200);
		return 1;
	}
	else
	{
		HttpRequest http_request(url, full_path, complete_cb_, progress_cb_);
		http_request.AddHeader("Accept", "*/*");

		return PostRequest(http_request);
	}
}

bool FileFetcher::IsValidImage(std::wstring file_path)
{
	if (nbase::FilePathIsExist(file_path, false))
	{
		QImage image;
		if (image.load(QString::fromStdWString(file_path))
			&& image.height() > 0 && image.width() > 0)
		{
			return IsCompletedImage(file_path);
		}
	}
	return false;
}

bool FileFetcher::IsCompletedImage(std::wstring file_path)
{
	std::string image_buffer;
	nbase::ReadFileToString(file_path, image_buffer);
	int image_size = image_buffer.size();
	//jpg png图是根据最前面和最后面特殊字节确定. bmp根据文件长度确定  
	//png检查  
	if (image_size >= MIN_IMAGE_SIZE && (unsigned char)image_buffer[0] == 137 && image_buffer[1] == 80 
		&& image_buffer[2] == 78 && image_buffer[3] == 71 && image_buffer[4] == 13
		&& image_buffer[5] == 10 && image_buffer[6] == 26 && image_buffer[7] == 10)
	{
				
		if (image_buffer[image_size - 5] == 68 && (unsigned char)image_buffer[image_size - 4] == 174 && image_buffer[image_size - 3] == 66
			&& image_buffer[image_size - 2] == 96 && (unsigned char)image_buffer[image_size - 1] == 130)
		{
			return true;
		}

		//有些情况最后多了些没用的字节  
		for (int i = image_size - 1; i > image_size / 2; --i)
		{
			if (image_buffer[i - 5] == 68 && (unsigned char)image_buffer[i - 4] == 174 && image_buffer[i - 3] == 66
				&& image_buffer[i - 2] == 96 && (unsigned char)image_buffer[i - 1] == 130)
			{
				return true;
			}
		}
	}
	else if (image_size >= MIN_IMAGE_SIZE && image_buffer[0] == 66 && image_buffer[1] == 77)//bmp  
	{
		//bmp长度  
		//整数转成字符串拼接 
		int len = 0;
		for (int i = 5; i >= 2; i--)
		{
			len = len * 16 + image_buffer[i];
		}
			
		if (len <= image_size) //有些图比实际要长  
		{
			return true;
		}
	}
	else if (image_size >= MIN_IMAGE_SIZE && (unsigned char)image_buffer[0] == 255 
		&& (unsigned char)image_buffer[1] == 216) //jpg  
	{
		//标准jpeg最后出现ff d9  
		if ((unsigned char)image_buffer[image_size - 2] == 255 
			&& (unsigned char)image_buffer[image_size - 1] == 217)
		{
			return true;
		}
		else
		{
			//有好多jpg最后被人为补了些字符也能打得开, 算作完整jpg, ffd9出现在近末端  
			//jpeg开始几个是特殊字节, 所以最后大于10就行了 从最后字符遍历  
			//有些文件会出现两个ffd9 后半部分ffd9才行  
			for (int i = image_size - 2; i > image_size / 2; --i)
			{
				//检查有没有ffd9连在一起的  
				if ((unsigned char)image_buffer[i] == 255 
					&& (unsigned char)image_buffer[i + 1] == 217)
				{
					return true;
				}
			}
		}
	}
		
	return false;
}