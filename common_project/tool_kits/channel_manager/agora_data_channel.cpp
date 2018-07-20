#include "agora_data_channel.h"
#include "agora_invite/agora_invite_manager.h"
#include "agora_signalling_channel/agora_signalling_channel_manager.h"

AgoraDataChannel::AgoraDataChannel(bool master) :BaseDataChannel(master)
{
	b_connection_ = false;
	m_joinchannelCb_ = nullptr;
	s_channel_id_ = "";
}

AgoraDataChannel::~AgoraDataChannel()
{

}

void AgoraDataChannel::JoinChannel(std::string channel_id, JoinChannelCallback& cb)
{
	s_channel_id_ = channel_id;
	ReJoinChannel(cb);
}

void AgoraDataChannel::ReJoinChannel(JoinChannelCallback& cb)
{
	if (channel_join_cb_)
	{
		channel_join_cb_();
	}
	if (channel_join_fail_cb_)
	{
		channel_join_fail_cb_();
	}
	m_joinchannelCb_ = cb;
	auto join_channel_cb = nbase::Bind(&AgoraDataChannel::OnSignallingJoinCb, this, std::placeholders::_1);
	auto join_fail_cb = nbase::Bind(&AgoraDataChannel::OnSignallingJoinFailCb, this, std::placeholders::_1, std::placeholders::_2);
	channel_join_cb_ = AGSignallingChannelManager::GetInstance()->RegChannelJoinCb(join_channel_cb);
	channel_join_fail_cb_ = AGSignallingChannelManager::GetInstance()->RegChannelJoinFailCb(join_fail_cb);


	//TODO  加入频道前先要登录
	/*if (!UserInfo::GetInstance()->IsLoginAgora())
	{
	Login();
	}
	else
	{

	}*/

	AGSignallingChannelManager::GetInstance()->ChannelJoin(s_channel_id_);
}

void AgoraDataChannel::LeaveChannel(std::string channel_id, std::string notified_account, LeaveChannelCallback& cb)
{
	if (channel_join_cb_)
	{
		channel_join_cb_();
	}
	if (channel_join_fail_cb_)
	{
		channel_join_fail_cb_();
	}
	AGSignallingChannelManager::GetInstance()->ChannelLeave(channel_id);
	if (!notified_account.empty())
	{
		AGInviteManager::GetInstance()->ChannelInviteEnd(channel_id, notified_account, 0);
	}
}

void AgoraDataChannel::SendChannelMessage(std::string channel_id, std::string data)
{
	AGSignallingChannelManager::GetInstance()->MessageChannelSend(channel_id, data, "");
}

void AgoraDataChannel::SendP2PMessage(std::string account, uint32_t uid, std::string msg)
{
	AGSignallingChannelManager::GetInstance()->MessageInstantSend(account, uid, msg, "");
}

void AgoraDataChannel::SetConnection(bool b_connect)
{
	b_connection_ = b_connect;
}

NetworkServiceType AgoraDataChannel::GetChannelType()
{
	return SWITCH_NETWORK_AGORA;
}

void AgoraDataChannel::GetChannelInfo(ChannelInfo& info)
{
	info.agora_channel_id_ = s_channel_id_;
	info.b_channel_connect_ = b_connection_;
}

void AgoraDataChannel::Login()
{
	//放到独立模块去做
}

void AgoraDataChannel::LoginCallback(int code)
{
	if (code == 0)
	{
		AGSignallingChannelManager::GetInstance()->ChannelJoin(s_channel_id_);
	}
}

void AgoraDataChannel::OnSignallingJoinCb(std::string s_channel_id)
{
	if (s_channel_id == s_channel_id_)
	{
		b_connection_ = true;
		if (m_joinchannelCb_)
		{
			m_joinchannelCb_(s_channel_id_, 0);
		}
	}
	else
	{
		b_connection_ = false;
		if (m_joinchannelCb_)
		{
			m_joinchannelCb_(s_channel_id_, -1);
		}
	}
}

void AgoraDataChannel::OnSignallingLeaveCb(std::string s_channel_id, int code)
{

}

std::string AgoraDataChannel::GetSessionOrChannelID()
{
	return s_channel_id_;
}

void AgoraDataChannel::OnSignallingJoinFailCb(const std::string s_channel_id, int error)
{
	if (s_channel_id == s_channel_id_ && error != 0)
	{
		b_connection_ = false;
		if (m_joinchannelCb_)
		{
			m_joinchannelCb_(s_channel_id_, -1);
		}
	}
}
