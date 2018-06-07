#pragma once
#include "base_invite.h"

class AgoraInvite : public BaseInvite
{
public:
	AgoraInvite();
	~AgoraInvite();
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
	virtual bool GetDetail(BaseInviteDetail& detail) override;

private:
	void DataJoinCallBack(std::string channel_id, int code);
	void ChatJoinCallBack(int code, std::string channel_id, uint32_t uid, std::string extra_str);

private:
	One2OneCallInfo info_;
	JoinChatCb chat_cb_;
	JoinChannelCallback data_cb_;
};