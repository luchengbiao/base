#include "agora_login_callback.h"
#include "manager\agora_login_manager.h"

void AGLoginCallback::OnLoginSuccessCallback(uint32_t uid, int fd)
{
	AGLoginManager::GetInstance()->OnloginSuccessCallback(uid, fd);
}

void AGLoginCallback::OnLoginFailedCallback(int ecode)
{
	AGLoginManager::GetInstance()->OnloginFailCallback(ecode);
}

void AGLoginCallback::OnLogoutCallback(int ecode)
{
	AGLoginManager::GetInstance()->OnlogoutCallback(ecode);
}

