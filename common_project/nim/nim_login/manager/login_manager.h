#pragma once

#include "base/memory/singleton.h"
#include <string>
#include <map>
#include <functional>
#include <memory>
#include "base/util/auto_unregister.h"
#include "base/callback/callback.h"
#include "nim_sdk_manager/helper/nim_client_helper.h"

enum LoginStatus
{
	LoginStatus_NONE,		//初始状态
	LoginStatus_LOGIN,		//正在登录
	LoginStatus_CANCEL,		//取消登录
	LoginStatus_SUCCESS,	//已经登录
	LoginStatus_EXIT		//退出登录
};

enum LogoutCode
{
	Logout_SUCCESS = 0,		//登出成功
	Logout_DISCONNECT,		//掉线
	Logout_KICKOUT,		//被踢
};

typedef std::function<void(const int&)> NimLoginCallback;
typedef std::function<void(const int&)> NimLogoutCallback;

namespace nim_comp
{
/** @class LoginManager
  * @brief 登陆管理器
  * @copyright (c) 2016, NetEase Inc. All rights reserved
  * @date 2016/09/18
  */
class LoginManager : public nbase::SupportWeakCallback
{
public:
	SINGLETON_DEFINE(LoginManager);	
	LoginManager();

	/**
	* 判断帐号是否为已登录帐号
	* @param[in] account 用户id
	* @return bool true 是，false 否
	*/
	bool IsSelf(const std::string &account);

	/**
	* 设置登陆账号
	* @param[in] account
	* @return void	无返回值
	*/
	void SetAccount(const std::string &account);

	/**
	* 获取登陆账号
	* @return string 用户id
	*/
	std::string GetAccount();

	/**
	* 判断帐号是否为已登录帐号(自动转换为小写来判断)
	* @param[in] account 用户id
	* @return bool true 是，false 否
	*/
	bool IsEqual(const std::string& account);

	/**
	* 设置登陆密码
	* @param[in] password 登录密码
	* @return void	无返回值
	*/
	void SetPassword(const std::string &password);

	/**
	* 获取登陆密码
	* @return string 用户密码
	*/
	std::string GetPassword();

	/**
	* 设置登陆状态
	* @param[in] status 登录状态
	* @return void	无返回值
	*/
	void SetLoginStatus(LoginStatus status);

	/**
	* 获取登陆状态
	* @return LoginStatus 登录状态
	*/
	LoginStatus GetLoginStatus();

	/**
	* 设置断网重连标记
	* @param[in] active 是否处于连接状态
	* @return void	无返回值
	*/
	void SetLinkActive(bool active);

	/**
	* 判断是否处于连接状态
	* @return bool true 连接，false 断开
	*/
	bool IsLinkActive();

	/**
	* 设置网易的AppKey
	* @param[in] appKey
	* @return void	无返回值
	*/
	void SetAPPKey(std::string appKey);

	/**
	* 获取APP Key
	* @return string APP Key
	*/
	std::string GetAPPKey();

	// init nim log
	void InitUserFolder();

	// reg cb
	UnregisterCallback RegLoginCb(const NimLoginCallback& callback);
	UnregisterCallback RegLogoutCb(const NimLogoutCallback& callback);

	// 
	void DoLogin(std::string user, std::string pass);
	void DoLogout(bool over, nim::NIMLogoutType type);

	// cb
	void OnLoginCallback(int code);
	void OnLogoutCallback(int code);

private:
	std::string account_;
	std::string password_;
	std::string app_key_;
	LoginStatus status_;
	bool active_;
	std::map<int, std::unique_ptr<NimLoginCallback>> login_cb_list_;
	std::map<int, std::unique_ptr<NimLogoutCallback>> logout_cb_list_;
};
}