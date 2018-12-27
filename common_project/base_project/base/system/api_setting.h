#pragma once
#include <string>
#include <map>

class ApiSetting
{
public:
	static std::string api_host_;
	static int api_port_;
	static std::string api_context_;
	static std::string api_sign_key_;
	static std::string api_server_version_;
	static std::string app_version_;
	static std::string app_inner_version_;
	static std::string api_cookie_;
	static std::string api_token_;
	static std::string api_auth_;
	static std::string api_socket_;
	static std::string register_dir_;
	static std::string url_head_;

	static std::string GetOtherApiHost(std::string key);
	static void SetOtherApiHost(std::string key, std::string val);

	static std::string GetAppKey(std::string key);
	static void SetAppKey(std::string key, std::string val);


	static void Load();

private:
	static std::map<std::string, std::string> map_other_host_;
	static std::map<std::string, std::string> map_app_key_;
};

