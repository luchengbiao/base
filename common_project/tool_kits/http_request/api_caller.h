#pragma once
#include <functional>
#include <map>
#include <string>
#include <stdint.h>
#include "api_result.h"

typedef std::function<void(ApiResult)> ASYNSC_API_CALLBACK;
typedef std::map<std::string, std::string> SS_MAP;
typedef std::function<void(double)> UploadFileProgressCallback;

/*
*访问API接口
* by Darren
*/

class ApiCaller
{
public:
	ApiCaller();
	~ApiCaller();

	static void GlobalInit();
	static void GlobalCleanup();
	static bool b_global_init;
	
	int	AsyncCallGet(const std::string &path, SS_MAP &param_map_, const ASYNSC_API_CALLBACK& response_cb, bool need_url_encode = false);
	int	AsyncCallPost(const std::string &path, SS_MAP &param_map_, const ASYNSC_API_CALLBACK& response_cb, bool need_url_encode = false);
	int	AsyncCallPut(const std::string &path, SS_MAP &param_map_, const ASYNSC_API_CALLBACK& response_cb, bool need_url_encode = false);
	int	AsyncCallDelete(const std::string &path, SS_MAP &param_map_, const ASYNSC_API_CALLBACK& response_cb, bool need_url_encode = false);
	std::string CallPut(std::string &path, std::map< std::string, std::string> &param_map, ApiResult& api_result);
	std::string assemble_post_url_(std::string path, std::map<std::string, std::string> &param_map_);
	std::string build_user_agent_();
	void UploadFileToServer(const std::string &url_path, const std::wstring &file_path, const std::wstring &file_name, 
		const ASYNSC_API_CALLBACK& comp_cb, const UploadFileProgressCallback& prog_cb);
	std::string GetServiceAgreement(std::string path);

public:
	static std::string CalMd5ValidSign(std::map<std::string, std::string> &param_map_);

private:
	
	std::string builde_cookie_();
	std::string builde_auth_();
	std::string build_token_();
	std::string assemble_general_url_(std::string path, std::map<std::string, std::string> &param_map_, bool need_sep = false, bool need_url_encode = false);
	std::string cal_general_sign_(std::string path, std::map<std::string, std::string> &param_map_,bool need_sep = false);  //need_sep 签名字段需要进行分割

	
	std::string generate_post_field_(std::map<std::string, std::string> &param_map_);
	static size_t ApiCaller::DefaultApiDataCallback(void* pBuffer, size_t nSize, size_t nMemByte, void* pParam);

private:
	std::string		s_response_;
	std::string		s_path_;
};


