#include "..\header\login_net_ease.h"
#include "nim_login\manager\login_manager.h"
#include "..\..\nim_manager\init\nim_init_manager.h"
#include "nim_sdk_manager\helper\nim_client_helper.h"
#include "base\util\string_number_conversions.h"
void NetEaseLogin::Login(std::string app_key,std::string account, std::string pwd, NetEaseLoginCb cb)
{
	nim_comp::LoginManager* nim_log = nim_comp::LoginManager::GetInstance();
	if (nim_log->GetLoginStatus() == LoginStatus_LOGIN)
	{
		if (cb){
			cb(0, L"");
		}
	}


	std::string old_app_key = "45c6af3c98409b18a84451215d0bdd6e";
	std::string new_app_key = app_key;//GetConfigValue(g_AppKey);
	if (!new_app_key.empty())
	{
		old_app_key = new_app_key;
	}


	nim_log->SetAccount(account);
	nim_log->SetPassword(pwd);
	nim_log->SetAPPKey(old_app_key);

	auto log_cb = ToWeakCallback([=](int code){
		std::wstring error = L"";
		if (code == nim::kNIMResSuccess)
		{
			nim_log->SetLoginStatus(LoginStatus_LOGIN);
			DoRegLogoutCb();
			//nim_log->RegLogoutCb(LogoutCb(code));	//ע��ǳ��ص�
		}
		else
		{
			
			if (code == nim::kNIMResUidPassError)
			{
				error = L"�û����������������������";
			}
			else if (code == nim::kNIMResConnectionError)
			{
				error = L"����������⣬��ȷ����������";
			}
			else if (code == nim::kNIMResTimeoutError)
			{
				error = L"���ӳ�ʱ";
			}
			else if (code == nim::kNIMResExist)
			{
				error = L"���������豸�ϵ�¼���������µ�¼";
			}
			else
			{
				std::wstring s_code = nbase::IntToString16(code);
				error = L"��¼ʧ�ܣ�������" + s_code;
			}
		}

		if (cb)
		{
			cb(code, error);
		}
	});

	nim_log->RegLoginCb(log_cb);
	nim_log->DoLogin(account, pwd);
}


void NetEaseLogin::Login(std::string app_key,std::string account, std::string pwd, NetEaseLoginCb in_cb, NetEaseLogoutCb out_cb)
{
	nim_comp::LoginManager* nim_log = nim_comp::LoginManager::GetInstance();
	if (nim_log->GetLoginStatus() == LoginStatus_LOGIN)
	{
		if (in_cb){
			in_cb(0, L"");
		}
	}


	std::string old_app_key = "45c6af3c98409b18a84451215d0bdd6e";
	std::string new_app_key = app_key;//GetConfigValue(g_AppKey);
	if (!new_app_key.empty())
	{
		old_app_key = new_app_key;
	}


	nim_log->SetAccount(account);
	nim_log->SetPassword(pwd);
	nim_log->SetAPPKey(old_app_key);

	auto log_cb = ToWeakCallback([=](int code){
		std::wstring error = L"";
		if (code == nim::kNIMResSuccess)
		{
			nim_log->SetLoginStatus(LoginStatus_LOGIN);
			RegLogout(out_cb);
		}
		else
		{

			if (code == nim::kNIMResUidPassError)
			{
				error = L"�û����������������������";
			}
			else if (code == nim::kNIMResConnectionError)
			{
				error = L"����������⣬��ȷ����������";
			}
			else if (code == nim::kNIMResTimeoutError)
			{
				error = L"���ӳ�ʱ";
			}
			else if (code == nim::kNIMResExist)
			{
				error = L"���������豸�ϵ�¼���������µ�¼";
			}
			else
			{
				std::wstring s_code = nbase::IntToString16(code);
				error = L"��¼ʧ�ܣ�������" + s_code;
			}
		}

		if (in_cb)
		{
			in_cb(code, error);
		}
	});

	nim_log->RegLoginCb(log_cb);
	nim_log->DoLogin(account, pwd);
}

bool NetEaseLogin::CheckStatus(std::string app_key,std::string account, std::string pwd, NetEaseLoginCb cb)
{
	nim_comp::LoginManager* nim_log = nim_comp::LoginManager::GetInstance();
	if (nim_log->GetLoginStatus() == LoginStatus_LOGIN)
	{
		return true;
	}
	else
	{
		Login(app_key,account, pwd, cb);
		return false;
	}
	return true;
}

void NetEaseLogin::RegLogout(NetEaseLogoutCb cb)
{
	auto logout_cb = ToWeakCallback([=](int code){
		std::wstring error;
		if (code == 0)
		{
			error = L"�ǳ��ɹ�";
		}

		else if (code == nim::kNIMResConnectionError)
		{
			error = L"����������⣬��ȷ����������";
		}
		else if (code == Logout_KICKOUT)
		{
			error = L"���������豸�ϵ�¼���������µ�¼";
		}
		else if (code == nim::kNIMResTimeoutError)
		{
			error = L"���ӳ�ʱ";
		}
		else
		{
			std::wstring s_code = nbase::IntToString16(code);
			error = L"�����룺" + s_code;
		}
		if (cb)
		{
			cb(code, error);
		}
	});
	nim_comp::LoginManager::GetInstance()->RegLogoutCb(logout_cb);
}


void NetEaseLogin::RegLoginCb(NetEaseLoginCodeCb cb)
{
	nim_comp::LoginManager::GetInstance()->RegLoginCb(cb);
}

bool NetEaseLogin::IsOnline()
{
	return 	(nim_comp::LoginManager::GetInstance()->GetLoginStatus() == LoginStatus_LOGIN);
}

void NetEaseLogin::Logout(NetEaseLogoutCb cb, nim::NIMLogoutType type)
{
	nim_comp::LoginManager* nim_log = nim_comp::LoginManager::GetInstance();
	auto task_cb = ToWeakCallback([=](int code){
		std::wstring error = L"";
		if (code == 0)
		{
			error = L"";
		}

		else if (code == nim::kNIMResConnectionError)
		{
			error = L"����������⣬��ȷ����������";
		}
	
		else if (code == nim::kNIMResExist)
		{
			error = L"���������豸�ϵ�¼���������µ�¼";
		}
		if (cb)
		{
			cb(code, error);
		}
	});

	nim_log->RegLogoutCb(task_cb);
	nim_log->DoLogout(false,type);
	
}


void NetEaseLogin::Logout()
{
	nim_comp::LoginManager* nim_log = nim_comp::LoginManager::GetInstance();
	nim_log->DoLogout(false, nim::kNIMLogoutPersonal);
}

void NetEaseLogin::LogoutCb(int code)
{

}

void NetEaseLogin::DoRegLogoutCb()
{
	nim_comp::LoginManager* nim_log = nim_comp::LoginManager::GetInstance();
	auto LogoutCb = ToWeakCallback([=](int code){
		std::wstring error;
		if (code)
		{
			if (code == 0)
			{
				error = L"";
			}

			else if (code == nim::kNIMResConnectionError)
			{
				error = L"����������⣬��ȷ����������";
			}

			else if (code == nim::kNIMResExist)
			{
				error = L"���������豸�ϵ�¼���������µ�¼";
			}
		}

	});

	nim_log->RegLogoutCb(LogoutCb);
}

