#include "api_caller.h"
#include "base\encrypt\encrypt.h"
#include <thread>
#include <vector>
#include "base\util\string_util.h"
#include <algorithm>
#include "base\util\string_number_conversions.h"
#include "encrypt.h"
#include "url_util.h"
#include "HttpsClient.h"
#include "curl\curl.h"
#include "api_result.h"
#include "log\log.h"
#include "qthread_manager\closure.h"
#include "base\system\api_setting.h"
#include "..\common\system\system_tool.h"
#include "base\file\file_util.h"
#include "nim_sdk_manager\api\nim_tools_http_cpp.h"

using namespace std;
#define SERVER_SUPPORT
bool ApiCaller::b_global_init = false;

ApiCaller::ApiCaller()
{
	s_response_ = "";
	s_path_ = "";
}

ApiCaller::~ApiCaller()
{

}


int ApiCaller::AsyncCallGet(const std::string &path, SS_MAP &param_map_, const ASYNSC_API_CALLBACK& response_cb, bool need_url_encode /*= false*/)
{
	for (auto i = param_map_.begin(); need_url_encode && i != param_map_.end(); i++)
	{
		std::string& v = i->second;
		v = urlencode(v);
	}
	std::string url = "";
	if (path.find("http://") != std::string::npos 
		|| path.find("https://") != std::string::npos)
	{
		url = path;
		// 添加参数
		std::map< std::string, std::string>::iterator iter_ = param_map_.begin();
		while (iter_ != param_map_.end())
		{
			iter_ == param_map_.begin() ? url.append("?") : url.append("&");
			url.append(iter_->first).append("=").append(iter_->second);
			iter_++;
		}
	}
	else
	{
		url = assemble_general_url_(path, param_map_);
	}

	auto  res_cb = [=](const CHttpRequestPtr & http_request)
	{
		ApiResult api_result;
		if (!http_request || http_request->m_response.empty())
		{
			api_result.SetCode(-1);
			api_result.SetMsg(L"连接服务器失败");
		}
		else
		{
			ApiResult::Parse(http_request->m_response, api_result);
			if (api_result.Success())
			{
				if (url.find("/demand/list") == string::npos)
				{
					LOG_MSG(L"aysnc get api {0} result ok") << url.c_str();
				}

			}
			else
			{
				LOG_MSG(L"aysnc get api {0} result {1}") << url.c_str() << http_request->m_response.c_str();
			}
		}

		if (response_cb)
		{
			response_cb(api_result);  // res_cb And response_cb are definitely called in HttpsClient::CallbackThreadPool
			//qtbase::Post2Task(kThreadUIHelper, nbase::Bind(response_cb, api_result));
		}
	};

	curl_slist* sl_headers = NULL;
	sl_headers = curl_slist_append(sl_headers, "Accept: */*");
#ifdef SERVER_SUPPORT
	sl_headers = curl_slist_append(sl_headers, builde_auth_().c_str());
	sl_headers = curl_slist_append(sl_headers, build_token_().c_str());
#endif // SERVER_SUPPORT

#ifndef SERVER_SUPPORT
	sl_headers = curl_slist_append(sl_headers, builde_cookie_().c_str());
	sl_headers = curl_slist_append(sl_headers, builde_auth_().c_str());
#endif // SERVER_SUPPORT
	std::string ts_str = "xy-nonce: " + param_map_["ts"];
	sl_headers = curl_slist_append(sl_headers, ts_str.c_str());
	std::string sign_str = "xy-sign: " + param_map_["sign"];
	sl_headers = curl_slist_append(sl_headers, sign_str.c_str());
	sl_headers = curl_slist_append(sl_headers, "xy-access-key: jyxb");

	int status = CHttpClient::GetInstance()->AsyncGet(url, sl_headers, res_cb);

	return status;
}

