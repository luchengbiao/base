#include "..\header\login_agora.h"
#include <string>
#include "agora_login\agora_login_manager.h"
#include "base\util\string_number_conversions.h"

LoginAgora::LoginAgora()
{

}

LoginAgora::~LoginAgora()
{

}

void LoginAgora::Login(std::string app_id,std::string account, std::string pwd, AgoraLoginCb cb)
{
	AGLoginManager* ag_login_manager = AGLoginManager::GetInstance();
	if (ag_login_manager->IsOnline())
	{
		return;
	}
	auto task_cb = ToWeakCallback([=](uint32_t u_id,int code){
		std::wstring str = L"";
		if (code == SUCCESS)
		{

		}
		else if (code == LOGIN_E_NET)
		{
			str = L"您的网络出现问题，请检查网络";
		}
		else if (code == LOGIN_E_TOKEN_KICKED)
		{
			str = L"您已在其他地方登录";
		}
		else
		{
			std::wstring s_code = nbase::IntToString16(code);
			str = L"登录错误码："+s_code;
		}
		if (cb)
		{
			cb(code, str, u_id);
		}
	});


	ag_login_manager->RegLoginCb(task_cb);
	ag_login_manager->Login(app_id, account, pwd, 0, "");
}

void LoginAgora::Logout(AgoraLogoutCb cb)
{
	AGLoginManager* ag_login_manager = AGLoginManager::GetInstance();

	auto logout_cb = ToWeakCallback([=](int code){
		std::wstring msg = L"";
		if (code == LOGOUT_E_KICKED)
		{
			msg = L"您已在其他地方登录";
		}
		else if (code == SUCCESS)
		{
			msg = L"登出成功";
		}
		else if (code == LOGIN_E_NET)
		{
			msg = L"网络问题";
		}
		else
		{
			std::wstring s_code = nbase::IntToString16(code);
			msg = L"登出码" + s_code;
		}
		
	});
	ag_login_manager->RegLogoutCb(logout_cb);
	ag_login_manager->Logout();



}


void LoginAgora::Logout()
{
	AGLoginManager* ag_login_manager = AGLoginManager::GetInstance();
	ag_login_manager->Logout();
}

void LoginAgora::RegLogoutCb(AgoraLogoutCb cb)
{
	auto logout_cb = ToWeakCallback([=](int code){
		std::wstring msg = L"";
		if (code == LOGOUT_E_KICKED)
		{
			msg = L"您已在其他地方登录";
		}
		else if (code == SUCCESS)
		{
			msg = L"登出成功";
		}
		else if (code == LOGIN_E_NET)
		{
			msg = L"网络问题";
		}
		else
		{
			std::wstring s_code = nbase::IntToString16(code);
			msg = L"登出码" + s_code;
		}
		if (cb)
		{
			cb(code, msg);
		}
	});
	AGLoginManager::GetInstance()->RegLogoutCb(logout_cb);

}

bool LoginAgora::CheckStatus(std::string app_id,std::string account, std::string pwd, AgoraLoginCb cb)
{
	AGLoginManager* ag_login_manager = AGLoginManager::GetInstance();
	if (ag_login_manager->IsOnline())
	{
		return true;
	}
	else
	{
		Login(app_id,account, pwd, cb);
		return false;
	}
}


bool LoginAgora::IsOnline()
{
	return AGLoginManager::GetInstance()->IsOnline();
}

void LoginAgora::SetAppid(std::string id)
{

}

