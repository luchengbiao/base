#include "self_data_channel.h"


SelfDataChannel::SelfDataChannel(bool master) : BaseDataChannel(master)
{
	s_channel_id_ = "";
	b_connection_ = "";
	heart_beat_interval_ = 3000;
}

SelfDataChannel::~SelfDataChannel()
{

}

void SelfDataChannel::JoinChannel(std::string channel_id, JoinChannelCallback& cb)
{

}

void SelfDataChannel::ReJoinChannel(JoinChannelCallback& cb)
{

}

void SelfDataChannel::LeaveChannel(std::string channel_id, std::string notified_account, LeaveChannelCallback& cb)
{

}

void SelfDataChannel::SendChannelMessage(std::string channel_id, std::string data)
{

}

void SelfDataChannel::SendP2PMessage(std::string account, uint32_t uid, std::string msg)
{

}

void SelfDataChannel::SetConnection(bool b_connect)
{

}

void SelfDataChannel::GetChannelInfo(ChannelInfo& info)
{

}

NetworkServiceType SelfDataChannel::GetChannelType()
{
	return SWITCH_NETWORK_SELF;
}

std::string SelfDataChannel::GetSessionOrChannelID()
{
	return s_channel_id_;
}
