#include "api_setting.h"

std::string ApiSetting::api_host_ = "";

int ApiSetting::api_port_ = 0;

std::string ApiSetting::api_context_ = "";

std::string ApiSetting::api_sign_key_ = "";

std::string ApiSetting::api_server_version_ = "";

std::string ApiSetting::app_version_ = "";

std::string ApiSetting::app_inner_version_ = "";

std::string ApiSetting::api_cookie_ = "";

std::string ApiSetting::api_auth_ = "";

std::string ApiSetting::api_token_ = "";

std::string ApiSetting::api_socket_ = "";

std::string ApiSetting::register_dir_ = "";

std::string ApiSetting::url_head_ = "";

std::map<std::string, std::string> ApiSetting::map_other_host_;

std::map<std::string, std::string> ApiSetting::map_app_key_;

std::string ApiSetting::GetOtherApiHost(std::string key)
{
	auto iter = map_other_host_.find(key);
	if (iter != map_other_host_.end())
	{
		return iter->second;
	}
	return "";
}

void ApiSetting::SetOtherApiHost(std::string key, std::string val)
{
	map_other_host_[key] = val;
}

std::string ApiSetting::GetAppKey(std::string key)
{
	auto iter = map_app_key_.find(key);
	if (iter != map_app_key_.end())
	{
		return iter->second;
	}
	return "";
}

void ApiSetting::SetAppKey(std::string key, std::string val)
{
	map_app_key_[key] = val;
}