int ApiCaller::AsyncCallPut(const std::string &path, SS_MAP &param_map_, const ASYNSC_API_CALLBACK& response_cb, bool need_url_encode /*= false*/)
{
	for (auto i = param_map_.begin(); need_url_encode && i != param_map_.end(); i++)
	{
		std::string& v = i->second;
		v = urlencode(v);
	}
	std::string url = "";
	if (path.find("http://") != std::string::npos
		|| path.find("https://") != std::string::npos)
	{
		url = path;
	}
	else
	{
		url = assemble_general_url_(path, param_map_);
	}

	auto  res_cb = [=](const CHttpRequestPtr & http_request)
	{
		ApiResult api_result;
		if (!http_request || http_request->m_response.empty())
		{
			api_result.SetCode(-1);
			api_result.SetMsg(L"连接服务器失败");
		}
		else
		{
			ApiResult::Parse(http_request->m_response, api_result);
			if (api_result.Success())
			{
				if (url.find("/demand/list") == string::npos)
				{
					LOG_MSG(L"aysnc get api {0} result ok") << url.c_str();
				}

			}
			else
			{
				LOG_MSG(L"aysnc get api {0} result {1}") << url.c_str() << http_request->m_response.c_str();
			}
		}

		if (response_cb)
		{
			response_cb(api_result);  // res_cb And response_cb are definitely called in HttpsClient::CallbackThreadPool
			//qtbase::Post2Task(kThreadUIHelper, nbase::Bind(response_cb, api_result));
		}
	};

	curl_slist* sl_headers = NULL;
	sl_headers = curl_slist_append(sl_headers, "Accept: */*");
#ifdef SERVER_SUPPORT
	sl_headers = curl_slist_append(sl_headers, builde_auth_().c_str());
	sl_headers = curl_slist_append(sl_headers, build_token_().c_str());
#endif // SERVER_SUPPORT

#ifndef SERVER_SUPPORT
	sl_headers = curl_slist_append(sl_headers, builde_cookie_().c_str());
	sl_headers = curl_slist_append(sl_headers, builde_auth_().c_str());
#endif // SERVER_SUPPORT
	std::string ts_str = "xy-nonce: " + param_map_["ts"];
	sl_headers = curl_slist_append(sl_headers, ts_str.c_str());
	std::string sign_str = "xy-sign: " + param_map_["sign"];
	sl_headers = curl_slist_append(sl_headers, sign_str.c_str());
	sl_headers = curl_slist_append(sl_headers, "xy-access-key: jyxb");

	int status = CHttpClient::GetInstance()->AsyncPut(url, sl_headers, res_cb);

	return status;
}

int ApiCaller::AsyncCallDelete(const std::string &path, SS_MAP &param_map_, const ASYNSC_API_CALLBACK& response_cb, bool need_url_encode /*= false*/)
{
	for (auto i = param_map_.begin(); need_url_encode && i != param_map_.end(); i++)
	{
		std::string& v = i->second;
		v = urlencode(v);
	}
	std::string url = "";
	if (path.find("http://") != std::string::npos
		|| path.find("https://") != std::string::npos)
	{
		url = path;
	}
	else
	{
		url = assemble_general_url_(path, param_map_);
	}

	auto  res_cb = [=](const CHttpRequestPtr & http_request)
	{
		ApiResult api_result;
		if (!http_request || http_request->m_response.empty())
		{
			api_result.SetCode(-1);
			api_result.SetMsg(L"连接服务器失败");
		}
		else
		{
			ApiResult::Parse(http_request->m_response, api_result);
			if (api_result.Success())
			{
				if (url.find("/demand/list") == string::npos)
				{
					LOG_MSG(L"aysnc get api {0} result ok") << url.c_str();
				}

			}
			else
			{
				LOG_MSG(L"aysnc get api {0} result {1}") << url.c_str() << http_request->m_response.c_str();
			}
		}

		if (response_cb)
		{
			response_cb(api_result);  // res_cb And response_cb are definitely called in HttpsClient::CallbackThreadPool
			//qtbase::Post2Task(kThreadUIHelper, nbase::Bind(response_cb, api_result));
		}
	};

	curl_slist* sl_headers = NULL;
	sl_headers = curl_slist_append(sl_headers, "Accept: */*");
#ifdef SERVER_SUPPORT
	sl_headers = curl_slist_append(sl_headers, builde_auth_().c_str());
	sl_headers = curl_slist_append(sl_headers, build_token_().c_str());
#endif // SERVER_SUPPORT

#ifndef SERVER_SUPPORT
	sl_headers = curl_slist_append(sl_headers, builde_cookie_().c_str());
	sl_headers = curl_slist_append(sl_headers, builde_auth_().c_str());
#endif // SERVER_SUPPORT
	std::string ts_str = "xy-nonce: " + param_map_["ts"];
	sl_headers = curl_slist_append(sl_headers, ts_str.c_str());
	std::string sign_str = "xy-sign: " + param_map_["sign"];
	sl_headers = curl_slist_append(sl_headers, sign_str.c_str());
	sl_headers = curl_slist_append(sl_headers, "xy-access-key: jyxb");

	int status = CHttpClient::GetInstance()->AsyncDelete(url, sl_headers, res_cb);

	return status;
}

