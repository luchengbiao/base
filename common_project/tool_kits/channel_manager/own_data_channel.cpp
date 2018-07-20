#include "own_data_channel.h"
#include "qthread_manager\closure.h"

OwnDataChannel::OwnDataChannel(bool b_master) :BaseDataChannel(b_master)
{
	b_init_join_ = false;
	b_disconnect_tips_ = false;
	disconnect_count_ = 0;
	b_connect_ = false;
	ssl_url_ = "";
	heart_beat_interval_ = 500;
}

OwnDataChannel::~OwnDataChannel()
{
	OnDisconnect();
}

void OwnDataChannel::JoinChannel(std::string channel_id, JoinChannelCallback& cb)
{
	m_joinchannelCb_ = cb;
	QObject::connect(&m_webSocket_, SIGNAL(connected()), this, SLOT(SlotConnected()));
	QObject::connect(&m_webSocket_, SIGNAL(sslErrors(const QList<QSslError>&)), this, SLOT(SlotSslErrors(const QList<QSslError>&)));
	QObject::connect(&m_webSocket_, SIGNAL(disconnected()), this, SLOT(SlotDisConnected()));
	QObject::connect(&m_webSocket_, SIGNAL(textMessageReceived(const QString&)), this, SLOT(SlotTextMessageReceived(const QString&)));
	QObject::connect(&m_webSocket_, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(SlotError(QAbstractSocket::SocketError)));
	QObject::connect(&m_webSocket_, SIGNAL(pong(quint64, const QByteArray &)), this, SLOT(SlotPong(quint64, const QByteArray &)));
	QObject::connect(&m_reconnect_timer_, SIGNAL(timeout()), this, SLOT(SlotReconnect()));
	QObject::connect(&m_heart_beat_timer_, SIGNAL(timeout()), this, SLOT(SlotHeartBeat()));
	ssl_url_ = channel_id;
	heart_beat_interval_ = 500;
	qDebug() << QString::fromStdString(ssl_url_);
	m_webSocket_.open(QUrl(QString::fromStdString(channel_id)));
	
	auto join_check = ToWeakCallback([=](){
		OnDisconnect();
		if (m_joinchannelCb_)
		{
			m_joinchannelCb_("", -1);
		}
	});
	qtbase::Post2DelayedTask(kThreadUIHelper, join_check, nbase::TimeDelta::FromSeconds(6));
}

void OwnDataChannel::ReJoinChannel(JoinChannelCallback& cb)
{

}

void OwnDataChannel::LeaveChannel(std::string channel_id, std::string notified_account, LeaveChannelCallback& cb)
{
	OnDisconnect();
}

void OwnDataChannel::SendChannelMessage(std::string channel_id, std::string data)
{
	if (b_connect_)
	{
		int len = m_webSocket_.sendTextMessage(QString::fromStdString(data));
	}
}

void OwnDataChannel::SendP2PMessage(std::string account, uint32_t uid, std::string msg)
{

}

void OwnDataChannel::SetConnection(bool b_connect)
{

}

void OwnDataChannel::GetChannelInfo(ChannelInfo& info)
{

}

NetworkServiceType OwnDataChannel::GetChannelType()
{
	return SWITCH_NETWORK_OWN;
}

std::string OwnDataChannel::GetSessionOrChannelID()
{
	return "";
}

void OwnDataChannel::SetSessionOrChannelID(std::string session_id)
{

}

void OwnDataChannel::SlotConnected()
{
	qDebug() << "websocket connect";
	b_init_join_ = true;
	m_reconnect_timer_.stop();
	b_connect_ = true;
	if (b_disconnect_tips_)
	{
		b_disconnect_tips_ = false;
		disconnect_count_ = 0;
		connect_state_cb_(1);  //提示断线
	}
	ping_seq_ = 0;
	m_heart_beat_timer_.start(heart_beat_interval_); // 每隔heart_beat_interval_(ms)给服务器发送一次心跳

	if (m_joinchannelCb_)
	{
		m_joinchannelCb_("", 0);
	}
}

void OwnDataChannel::SlotTextMessageReceived(const QString &message)
{
	receive_message_cb_(message.toStdString());
}

void OwnDataChannel::SlotSslErrors(const QList<QSslError> &errors)
{
	Q_UNUSED(errors);

	// WARNING: Never ignore SSL errors in production code.
	// The proper way to handle self-signed certificates is to add a custom root
	// to the CA store.

	m_webSocket_.ignoreSslErrors();
}

void OwnDataChannel::SlotDisConnected()
{
	qDebug() << "websocket disconnect";
	if (!b_init_join_)
	{
		OnDisconnect();
	}
	else
	{
		b_connect_ = false;
		m_reconnect_timer_.start(500);
		m_heart_beat_timer_.stop();
	}
}

void OwnDataChannel::SlotReconnect()
{
	qDebug() << "websocket start reconnect";
	m_webSocket_.abort();
	m_webSocket_.open(QUrl(QString::fromStdString(ssl_url_)));
}

void OwnDataChannel::SlotHeartBeat()
{
	if (ping_seq_ != pong_seq_)
	{
		qDebug() << "disconnect";
		b_connect_ = false;
		++disconnect_count_;
		if (disconnect_count_ >5)
		{
			if (!b_disconnect_tips_)
			{
				b_disconnect_tips_ = true;
				connect_state_cb_(0);  //提示断线
			}
		}
	}
	else
	{
		qDebug() << "connect";
		b_connect_ = true;
		if (b_disconnect_tips_)
		{
			b_disconnect_tips_ = false;
			disconnect_count_ = 0;
			connect_state_cb_(1);  //提示断线
		}
	}
	qDebug() << "ping";
	QString seq_str = QString::number(++ping_seq_, 10);
	m_webSocket_.ping(seq_str.toLatin1());
}

void OwnDataChannel::SlotError(QAbstractSocket::SocketError error)
{

}

void OwnDataChannel::SlotPong(quint64 elapsedTime, const QByteArray &payload)
{
	qDebug() << "pong: " << elapsedTime << ", " << payload;
	QString seq_str(payload);
	pong_seq_ = seq_str.toInt();
}

void OwnDataChannel::OnDisconnect()
{
	b_connect_ = false;
	m_reconnect_timer_.stop();
	m_heart_beat_timer_.stop();
	m_webSocket_.close();
}

void OwnDataChannel::SetConnectStateCb(DataChannelConnectStateCb connect_cb)
{
	connect_state_cb_ = connect_cb;
}

void OwnDataChannel::SetChannelMessageCb(DataChannelMessageCb msg_cb)
{
	receive_message_cb_ = msg_cb;
}
