#include "log_upload_uitl.h"
#include <thread>
#include "base\file\file_util.h"
#include "base\util\string_number_conversions.h"
#include "base\win32\platform_string_util.h"
#include "common\system\system_tool.h"
#include "http_request\api_caller.h"
#include "http_request\api_result.h"
#include "qiniu_upload\qiniu_multi_upload.h"
#include "..\base\log_singleton_interface.h"
#include "..\singleton\log_common.h"

namespace log_upload_util
{
	static std::string s_api_upload_log;
	static std::string s_api_get_upload_token;

	void SetUploadApi(const std::string& api_upload_log, const std::string& api_get_upload_token)
	{
		s_api_upload_log = api_upload_log;
		s_api_get_upload_token = api_get_upload_token;
	}

	void UploadLogsInSingleOneFile(const std::vector<LogSingletonInterface*>& vec_log_singleton, 
								   const std::string& biz_id,
								   const std::string& content,
								   const std::wstring& temp_upload_file_path,
								   const UploadedCallback& uploaded_cb)
	{
		std::string log_str;

		for (auto log_singleton : vec_log_singleton)
		{
			if (log_singleton)
			{
				log_singleton->GetLogContent(log_str);
			}
		}

		nbase::WriteFile(temp_upload_file_path, log_str);

		auto finish_cb = [=](bool ret, const std::string& urlOrErrInfo)
		{
			DBG_WRAPPER(LOG_DBG(L"Upload File Result: {0}, {1}") << ret << urlOrErrInfo);

			if (ret)
			{
				auto cb = [=](ApiResult res)
				{
					if (res.Success())
					{
						nbase::DeleteFile(temp_upload_file_path);

						if (uploaded_cb)
						{
							uploaded_cb(true, "");
						}
					}
					else
					{
						if (uploaded_cb)
						{
							uploaded_cb(false, s_api_upload_log + " failed");
						}
					}
				};

				SS_MAP param =
				{
					{ "bizId",		biz_id			},
					{ "content",	content			},
					{ "type",		"PC_EVENT"		},
					{ "logUrls",	urlOrErrInfo	},
					{ "source",		"pc"			},
					{ "courseId",	"-1"			},
				};

				ApiCaller().AsyncCallPost(s_api_upload_log, param, cb);
			}
			else
			{
				if (uploaded_cb)
				{
					uploaded_cb(false, urlOrErrInfo);
				}
			}
		};

		if (nbase::FilePathIsExist(temp_upload_file_path, false))
		{
			UploadFile(temp_upload_file_path, finish_cb);
		}
	}

	bool UploadFile(const std::wstring& file_path, const UploadedCallback& uploaded_cb)
	{
		std::wstring name;
		nbase::FilePathApartFileName(file_path, name);
		std::string upload_name = std::move(nbase::UTF16ToUTF8(name));
		if (upload_name.empty())
		{
			upload_name = std::move(nbase::UTF16ToUTF8(file_path));
		}

		std::string mbcsPath;
		nbase::win32::UnicodeToMBCS(file_path, mbcsPath);

		auto cb = [=](ApiResult api_result)
		{
			if (api_result.Success())
			{
				std::string token = api_result.GetDataString("token");
				std::string base = api_result.GetDataString("baseUrl");

				std::thread task([=]
				{
					QiniuMultiUpload upload_note;
					QiNiuUploadInfo qiniu;

					qiniu.base_url_ = base;
					qiniu.token_ = token;
					qiniu.file_path_ = file_path;
					qiniu.file_name_ = name;

					upload_note.UploadFile(qiniu, uploaded_cb);
				});

				task.detach();
			}
			else
			{
				if (uploaded_cb)
				{
					uploaded_cb(false, "get upload token failed!");
				}
			}

		};

		//从服务器获取token
		SS_MAP param =
		{
			{ "fileName", upload_name }
		};

		ApiCaller().AsyncCallGet(s_api_get_upload_token, param, cb, true);

		return true;
	}

	void UploadCustomLog(const std::map<std::string, std::string>& custom_log)
	{
		ApiCaller().AsyncCallPost(s_api_upload_log, const_cast<SS_MAP&>(custom_log), nullptr);
	}
}