int ApiCaller::AsyncCallPost(const std::string &path, SS_MAP &param_map_, const ASYNSC_API_CALLBACK& response_cb, bool need_url_encode /*= false*/)
{
	for (auto i = param_map_.begin(); need_url_encode && i != param_map_.end(); i++)
	{
		string& v = i->second;
		v = urlencode(v);
	}

	std::string post_f_ = generate_post_field_(param_map_);

	SS_MAP field_map_;
	field_map_["_post_field_"] = post_f_;

	std::string url = "";
	if (path.find("http://") != std::string::npos
		|| path.find("https://") != std::string::npos)
	{
		url = path;
	}
	else
	{
		url = assemble_post_url_(path, field_map_);
	}

	auto  res_cb = [=](const CHttpRequestPtr & http_request)
	{
		ApiResult api_result;

#ifdef DEBUG
		LOG_MSG(nbase::UTF8ToUTF16(http_request->m_response));
#endif


		if (!http_request || http_request->m_response.empty())
		{
			api_result.SetCode(-1);
			api_result.SetMsg(L"连接服务器失败");
		}
		else
		{
			ApiResult::Parse(http_request->m_response, api_result);
			if (api_result.Success())
			{
				LOG_MSG(L"aysnc post api {0} result ok") << url.c_str();
				std::wstring msg = nbase::UTF8ToUTF16(api_result.GetMsg());
			}
			else
			{
				LOG_MSG(L"aysnc post api {0} result {1}") << url.c_str() << http_request->m_response.c_str();
				std::wstring msg = nbase::UTF8ToUTF16(api_result.GetMsg());
			}
		}
		if (response_cb)
		{
			response_cb(api_result);  // res_cb And response_cb are definitely called in HttpsClient::CallbackThreadPool
			//qtbase::Post2Task(kThreadUIHelper, nbase::Bind(response_cb, api_result));
		}
	};


	curl_slist* sl_headers = NULL;
	sl_headers = curl_slist_append(sl_headers, "Accept: */*");
#ifdef SERVER_SUPPORT
	sl_headers = curl_slist_append(sl_headers, builde_auth_().c_str());
	sl_headers = curl_slist_append(sl_headers, build_token_().c_str());
#endif // SERVER_SUPPORT

#ifndef SERVER_SUPPORT
	sl_headers = curl_slist_append(sl_headers, builde_cookie_().c_str());
	sl_headers = curl_slist_append(sl_headers, builde_auth_().c_str());
#endif // SERVER_SUPPORT


	sl_headers = curl_slist_append(sl_headers, "Expect:");
	std::string ts_str = "xy-nonce: " + param_map_["ts"];
	sl_headers = curl_slist_append(sl_headers, ts_str.c_str());
	std::string sign_str = "xy-sign: " + param_map_["sign"];
	sl_headers = curl_slist_append(sl_headers, sign_str.c_str());
	sl_headers = curl_slist_append(sl_headers, "xy-access-key: jyxb");

	int64_t status = CHttpClient::GetInstance()->AsyncPost(url, post_f_, sl_headers, res_cb);

	return (int)status;
}


void ApiCaller::GlobalInit()
{
	
	
}

void ApiCaller::GlobalCleanup()
{
	
}

