#pragma once
#include "base\callback\callback.h"
#include "invite_manager_global.h"

struct BaseInviteDetail
{
	std::string nim_channel_id;

	BaseInviteDetail() : nim_channel_id("")
	{

	}
};

class BaseInvite :public nbase::SupportWeakCallback
{
public:
	BaseInvite();
	~BaseInvite();

	virtual void OnSetCallInfo(One2OneCallInfo info) = 0;
	virtual void OnInitInvite(One2OneCallInfo info, bool b_force_join = false) = 0;	//初始化邀请	
	virtual void StartInvite() = 0;							//发起邀请
	virtual void StartInviteByAccount(std::string account) = 0;						//邀请失败后声网继续发起邀请对方
	virtual void AcceptInvite() = 0;						//接受邀请
	virtual void RefuseInvite() = 0;						//拒绝邀请
	virtual void CancelInvite() = 0;						//主动取消邀请
	virtual void EndInvite(std::string peer_account) = 0;	//超时结束邀请 
	virtual void EndFailInvite(bool b_creator) = 0;			//邀请不成功结束 
	virtual void CancelAllService() = 0;					//主要是处理定时器
	virtual bool GetDetail(BaseInviteDetail& detail) = 0;
};