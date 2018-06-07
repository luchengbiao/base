#include "login_base.h"
#include "base\memory\singleton.h"
#include "nim_sdk_manager\helper\nim_client_helper.h"

using NetEaseLoginCb = std::function<void(int, std::wstring)>;
using NetEaseLogoutCb = std::function<void(int,std::wstring)>;
using NetEaseLoginCodeCb = std::function<void(int)>;
class NetEaseLogin :public LoginBase
{
public:
	SINGLETON_DEFINE(NetEaseLogin);
	NetEaseLogin(){};
	~NetEaseLogin(){};
public:
	void Login(std::string app_key,std::string account, std::string pwd, NetEaseLoginCb cb);
	void Login(std::string app_key,std::string account, std::string pwd, NetEaseLoginCb in_cb, NetEaseLogoutCb out_cb);
	bool CheckStatus(std::string app_key,std::string account, std::string pwd,NetEaseLoginCb cb);	//Èç¹ûÎ´µÇÂ¼£¬³¢ÊÔµÇÂ¼

	void RegLogout(NetEaseLogoutCb cb);
	void RegLoginCb(NetEaseLoginCodeCb cb);

	bool IsOnline();
	void Logout(NetEaseLogoutCb cb,nim::NIMLogoutType type);
	void Logout();
	void LogoutCb(int code);

	void DoRegLogoutCb();
};