std::string ApiCaller::builde_cookie_()
{
	std::string cookie_str("Cookie: ");

	cookie_str.append("auth=" + ApiSetting::api_cookie_);

	return cookie_str;
}

std::string ApiCaller::builde_auth_()
{
	std::string auth_str("auth: ");

	auth_str.append(ApiSetting::api_auth_);

	return auth_str;
}


std::string ApiCaller::build_token_()
{
	std::string token_str("token: ");

	token_str.append(ApiSetting::api_token_);

	return token_str;
}

std::string ApiCaller::cal_general_sign_(std::string path, std::map<std::string, std::string> &param_map, bool need_sep)
{
	std::vector<std::string> vec_key;
	
	//添加时间戳
	time_t ts_ = time(NULL);
	
	srand(ts_);

	int r = rand() % 999999;

	std::string ts_s_ = nbase::StringPrintf("%06d%d", r, ts_).append(systembase::GetMac());
	param_map.insert(std::make_pair("ts", ts_s_));

	//添加key path
	vec_key.push_back(ApiSetting::api_sign_key_);
	vec_key.push_back(path);

	auto iter_ = param_map.begin();
	while (iter_ != param_map.end())
	{
		if (need_sep)
		{
			std::string key = iter_->first;
			std::string content = iter_->second;
			std::string key_str = "&" + key + "=";
			int n = content.find_first_of(key_str);
			if (n > 0)
			{
				//多个值需要进行分割
				nbase::StringSplit(content, key_str, vec_key);
			}
			else
			{
				vec_key.push_back(iter_->second);
			}
		}
		else
		{
			vec_key.push_back(iter_->second);
		}

		iter_++;
	}

	std::sort(vec_key.begin(), vec_key.end());

	std::string str_("");
	for (int i = 0; i < vec_key.size(); i++)
	{
		str_.append(vec_key[i]);
	}

	std::string cal_md5_;
	CalMd5(str_, cal_md5_);
	param_map.insert(std::make_pair("sign", cal_md5_));

	return cal_md5_;
}

std::string ApiCaller::assemble_post_url_(std::string path, std::map<std::string, std::string>& param_map)
{
	s_path_ = path;
	//先计算签名
	cal_general_sign_(path, param_map);

	std::string url_str(ApiSetting::api_host_);
	if (ApiSetting::api_port_ != 80)
	{
		url_str.append(":").append(nbase::IntToString(ApiSetting::api_port_));
		
	}
	url_str.append(ApiSetting::api_context_).append(path);

	url_str.append("?ts=").append(param_map["ts"]);
	url_str.append("&sign=").append(param_map["sign"]);
	
	return url_str;
}

std::string ApiCaller::assemble_general_url_(std::string path, std::map< std::string, std::string>& param_map, bool need_sep, bool need_url_encode)
{
	s_path_ = path;
	//先计算签名
	cal_general_sign_(path, param_map, need_sep);

	for (auto i = param_map.begin(); need_url_encode && i != param_map.end(); i++)
	{
		std::string& v = i->second;
		v = urlencode(v);
	}

	std::string url_str(ApiSetting::api_host_);
	if (ApiSetting::api_port_ != 80)
	{
		url_str.append(":").append(nbase::IntToString(ApiSetting::api_port_));

	}
	url_str.append(ApiSetting::api_context_).append(path);

	if (!param_map.empty())
	{

		std::map< std::string, std::string>::iterator iter_ = param_map.begin();

		while (iter_ != param_map.end())
		{
			iter_ == param_map.begin() ? url_str.append("?") : url_str.append("&");
			url_str.append(iter_->first).append("=").append(iter_->second);
			iter_++;
		}
	}
	return url_str;

}

std::string ApiCaller::generate_post_field_(std::map< std::string, std::string> &param_map_)
{
	std::string post_f_("");
	if (!param_map_.empty())
	{
		std::map< std::string, std::string>::iterator iter_ = param_map_.begin();

		while (iter_ != param_map_.end())
		{
			if (iter_ != param_map_.begin())
			{
				post_f_.append("&");
			}
			post_f_.append(iter_->first).append("=").append(iter_->second);
			iter_++;
		}
	}
	return post_f_;
}

