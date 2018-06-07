#pragma once
#include "base\memory\singleton.h"
#include "base\callback\callback.h"
#include "base\util\auto_unregister.h"
#include <map>
#include <memory>
#include <functional>

typedef std::function<void(uint32_t,int ecode)>	AgoraLoginCallback;	/**< 信令登录回调 */
typedef std::function<void(int ecode)>	AgoraLogoutCallback;	/**< 信令登出回调 */

class AGLoginManager : public nbase::SupportWeakCallback
{
public:
	SINGLETON_DEFINE(AGLoginManager);
	AGLoginManager();
	~AGLoginManager();

	// reg cb
	UnregisterCallback RegLoginCb(const AgoraLoginCallback& callback);
	UnregisterCallback RegLogoutCb(const AgoraLogoutCallback& callback);

	// cb
	void OnloginSuccessCallback(uint32_t uid, int fd);
	void OnloginFailCallback(int ecode);
	void OnlogoutCallback(int ecode);

	// 
	void Login(std::string s_vendorID, std::string s_account, std::string s_token, uint32_t uid, std::string s_deviceID);
	void Login(std::string s_vendorID, std::string s_account, std::string s_token, uint32_t uid, std::string s_deviceID, int retry_time_in_s, int retry_count);
	void Logout();
	void Start();
	void Stop();
	bool IsOnline();
	int GetStatus();
	void SetAppId(std::string app_id);
	std::string GetAppId();
	uint32_t GetUid();

private:
	uint32_t uid_;
	std::string app_id_;
	std::map<int, std::unique_ptr<AgoraLoginCallback>> login_cb_list_;
	std::map<int, std::unique_ptr<AgoraLogoutCallback>> logout_cb_list_;
};
