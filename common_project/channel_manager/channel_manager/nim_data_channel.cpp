#include "nim_data_channel.h"
#include "nim_rts/manager/rts_manager.h"
#include "nim_login/manager/login_manager.h"
#include "base/util/string_util.h"

NimDataChannel::NimDataChannel(bool master) : BaseDataChannel(master)
{
	b_connection_ = false;
	rts_session_id_ = "";
	s_channel_id_ = "";
	rts_login_ = false;
}

NimDataChannel::~NimDataChannel()
{

}

void NimDataChannel::JoinChannel(std::string channel_id, JoinChannelCallback& cb)
{
	s_channel_id_ = channel_id;
	m_joinchannelCb_ = cb;
	if (b_master_)
	{
		auto cb = nbase::Bind(&NimDataChannel::CreateChannelCb, this, std::placeholders::_1);
		nim_comp::RtsManager::GetInstance()->CreateConf(s_channel_id_, "", cb);
	}
	else
	{
		JoinChannelPrivate(s_channel_id_);
	}
}

void NimDataChannel::ReJoinChannel(JoinChannelCallback& cb)
{
	if (nim_comp::LoginManager::GetInstance()->GetLoginStatus() == LoginStatus_SUCCESS)
	{
		m_joinchannelCb_ = cb;
		JoinChannelPrivate(s_channel_id_);
	}
}

void NimDataChannel::CreateChannelCb(nim::NIMResCode res_code)
{
	if (res_code == nim::kNIMResSuccess || res_code == nim::kNIMResExist)
	{
		//LOG_MSG(L"NimDataChannel::CreateChannelCb res :{0}") << res_code;
		JoinChannelPrivate(s_channel_id_);
	}
	else
	{
		//LOG_ERR(L"NimDataChannel::CreateChannelCb res :{0}") << res_code;

		if (m_joinchannelCb_)
		{
			m_joinchannelCb_(s_channel_id_, res_code);
		}
	}
}

void NimDataChannel::JoinChannelCb(nim::NIMResCode res_code, const std::string& session_id, __int64 channel_id, const std::string& custom_info)
{
	//LOG_MSG(L"NimDataChannel::JoinChannelCb res :{0}") << res_code;
	int code = 0;
	if (res_code == nim::kNIMResSuccess)
	{
		if (!rts_session_id_.empty() && rts_session_id_ != session_id)
		{
			nim_comp::RtsManager::GetInstance()->Hangup(rts_session_id_, nim::Rts::HangupCallback());
		}
		if (rts_login_ && rts_session_id_ != session_id)
		{
			EndChannel();
		}
		else
		{
			rts_login_ = true;
			course_channel_id_ = nbase::StringPrintf("%lld", channel_id);
			b_connection_ = true;
			rts_session_id_ = session_id;
		}
	}
	else
	{
		code = -1;
		EndChannel();
	}

	if (m_joinchannelCb_)
	{
		m_joinchannelCb_(s_channel_id_, code);
	}
}

void NimDataChannel::EndChannel()
{
	rts_login_ = false;
	b_connection_ = false;
	if (!rts_session_id_.empty())
	{
		nim_comp::RtsManager::GetInstance()->Hangup(rts_session_id_, nim::Rts::HangupCallback());
		rts_session_id_.clear();
	}
}

void NimDataChannel::JoinChannelPrivate(std::string channel_id)
{
	auto cb = nbase::Bind(&NimDataChannel::JoinChannelCb, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
	nim_comp::RtsManager::GetInstance()->JoinConf(channel_id, true, cb);
}

void NimDataChannel::LeaveChannel(std::string session_id, std::string notified_account, LeaveChannelCallback& cb)
{
	nim_comp::RtsManager::GetInstance()->Hangup(session_id, nim::Rts::HangupCallback());
	if (cb)
	{
		cb(session_id, 0);
	}
}

void NimDataChannel::SendChannelMessage(std::string channel_id, std::string data)
{
	nim_comp::RtsManager::GetInstance()->SendData(channel_id, nim::kNIMRtsChannelTypeTcp, data);
}

void NimDataChannel::SendP2PMessage(std::string session_id, uint32_t uid, std::string msg)
{
	nim_comp::RtsManager::GetInstance()->Control(session_id, msg, nim::Rts::ControlCallback());
}

void NimDataChannel::SetConnection(bool b_connect)
{
	rts_login_ = b_connect;
	b_connection_ = b_connect;
}

void NimDataChannel::GetChannelInfo(ChannelInfo& info)
{
	info.s_nim_rts_session_id_ = rts_session_id_;
	info.b_channel_connect_ = b_connection_;
	info.nim_channel_id = course_channel_id_;
}

NetworkServiceType NimDataChannel::GetChannelType()
{
	return SWITCH_NETWORK_NETEASE;
}

std::string NimDataChannel::GetSessionOrChannelID()
{
	return rts_session_id_;
}

