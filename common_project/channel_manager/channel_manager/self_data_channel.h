#pragma once
#include "base_data_channel.h"

class SelfDataChannel : public BaseDataChannel
{
public:
	SelfDataChannel(bool master);
	SelfDataChannel(){};
	~SelfDataChannel();

	virtual void JoinChannel(std::string channel_id, JoinChannelCallback& cb);
	virtual void ReJoinChannel(JoinChannelCallback& cb);
	virtual void LeaveChannel(std::string channel_id, std::string notified_account, LeaveChannelCallback& cb);
	virtual void SendChannelMessage(std::string channel_id, std::string data);
	virtual void SendP2PMessage(std::string account, uint32_t uid, std::string msg);

	virtual void SetConnection(bool b_connect);
	virtual void GetChannelInfo(ChannelInfo& info);
	virtual NetworkServiceType GetChannelType();
	virtual std::string GetSessionOrChannelID();
	virtual void SetSessionOrChannelID(std::string session_id){ s_channel_id_ = session_id; };

private:
	std::string s_channel_id_;
	bool b_connection_;
	int heart_beat_interval_;
};