std::string ApiCaller::CalMd5ValidSign(std::map<std::string, std::string> &param_map)
{
	std::vector<std::string> vec_key;

	vec_key.push_back(ApiSetting::api_sign_key_);
	auto iter_ = param_map.begin();
	while (iter_ != param_map.end())
	{
		vec_key.push_back(iter_->second);
		iter_++;
	}
	std::sort(vec_key.begin(), vec_key.end());

	std::string key_str("");
	for (int i = 0; i < vec_key.size(); i++)
	{
		key_str.append(vec_key[i]);
	}

	std::string cal_md5_;
	CalMd5(key_str, cal_md5_);
	return cal_md5_;
}

std::string ApiCaller::build_user_agent_()
{
	std::string ua_("");

	ua_.append("os/").append(systembase::GetOSVersionString());
	ua_.append(" da_version/").append(ApiSetting::app_inner_version_);
	ua_.append(" os_version/").append(nbase::IntToString(systembase::GetOSVersion()));
	ua_.append(" jiayouxueba/").append(ApiSetting::app_version_);
	ua_.append(" device/PCMAC_").append(systembase::GetMac());
	ua_.append(" api/").append(ApiSetting::api_server_version_);

	return ua_;
}

size_t put_read_callback(char *ptr, size_t size, size_t nmemb, void *stream)
{
	//char* data = (char*)stream;
	return 0;
}

size_t ApiCaller::DefaultApiDataCallback(void* pBuffer, size_t nSize, size_t nMemByte, void* pParam)
{
	assert(pParam);
	ApiCaller* p_api_ = (ApiCaller*)pParam;

	LOG_MSG(L"call api {0} result {1}") << p_api_->s_path_ << string((char*)(pBuffer));
	p_api_->s_response_.append((char*)pBuffer);

	return nSize * nMemByte;
}

std::string ApiCaller::CallPut(std::string &path, std::map< std::string, std::string> &param_map, ApiResult& api_result)
{
	CURLcode res;

	std::string s_ret_("");
	CURL* curl = curl_easy_init();
	if (NULL == curl) {
		return "";
	}
	curl_slist* sl_headers_ = NULL;
	if (curl)
	{
		std::string url = assemble_general_url_(path, param_map);
		std::string ua = build_user_agent_();

		LOG_MSG(L"call api {0}") << url;

		sl_headers_ = curl_slist_append(sl_headers_, "Accept: */*");
		sl_headers_ = curl_slist_append(sl_headers_, "Content-Type: text/plain");
#ifdef SERVER_SUPPORT
		sl_headers_ = curl_slist_append(sl_headers_, builde_auth_().c_str());
		sl_headers_ = curl_slist_append(sl_headers_, build_token_().c_str());
#endif // SERVER_SUPPORT

#ifndef SERVER_SUPPORT
		sl_headers_ = curl_slist_append(sl_headers_, builde_cookie_().c_str());
#endif // SERVER_SUPPORT
		string ts_str = "xy-nonce: " + param_map["ts"];
		sl_headers_ = curl_slist_append(sl_headers_, ts_str.c_str());
		string sign_str = "xy-sign: " + param_map["sign"];
		sl_headers_ = curl_slist_append(sl_headers_, sign_str.c_str());
		sl_headers_ = curl_slist_append(sl_headers_, "xy-access-key: jyxb");

		if (strncmp(url.c_str(), "http://", 7) == 0)
		{
			// 方法1, 设定为不验证证书和HOST  
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
		}
		else if (strncmp(url.c_str(), "https://", 8) == 0)
		{
			// 方法2, 设定一个SSL判别证书, 未测试  
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 1L);
			curl_easy_setopt(curl, CURLOPT_SSLCERTTYPE, "PEM");
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
			curl_easy_setopt(curl, CURLOPT_CAINFO, "all.pem");    //设置一个证书文件  
		}

		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, sl_headers_);// 改协议头
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30);
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10);
		curl_easy_setopt(curl, CURLOPT_USERAGENT, ua.c_str());
		curl_easy_setopt(curl, CURLOPT_PROXY, "");

		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

		curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
		curl_easy_setopt(curl, CURLOPT_READFUNCTION, put_read_callback);
		curl_easy_setopt(curl, CURLOPT_READDATA, "");
		curl_easy_setopt(curl, CURLOPT_INFILESIZE, 0);	//size为0时不会调用read callback

		curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "");


		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, DefaultApiDataCallback);

		curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);

		res = curl_easy_perform(curl);   // 执行

		if (res != 0) {
			s_ret_ = curl_easy_strerror(res);
			LOG_ERR(L"call api {0} error {1} {2}") << path << res << curl_easy_strerror(res);;

		}

		ApiResult::Parse(s_response_, api_result);
		if (!api_result.Success())
		{
			s_ret_ = api_result.GetMsg();
		}
	}
	else
	{
		s_ret_ = "网络初始化失败！";
	}

	if (sl_headers_)
	{
		curl_slist_free_all(sl_headers_);
	}
	return s_ret_;
}

