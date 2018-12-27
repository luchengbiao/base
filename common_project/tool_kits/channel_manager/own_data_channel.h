#pragma once
#include "base_data_channel.h"

class OwnDataChannel :public BaseDataChannel
{
	Q_OBJECT
public:
	OwnDataChannel(bool b_master);
	~OwnDataChannel();

	virtual void JoinChannel(std::string channel_id, JoinChannelCallback& cb, std::string url = "");
	virtual void ReJoinChannel(JoinChannelCallback& cb);
	virtual void LeaveChannel(std::string channel_id, std::string notified_account, LeaveChannelCallback& cb);
	virtual void SendChannelMessage(std::string channel_id, std::string data);
	virtual void SendP2PMessage(std::string account, uint32_t uid, std::string msg);

	virtual void SetConnection(bool b_connect);
	virtual void GetChannelInfo(ChannelInfo& info);
	virtual NetworkServiceType GetChannelType();
	virtual std::string GetSessionOrChannelID();
	virtual void SetSessionOrChannelID(std::string session_id);
	virtual void SetConnectStateCb(DataChannelConnectStateCb connect_cb);
	virtual void SetChannelMessageCb(DataChannelMessageCb msg_cb);

private:
	void OnJoinChannelCb(std::string message);
	void OnConnectCloseCb();
	void OnRejoinConnect();
	void OnStartSchedulePing();
	void PingWebSocket();
	void StopHeartBeat();

private:
	std::string ssl_url_;
	bool b_connect_;
	std::string s_channel_id_;
	DataChannelConnectStateCb connect_state_cb_;
	DataChannelMessageCb receive_message_cb_;
	JoinChannelCallback m_joinchannelCb_;

	nbase::WeakCallbackFlag long_connection_timer_;
	nbase::WeakCallbackFlag reconnect_timer_;

	int count_ping_pong_;   //累计ping 次数 收到pong  重置0
};