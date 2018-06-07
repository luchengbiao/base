#pragma once
#include "agora/agora_sig.h"

class AGSignallingManager
{
public:
	AGSignallingManager(void);
	~AGSignallingManager(void);

	void Login(std::string s_vendorID, std::string s_account, std::string s_token, uint32_t uid, std::string s_deviceID);
	void Login(std::string s_vendorID, std::string s_account, std::string s_token, uint32_t uid, std::string s_deviceID, int retry_time_in_s, int retry_count);
	void Logout();
	void ChannelJoin(std::string s_channelID);
	void ChannelLeave(std::string s_channelID);
	void ChannelQueryUserNum(std::string s_channelID);
	void ChannelSetAttr(std::string s_channelID, std::string s_name, std::string s_value);
	void ChannelDelAttr(std::string s_channelID, std::string s_name);
	void ChannelClearAttr(std::string s_channelID);
	void ChannelInviteUser(std::string s_channelID, std::string s_account, uint32_t uid = 0);
	void ChannelInviteUser(std::string s_channelID, std::string s_account, std::string s_extra);
	void ChannelInvitePhone(std::string s_channelID, std::string s_phoneNum, uint32_t uid = 0);
	void ChannelInvitePhone(std::string s_channelID, std::string s_phoneNum, std::string s_sourcesNum);
	void ChannelInvitePhone(std::string s_channelID, std::string s_phoneNum, std::string s_sourcesNum, std::string s_extra);
	void ChannelInviteDTMF(std::string s_channelID, std::string s_phoneNum, std::string s_dtmf);
	void ChannelInviteAccept(std::string s_channelID, std::string s_account, uint32_t uid);
	void ChannelInviteRefuse(std::string s_channelID, std::string s_account, uint32_t uid, std::string s_extra);
	void ChannelInviteEnd(std::string s_channelID, std::string s_account, uint32_t uid);
	void MessageAppSend(std::string s_msg, std::string s_msgID);
	void MessageInstantSend(std::string s_account, uint32_t uid, std::string s_msg, std::string s_msgID);
	void MessageChannelSend(std::string s_channelID, std::string s_msg, std::string s_msgID);
	void MessageChannelSendFast(std::string s_channelID, std::string s_msg, std::string s_msgID);
	void MessagePushSend(std::string s_account, uint32_t uid, std::string s_msg, std::string s_msgID);
	void MessageChatSend(std::string s_account, uint32_t uid, std::string s_msg, std::string s_msgID);
	void MessageDTMFSend(uint32_t uid, std::string s_msg, std::string s_msgID);
	void SetBackground(uint32_t bOut);
	void SetNetworkStatus(uint32_t bOut);
	void Ping();
	void SetAttr(std::string s_name, std::string s_value);
	void GetAttr(std::string s_name);
	void GetAttrAll();
	void GetUserAttr(std::string s_account, std::string s_name);
	void GetUserAttrAll(std::string s_account);
	void QueryUserStatus(std::string s_account);
	void Invoke(std::string s_name, std::string s_req, std::string call_id);
	void Start();
	void Stop();
	bool IsOnline();
	int GetStatus();
	int GetSdkVersion();
	void Dbg(std::string s_value1, std::string s_value2);

private:
	static agora_sdk_win::IAgoraAPI *m_lpAgoraInst;
	static AGSignallingManager* m_lpAgoraSignalling;

public:
	static AGSignallingManager *GetAGSignalling(std::string s_vendor = "");
	static void CloseAGSignalling();

	static agora_sdk_win::ICallBack* m_lpSignallingEventHandler;
};
