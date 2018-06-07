#pragma once
#include "nim_sdk_manager\helper\nim_client_helper.h"
namespace nim_comp
{
	/** @class LoginCallback
	* @brief 登录注销相关回调函数类
	* @copyright (c) 2015, NetEase Inc. All rights reserved
	* @author towik
	* @date 2015/1/1
	*/
	class LoginCallback
	{
	public:
		/**
		* 登录结果回调
		* @param[in] json_params 返回的信息，Json数据转化的字符串
		* @param[in] user_data 登录时用户传的数据，sdk原封不动传给回调
		* @return void	无返回值
		*/
		static void OnLoginCallback(const nim::LoginRes& login_res, const void* user_data);
		/**
		* 登录成功或失败时，做一些界面上的显示。
		* @param[in] code 登录返回码
		* @param[in] relogin 是否重新登录
		* @return void	无返回值
		*/
		static void UILoginCallback(nim::NIMResCode code, bool relogin);

		/**
		* 退出登录结果回调
		* @param[in] json_params 返回的信息，Json数据转化的字符串
		* @return void	无返回值
		*/
		static void OnLogoutCallback(nim::NIMResCode res_code);
		/**
		* 被其他端踢下线的回调，要在程序开始运行时就注册好。
		* @param[in] json_params 返回的信息，Json数据转化的字符串
		* @return void	无返回值
		*/
		static void OnKickoutCallback(const nim::KickoutRes& res);

		/**
		* 掉线的回调，要在程序开始运行时就注册好。
		* @param[in] json_params 返回的信息，Json数据转化的字符串
		* @return void	无返回值
		*/
		static void OnDisconnectCallback();

		/**
		* 自动重连的回调，要在程序开始运行时就注册好。
		* @param[in] json_params 返回的信息，Json数据转化的字符串
		* @return void	无返回值
		*/
		static void OnReLoginCallback(const nim::LoginRes& login_res);

		/**
		* 移动端登录时的回调，要在程序开始运行时就注册好。
		* @param[in] json_params 返回的信息，Json数据转化的字符串
		* @return void	无返回值
		*/
		static void OnMultispotLoginCallback(const nim::MultiSpotLoginRes& res);

		/**
		* 把移动端踢下线的结果回调，要在程序开始运行时就注册好。
		* @param[in] json_params 返回的信息，Json数据转化的字符串
		* @return void	无返回值
		*/
		static void OnKickoutOtherClientCallback(const nim::KickOtherRes& res);
	};
}