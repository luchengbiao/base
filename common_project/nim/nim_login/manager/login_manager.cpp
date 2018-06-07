#include "login_manager.h"
#include "base/util/string_util.h"
#include "base/file/file_util.h"
#include "nim_base/user.h"
#include "log_manager/log.h"
#include "qthread_manager/closure.h"
#include "nim_sdk_manager/api/nim_cpp_client.h"
#include "callback/login_callback.h"

namespace nim_comp
{

void LoginManager::InitUserFolder()
{
	std::string account = LoginManager::GetInstance()->GetAccount();
	nbase::CreateDirectory(GetUserDataPath(account));
	nbase::CreateDirectory(GetUserImagePath(account));
	nbase::CreateDirectory(GetUserAudioPath(account));
}

LoginManager::LoginManager()
{
	account_ = "";
	password_ = "";
	app_key_ = "";
	status_ = LoginStatus_NONE;
	active_ = true;
}

bool LoginManager::IsSelf( const std::string &account )
{
	return (account == account_);
}

void LoginManager::SetAccount( const std::string &account )
{
	account_ = account;
	nbase::LowerString(account_);;
}

std::string LoginManager::GetAccount()
{
	return account_;
}

bool LoginManager::IsEqual(const std::string& account)
{
	std::string new_account = account;
	nbase::LowerString(new_account);
	return account_ == new_account;
}

void LoginManager::SetPassword( const std::string &password )
{
	password_ = password;
}

std::string LoginManager::GetPassword()
{
	return password_;
}

void LoginManager::SetLoginStatus( LoginStatus status )
{
	status_ = status;
}

LoginStatus LoginManager::GetLoginStatus()
{
	return status_;
}

void LoginManager::SetLinkActive( bool active )
{
	active_ = active;
}

bool LoginManager::IsLinkActive()
{
	return active_;
}

void LoginManager::SetAPPKey(std::string appKey)
{
	app_key_ = appKey;
}

std::string LoginManager::GetAPPKey()
{
	return app_key_;
}

void LoginManager::DoLogin(std::string user, std::string pass)
{
	assert(LoginManager::GetInstance()->GetLoginStatus() == LoginStatus_NONE);
	LoginManager::GetInstance()->SetLoginStatus(LoginStatus_LOGIN);
	LoginManager::GetInstance()->SetAccount(user);
	std::string pass_md5 = pass;
	LoginManager::GetInstance()->SetPassword(pass_md5);
	InitUserFolder();
	{
		int ver = 0;
		std::wstring vf;
		QLOG_APP(L"App Version {0}") << ver;
		QLOG_APP(L"Account {0}") << LoginManager::GetInstance()->GetAccount();
		QLOG_APP(L"UI ThreadId {0}") << GetCurrentThreadId();
		QLOG_APP(L"-----login begin-----");
	}

	std::string app_key = LoginManager::GetInstance()->GetAPPKey();

	auto cb = std::bind(LoginCallback::OnLoginCallback, std::placeholders::_1, nullptr);
	nim::Client::Login(app_key, LoginManager::GetInstance()->GetAccount(), LoginManager::GetInstance()->GetPassword(), cb);
}

void LoginManager::DoLogout(bool over, nim::NIMLogoutType type)
{
	QLOG_APP(L"DoLogout: {0} {1}") << over << type;
	LoginStatus status = LoginManager::GetInstance()->GetLoginStatus();
	if (status == LoginStatus_EXIT) {
		return;
	}
	LoginManager::GetInstance()->SetLoginStatus(LoginStatus_EXIT);

	if (status == LoginStatus_NONE) {
		OnLogoutCallback(Logout_SUCCESS);
		return;
	}

	if (over) {
		OnLogoutCallback(Logout_SUCCESS);
	} else {
		nim::Client::Logout(type, &LoginCallback::OnLogoutCallback);
	}
}

void LoginManager::OnLoginCallback(int code)
{
	for (auto& it : login_cb_list_)
	{
		(*(it.second))(code);
	}
}

void LoginManager::OnLogoutCallback(int code)
{
	for (auto& it : logout_cb_list_)
	{
		(*(it.second))(code);
	}
}

UnregisterCallback LoginManager::RegLoginCb(const NimLoginCallback& callback)
{
	NimLoginCallback* new_callback = new NimLoginCallback(callback);
	int cb_id = (int)new_callback;
	login_cb_list_[cb_id].reset(new_callback);

	auto cb = ToWeakCallback([this, cb_id]() {
		login_cb_list_.erase(cb_id);
	});
	return cb;
}

UnregisterCallback LoginManager::RegLogoutCb(const NimLogoutCallback& callback)
{
	NimLogoutCallback* new_callback = new NimLogoutCallback(callback);
	int cb_id = (int)new_callback;
	logout_cb_list_[cb_id].reset(new_callback);

	auto cb = ToWeakCallback([this, cb_id]() {
		logout_cb_list_.erase(cb_id);
	});
	return cb;
}

}