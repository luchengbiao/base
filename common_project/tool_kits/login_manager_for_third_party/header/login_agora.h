#pragma once
#include "login_base.h"
#include "base\memory\singleton.h"
#define SUCCESS 0
#define LOGOUT_E_OTHER  100 //未知原因导致退出登录, 通常是由于网络问题引起的。
#define LOGOUT_E_USER   101	//用户登出
#define LOGOUT_E_KICKED 103	//该账户已在别处登录

#define LOGIN_E_NET		201	//网络问题	
#define LOGIN_E_TOKEN_KICKED 207	//用户已使用更新后的 Signaling Key 在别处登录了系统。
#define LOGIN_E_FAILED 202	//被服务器拒绝
/*
	声网CODE
	SUCCESS				0				没有错误。
	LOGOUT_E_OTHER		100				未知原因导致退出登录, 通常是由于网络问题引起的。
	LOGOUT_E_USER		101				用户登出
	LOGOUT_E_NET		102				网络问题
	LOGOUT_E_KICKED		103				该账户已在别处登录
	LOGOUT_E_PACKET		104				已被弃用
	LOGOUT_E_TOKENEXPIRED	105			Signaling Key过期
	LOGOUT_E_OLDVERSION	106				已被弃用
	LOGOUT_E_TOKENWRONG	107				已被弃用
	LOGIN_E_OTHER		200				原因未知
	LOGIN_E_NET			201				网络问题
	LOGIN_E_FAILED	202	被服务器拒绝
	LOGIN_E_CANCEL	203	用户已取消登录
	LOGIN_E_TOKENEXPIRED	204	Signaling Key过期，登录被拒
	LOGIN_E_OLDVERSION	205	已被弃用
	LOGIN_E_TOKENWRONG	206	Signaling Key 无效，详见 返回LOGIN_E_TOKENWRONG(206)错误 。
	LOGIN_E_TOKEN_KICKED	207	用户已使用更新后的 Signaling Key 在别处登录了系统。
	LOGIN_E_ALREADY_LOGIN	208	用户已登录，再次发起登录会触发该错误。可以忽视该错误。
	JOINCHANNEL_E_OTHER	300	加载媒体引擎失败。
	SENDMESSAGE_E_OTHER	400	加载媒体引擎失败。
	SENDMESSAGE_E_TIMEOUT	401	发送消息超时
	QUERYUSERNUM_E_OTHER	500	查询频道用户号码失败
	QUERYUSERNUM_E_TIMEOUT	501	查询频道用户号码超时
	QUERYUSERNUM_E_BYUSER	501	已被弃用
	LEAVECHANNEL_E_OTHER	600	未知原因导致退出频道
	LEAVECHANNEL_E_KICKED	601	被管理员踢出频道
	LEAVECHANNEL_E_BYUSER	602	用户不在频道内。
	LEAVECHANNEL_E_LOGOUT	603	登出时被踢出频道
	LEAVECHANNEL_E_DISCONN	604	网络问题导致退出频道
	INVITE_E_OTHER	700	呼叫失败
	INVITE_E_REINVITE	701	重复呼叫
	INVITE_E_NET	702	网络问题
	INVITE_E_PEEROFFLINE	703	对方处于离线状体。
	INVITE_E_TIMEOUT	704	呼叫超时
	INVITE_E_CANTRECV	705	已被弃用
	GENERAL_E	1000	一般错误
	GENERAL_E_FAILED	1001	一般错误 - 失败
	GENERAL_E_UNKNOWN	1002	一般错误 - 未知
	GENERAL_E_NOT_LOGIN	1003	一般错误 - 操作前没有登录
	GENERAL_E_WRONG_PARAM	1004	一般错误 - 参数调用失败
*/

using AgoraLoginCb = std::function<void(int, std::wstring, uint32_t)>;//code,msg,u_id
using AgoraLogoutCb = std::function<void(int, std::wstring)>;
class LoginAgora :public LoginBase
{
public:
	SINGLETON_DEFINE(LoginAgora);
	LoginAgora();
	~LoginAgora();
	void Login(std::string app_key,std::string account, std::string pwd, AgoraLoginCb cb);
	void Logout(AgoraLogoutCb);
	void Logout();

	void RegLogoutCb(AgoraLogoutCb);	//注册登出情况的回调
	
	bool CheckStatus(std::string app_id,std::string account, std::string pwd, AgoraLoginCb cb);	//返回是否在线，并且在未在线的情况下自动登录


	bool IsOnline();	//只返回是否在线
	void SetAppid(std::string id);

};