#pragma once
#include <stdint.h>

class AGLoginCallback
{
public:
	static void OnLoginSuccessCallback(uint32_t uid, int fd);

	static void OnLoginFailedCallback(int ecode);

	static void OnLogoutCallback(int ecode);
};