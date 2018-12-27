#include "own_data_channel.h"
#include "qthread_manager\closure.h"
#include "log\singleton\log_class_online.h"
#include "..\http_request\ws_service.h"
#include "json\value.h"
#include "json\reader.h"

OwnDataChannel::OwnDataChannel(bool b_master) :BaseDataChannel(b_master)
{
	count_ping_pong_ = 0;
	b_connect_ = false;
	ssl_url_ = "";
	s_channel_id_ = "";
}

OwnDataChannel::~OwnDataChannel()
{
	reconnect_timer_.Cancel();
	long_connection_timer_.Cancel();
}

void OwnDataChannel::JoinChannel(std::string channel_id, JoinChannelCallback& cb, std::string url)
{
	s_channel_id_ = channel_id;
	m_joinchannelCb_ = cb;
	ssl_url_ = url;

	WsService::GetInstance()->OnClose();

	auto join_channel_cb = nbase::Bind(&OwnDataChannel::OnJoinChannelCb, this, std::placeholders::_1);
	auto connect_close_cb = nbase::Bind(&OwnDataChannel::OnConnectCloseCb, this);
	WsService::GetInstance()->RegisterConnectCloseCallback(connect_close_cb);
	WsService::GetInstance()->RegisterMessageCallback(join_channel_cb);
	std::string ws_url = ssl_url_;
	auto task_cb = ToWeakCallback([ws_url](){
		WsService::GetInstance()->OnConnect(ws_url);
	});
	qtbase::Post2Task(kThreadWebSocket, task_cb);
	LOG_O2O(L"websocket url:{0}.") << ssl_url_;
	LOG_O2O(L"Join Own Data Channel: first connect...");
}

void OwnDataChannel::ReJoinChannel(JoinChannelCallback& cb)
{

}

void OwnDataChannel::LeaveChannel(std::string channel_id, std::string notified_account, LeaveChannelCallback& cb)
{
	LOG_O2O(L"OwnDataChannel::LeaveChannel");
	reconnect_timer_.Cancel();
	StopHeartBeat();
	WsService::GetInstance()->OnClose();
}

void OwnDataChannel::SendChannelMessage(std::string channel_id, std::string data)
{
	if (b_connect_)
	{
		WsService::GetInstance()->OnSendMessage(data);
	}
}

void OwnDataChannel::SendP2PMessage(std::string account, uint32_t uid, std::string msg)
{

}

void OwnDataChannel::SetConnection(bool b_connect)
{
	b_connect_ = b_connect;
}

void OwnDataChannel::GetChannelInfo(ChannelInfo& info)
{
	info.agora_channel_id_ = s_channel_id_;
	info.b_channel_connect_ = b_connect_;
}

NetworkServiceType OwnDataChannel::GetChannelType()
{
	return SWITCH_NETWORK_OWN;
}

std::string OwnDataChannel::GetSessionOrChannelID()
{
	return s_channel_id_;
}

void OwnDataChannel::SetSessionOrChannelID(std::string session_id)
{

}

void OwnDataChannel::SetConnectStateCb(DataChannelConnectStateCb connect_cb)
{
	connect_state_cb_ = connect_cb;
}

void OwnDataChannel::SetChannelMessageCb(DataChannelMessageCb msg_cb)
{
	receive_message_cb_ = msg_cb;
}

void OwnDataChannel::OnJoinChannelCb(std::string message)
{
	if (message == "pong")
	{
		count_ping_pong_ = 0;
	}
	else
	{
		//收到消息表明websocket已经成功连接
		if (m_joinchannelCb_)
		{
			m_joinchannelCb_(s_channel_id_, 0);
			m_joinchannelCb_ = nullptr;
		}
		else if (connect_state_cb_ && !b_connect_)
		{
			connect_state_cb_(1);
		}

		if (!b_connect_)
		{
			SetConnection(true);
			OnStartSchedulePing();   //连接成功 开启心跳
		}

		if (receive_message_cb_)
		{
			receive_message_cb_(message);
		}
	}
}

void OwnDataChannel::OnConnectCloseCb()
{
	if (b_connect_)
	{
		if (connect_state_cb_)
		{
			connect_state_cb_(0);
		}
		OnRejoinConnect();
	}

	SetConnection(false);
	if (m_joinchannelCb_)
	{
		m_joinchannelCb_(s_channel_id_, -1);
		m_joinchannelCb_ = nullptr;
	}
}

void OwnDataChannel::OnRejoinConnect()
{
	LOG_O2O(L"OwnDataChannel::OnRejoinConnect...");
	WsService::GetInstance()->OnClose();
	StopHeartBeat();
	std::string ws_url = ssl_url_;
	reconnect_timer_.Cancel();
	auto task_cb = reconnect_timer_.ToWeakCallback([ws_url](){
		WsService::GetInstance()->OnConnect(ws_url);
	});
	qtbase::Post2RepeatedTask(kThreadWebSocket, task_cb, nbase::TimeDelta::FromSeconds(3));
}

void OwnDataChannel::OnStartSchedulePing()
{
	count_ping_pong_ = 0;
	reconnect_timer_.Cancel();
	long_connection_timer_.Cancel();
	auto task = long_connection_timer_.ToWeakCallback(nbase::Bind(&OwnDataChannel::PingWebSocket, this));
	qtbase::Post2RepeatedTask(kThreadMoreTaskHelper, task, nbase::TimeDelta::FromSeconds(2));
}

void OwnDataChannel::PingWebSocket()
{
	if (count_ping_pong_ >1)
	{
		//超过两次 认为已经断线 需要重连
		OnConnectCloseCb();
		return;
	}
	++count_ping_pong_;
#ifdef _DEBUG
	LOG_O2O(L"OwnDataChannel::OnSendPing...{0}") << count_ping_pong_;
#endif
	
	WsService::GetInstance()->OnSendPing();
}

void OwnDataChannel::StopHeartBeat()
{
	long_connection_timer_.Cancel();
}