void ApiCaller::UploadFileToServer(const std::string &url_path, const std::wstring &file_path, const std::wstring &file_name, 
	const ASYNSC_API_CALLBACK& comp_cb, const UploadFileProgressCallback& prog_cb)
{
	const static std::string BOUNDARY = "BOUNDARYfareoigj9032490aggsrths";

	std::string multipart_form_body = "";
	if (nbase::FilePathIsExist(file_path, false))
	{
		std::string file_content;
		nbase::ReadFileToString(file_path, file_content);
		multipart_form_body.append("--").append(BOUNDARY).append("\r\n")
			.append("Content-Disposition: form-data; name=\"upload_files\"; filename=\"").append(nbase::UTF16ToUTF8(file_name)).append("\"\r\n")
			.append("Content-Type: application/vnd.ms-powerpoint\r\n\r\n")
			.append(file_content).append("\r\n");
	}

	multipart_form_body.append("--").append(BOUNDARY).append("--\r\n");

	ApiCaller api_caller;
	std::string url = url_path;

	auto api_comp_cb = [=](bool b_success, int code, const std::string& s)
	{
		LOG_MSG(L"async upload server ppt {0}, {1}, {2}") << b_success << code << s;

		if (!b_success)
		{
			LOG_WAR(L"upload img failed {0}") << code;
			ApiResult api_result;
			api_result.SetCode(-1);
			api_result.SetMsg(L"连接服务器失败");
			if (comp_cb)
			{
				qtbase::Post2Task(kThreadUIHelper, nbase::Bind(comp_cb, api_result));
			}
			return;
		}

		Json::Value v;
		Json::Reader reader;
		reader.parse(s, v);
		ApiResult api_result(v);
		if (comp_cb)
		{
			qtbase::Post2Task(kThreadUIHelper, nbase::Bind(comp_cb, api_result));
		}
	};

	auto api_prog_cb = [=](double d1, double d2, double d3, double d4)
	{
		double percent = (d2 / d1 * 100);
		if (prog_cb)
		{
			qtbase::Post2Task(kThreadUIHelper, nbase::Bind(prog_cb, percent));
		}
	};

	nim_http::HttpRequest request(url, multipart_form_body.c_str(), multipart_form_body.size(), api_comp_cb, api_prog_cb);

	request.AddHeader("Content-Type", std::string("multipart/form-data; boundary=").append(BOUNDARY));
	std::string cookie = ApiSetting::api_cookie_;
	request.AddHeader("auth", cookie);
#ifdef SERVER_SUPPORT
	request.AddHeader("token", ApiSetting::api_token_);
#endif // SERVER_SUPPORT

	request.AddHeader("xy-access-key", "jyxb");
	request.SetMethodAsPost();
	request.SetTimeout(1000 * 60 * 30);

	PostRequest(request);
}

std::string ApiCaller::GetServiceAgreement(std::string path)
{
	std::string url_str(ApiSetting::api_host_);
	url_str.append(ApiSetting::api_context_).append(path);
	return url_str;
}
