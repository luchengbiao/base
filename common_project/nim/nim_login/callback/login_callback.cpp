#include "login_callback.h"
#include "log_manager\log.h"
#include <string>
#include "manager\login_manager.h"
#include "nim_sdk_manager\api\nim_cpp_client.h"
#include "nim_base\util.h"
#include "base\file\file_util.h"
#include "nim_base\user.h"
#include "base\callback\callback.h"
#include "qthread_manager\closure.h"
#include "base\time\time.h"
#include "nim\nim_chatroom_c_sdk\include\nim_res_code_def.h"

namespace nim_comp
{
	void LoginCallback::OnLogoutCallback(nim::NIMResCode res_code)
	{
		QLOG_APP(L"OnLogoutCallback: {0}") << res_code;
		QLOG_APP(L"-----logout end-----");
		
		auto task = [=]{
			LoginManager::GetInstance()->OnLogoutCallback(Logout_SUCCESS);
		};

		qtbase::Post2Task(kThreadUIHelper, task);
	}

	void LoginCallback::OnKickoutCallback(const nim::KickoutRes& res)
	{
		QLOG_APP(L"OnKickoutCallback: {0} - {1}") << res.client_type_ << res.kick_reason_;

		auto task = [=]{
			LoginManager::GetInstance()->OnLogoutCallback(Logout_KICKOUT);
		};

		qtbase::Post2Task(kThreadUIHelper, task);
	}

	void LoginCallback::OnDisconnectCallback()
	{
		QLOG_APP(L"OnDisconnectCallback");
		auto task = [=]{
			LoginManager::GetInstance()->OnLogoutCallback(Logout_DISCONNECT);
		};

		qtbase::Post2Task(kThreadUIHelper, task);
	}

	void LoginCallback::UILoginCallback(nim::NIMResCode code, bool relogin)
	{
		if (relogin)
		{
			QLOG_APP(L"-----relogin end {0}-----") << code;
			if (code == nim::kNIMResSuccess)
			{
				LoginManager::GetInstance()->SetLoginStatus(LoginStatus_SUCCESS);
				LoginManager::GetInstance()->SetLinkActive(true);
				LoginManager::GetInstance()->OnLoginCallback(code);
			}
			else if (code == nim::kNIMResTimeoutError || code == nim::kNIMResConnectionError)
			{
				LoginManager::GetInstance()->SetLoginStatus(LoginStatus_NONE);
				LoginManager::GetInstance()->SetLinkActive(false);
				LoginManager::GetInstance()->OnLoginCallback(code);
			}
			else
			{
				LoginManager::GetInstance()->SetLoginStatus(LoginStatus_NONE);
			}
		}
		else
		{
			QLOG_APP(L"-----login end {0}-----") << code;
			
			if (LoginManager::GetInstance()->GetLoginStatus() == LoginStatus_CANCEL)
			{
				QLOG_APP(L"-----login cancel end-----");
				if (code == nim::kNIMResSuccess) {
					LoginManager::GetInstance()->DoLogout(false, nim::kNIMLogoutChangeAccout);
				} else {
					LoginManager::GetInstance()->OnLogoutCallback(Logout_SUCCESS);
				}
				return;
			}
			else
			{
				LoginManager::GetInstance()->SetLoginStatus(code == nim::kNIMResSuccess ? LoginStatus_SUCCESS : LoginStatus_NONE);
			}

			LoginManager::GetInstance()->OnLoginCallback(code);
		}
	}

	void LoginCallback::OnLoginCallback(const nim::LoginRes& login_res, const void* user_data)
	{
		QLOG_APP(L"OnLoginCallback: {0} - {1}") << login_res.login_step_ << login_res.res_code_;
		if (login_res.res_code_ == nim::kNIMResSuccess)
		{
			if (login_res.login_step_ == nim::kNIMLoginStepLogin)
			{
				qtbase::Post2Task(kThreadUIHelper, nbase::Bind(&UILoginCallback, login_res.res_code_, false));
			}
		}
		else
		{
			qtbase::Post2Task(kThreadUIHelper, nbase::Bind(&UILoginCallback, login_res.res_code_, false));
		}
	}

	void LoginCallback::OnReLoginCallback(const nim::LoginRes& login_res)
	{
		QLOG_APP(L"OnReLoginCallback: {0} - {1}") << login_res.login_step_ << login_res.res_code_;
		if (login_res.res_code_ == nim::kNIMResSuccess)
		{
			if (login_res.login_step_ == nim::kNIMLoginStepLogin)
			{
				qtbase::Post2Task(kThreadUIHelper, nbase::Bind(&UILoginCallback, login_res.res_code_, true));
			}
		}
		else
		{
			qtbase::Post2Task(kThreadUIHelper, nbase::Bind(&UILoginCallback, login_res.res_code_, true));
		}
	}

	//多端
	void LoginCallback::OnMultispotLoginCallback(const nim::MultiSpotLoginRes& res)
	{
		QLOG_APP(L"OnMultispotLoginCallback: {0} - {1}") << res.notify_type_ << res.other_clients_.size();
	}

	void LoginCallback::OnKickoutOtherClientCallback(const nim::KickOtherRes& res)
	{
		//bool success = res.res_code_ == nim::kNIMResSuccess;
		//if (success && !res.device_ids_.empty())
		//{
		//	// 移动端把我们踢下线
		//}
	}
}