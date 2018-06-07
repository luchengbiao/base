#pragma once
#include "agora/agora_sig.h"

class AGSignallingCallBack : public agora_sdk_win::ICallBack{

public:
	AGSignallingCallBack();
	~AGSignallingCallBack();

	virtual void onReconnecting(uint32_t nretry) override;

	virtual void onReconnected(int fd) override;

	virtual void onLoginSuccess(uint32_t uid, int fd) override;

	virtual void onMessageAppReceived(char const * msg, size_t msg_size)  override;

	virtual void onChannelQueryUserNumResult(char const * channelID, size_t channelID_size, int ecode, int num)  override;

	virtual void onLoginFailed(int ecode)  override;

	virtual void onLogout(int ecode)  override;

	virtual void onChannelJoined(char const * channelID, size_t channelID_size)  override;

	virtual void onChannelUserJoined(char const * account, size_t account_size, uint32_t uid)  override;

	virtual void onChannelUserLeaved(char const * account, size_t account_size, uint32_t uid)  override;

	virtual void onChannelLeaved(char const * channelID, size_t channelID_size, int e_code)  override;

	virtual void onChannelUserList(int n, char **accounts, uint32_t* uids)  override;

	virtual void onInviteReceived(char const * channelID, size_t channelID_size, char const * account, size_t account_size,
		uint32_t uid, char const * extra, size_t extra_size)  override;

	virtual void onInviteReceivedByPeer(char const * channelID, size_t channelID_size, char const * account, size_t account_size, uint32_t uid)  override;

	virtual void onInviteAcceptedByPeer(char const * channelID, size_t channelID_size, char const * account, size_t account_size, 
		uint32_t uid, char const * extra, size_t extra_size)  override;

	virtual void onInviteRefusedByPeer(char const * channelID, size_t channelID_size, char const * account, size_t account_size, 
		uint32_t uid, char const * extra, size_t extra_size)  override;

	virtual void onInviteEndByPeer(char const * channelID, size_t channelID_size, char const * account, size_t account_size, 
		uint32_t uid, char const * extra, size_t extra_size)  override;

	virtual void onLog(char const * txt, size_t txt_size)  override;

	virtual void onMessageInstantReceive(char const * account, size_t account_size, uint32_t uid, char const * msg, size_t msg_size)  override;

	virtual void onMessageChannelReceive(char const * channelID, size_t channelID_size, 
		char const * account, size_t account_size, uint32_t uid, char const * msg, size_t msg_size)  override;

	virtual void onInviteFailed(char const * channelID, size_t channelID_size, char const * account, size_t account_size, 
		uint32_t uid, int ecode, char const * extra, size_t extra_size) override;
	
	virtual void onInviteEndByMyself(char const * channelID, size_t channelID_size, char const * account, size_t account_size, uint32_t uid) override;

	virtual void onChannelJoinFailed(char const * channelID, size_t channelID_size, int ecode) override;

	virtual void onMessageSendError(char const * messageID, size_t messageID_size, int ecode) override;

	virtual void onMessageSendSuccess(char const * messageID, size_t messageID_size) override;

};