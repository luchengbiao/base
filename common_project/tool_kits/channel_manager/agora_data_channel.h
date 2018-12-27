#pragma once
#include "base_data_channel.h"
#include "base\util\auto_unregister.h"

class AgoraDataChannel : public BaseDataChannel
{
	Q_OBJECT
public:
	AgoraDataChannel(bool master);
	AgoraDataChannel(){};
	~AgoraDataChannel();

	virtual void JoinChannel(std::string channel_id, JoinChannelCallback& cb, std::string url = "");
	virtual void ReJoinChannel(JoinChannelCallback& cb);
	virtual void LeaveChannel(std::string channel_id, std::string notified_account, LeaveChannelCallback& cb);
	virtual void SendChannelMessage(std::string channel_id, std::string data);
	virtual void SendP2PMessage(std::string account, uint32_t uid, std::string msg);

	virtual void SetConnection(bool b_connect);
	virtual void GetChannelInfo(ChannelInfo& info);
	virtual NetworkServiceType GetChannelType();
	virtual std::string GetSessionOrChannelID();
	virtual void SetSessionOrChannelID(std::string session_id){ s_channel_id_ = session_id; };
	virtual void SetConnectStateCb(DataChannelConnectStateCb connect_cb){}
	virtual void SetChannelMessageCb(DataChannelMessageCb msg_cb){}

private:
	void Login();
	void LoginCallback(int code);

	void OnSignallingJoinCb(std::string s_channel_id);
	void OnSignallingLeaveCb(std::string s_channel_id, int code);
	void OnSignallingJoinFailCb(const std::string s_channel_id, int error);

private:
	std::string s_channel_id_;
	bool b_connection_;
	JoinChannelCallback m_joinchannelCb_;
	UnregisterCallback channel_join_cb_;
	UnregisterCallback channel_join_fail_cb_;
};