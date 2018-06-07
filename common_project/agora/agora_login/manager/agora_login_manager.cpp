#include "agora_login_manager.h"
#include "..\agora_signalling_sdk\manager\signalling_manager.h"

AGLoginManager::AGLoginManager()
{
	uid_ = 0;
	app_id_ = "";
}

AGLoginManager::~AGLoginManager()
{

}

UnregisterCallback AGLoginManager::RegLoginCb(const AgoraLoginCallback& callback)
{
	AgoraLoginCallback* new_callback = new AgoraLoginCallback(callback);
	int cb_id = (int)new_callback;
	login_cb_list_[cb_id].reset(new_callback);

	auto cb = ToWeakCallback([this, cb_id]() {
		login_cb_list_.erase(cb_id);
	});
	return cb;
}

UnregisterCallback AGLoginManager::RegLogoutCb(const AgoraLogoutCallback& callback)
{
	AgoraLogoutCallback* new_callback = new AgoraLogoutCallback(callback);
	int cb_id = (int)new_callback;
	logout_cb_list_[cb_id].reset(new_callback);

	auto cb = ToWeakCallback([this, cb_id]() {
		logout_cb_list_.erase(cb_id);
	});
	return cb;
}

void AGLoginManager::OnloginSuccessCallback(uint32_t uid, int fd)
{
	uid_ = uid;

	for (auto& it : login_cb_list_)
	{
		(*(it.second))(uid,0);
	}
}

void AGLoginManager::OnloginFailCallback(int ecode)
{
	for (auto& it : login_cb_list_)
	{
		(*(it.second))(uid_,ecode);
	}
}

void AGLoginManager::OnlogoutCallback(int ecode)
{
	for (auto& it : logout_cb_list_)
	{
		(*(it.second))(ecode);
	}
}

void AGLoginManager::Login(std::string s_vendorID, std::string s_account, std::string s_token, uint32_t uid, std::string s_deviceID)
{
	AGSignallingManager::GetAGSignalling(app_id_)->Login(s_vendorID, s_account, s_token, uid, s_deviceID);
}

void AGLoginManager::Login(std::string s_vendorID, std::string s_account, std::string s_token, uint32_t uid, std::string s_deviceID, int retry_time_in_s, int retry_count)
{
	AGSignallingManager::GetAGSignalling(app_id_)->Login(s_vendorID, s_account, s_token, uid, s_deviceID, retry_time_in_s, retry_count);
}

void AGLoginManager::Logout()
{
	AGSignallingManager::GetAGSignalling(app_id_)->Logout();
}

void AGLoginManager::Start()
{
	AGSignallingManager::GetAGSignalling(app_id_)->Start();
}

void AGLoginManager::Stop()
{
	AGSignallingManager::GetAGSignalling(app_id_)->Stop();
}

bool AGLoginManager::IsOnline()
{
	return AGSignallingManager::GetAGSignalling(app_id_)->IsOnline();
}

int AGLoginManager::GetStatus()
{
	return AGSignallingManager::GetAGSignalling(app_id_)->GetStatus();
}

void AGLoginManager::SetAppId(std::string app_id)
{
	app_id_ = app_id;
}

std::string AGLoginManager::GetAppId()
{
	return app_id_;
}

uint32_t AGLoginManager::GetUid()
{
	return uid_;
}

