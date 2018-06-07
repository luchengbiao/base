#pragma once
#include "base_invite.h"
#include "nim_sdk_manager\api\nim_cpp_nos.h"

class NimInvite : public BaseInvite
{
public:
	NimInvite();
	~NimInvite();
	virtual void OnSetCallInfo(One2OneCallInfo info){ info_ = info; }
	virtual void OnInitInvite(One2OneCallInfo info, bool b_force_join = false) override;
	virtual void StartInvite() override;
	virtual void StartInviteByAccount(std::string account) override;						//邀请失败后声网继续发起邀请对方
	virtual void AcceptInvite() override;
	virtual void RefuseInvite() override;
	virtual void EndInvite(std::string peer_account) override;
	virtual void CancelInvite() override;
	virtual void CancelAllService() override;
	virtual void EndFailInvite(bool b_creator) override;
	virtual bool GetDetail(BaseInviteDetail& detail)override;
private:
	void OnStartRtsCb(std::string old_id, nim::NIMResCode res_code,
		const std::string session_id, int channel_type, const std::string uid, const std::string channel_id);

private:
	One2OneCallInfo info_;
	std::string session_id_;
	std::string course_channel_id_;
	nbase::WeakCallbackFlag call_repeat_;
};