#pragma once
#include <functional>
#include "base\callback\callback.h"
#include "base\memory\singleton.h"


using LoginCb = std::function<void(int)>;	//code = 0Îª³É¹¦
using LogoutCb = std::function<void(int)>;
class LoginBase : public nbase::SupportWeakCallback
{
	
public:
	SINGLETON_DEFINE(LoginBase);
	LoginBase(){};
	~LoginBase(){};

	 void Login(std::string account, std::string pwd, LoginCb cb){};
	 void Logout(LogoutCb cb){ };

};