#pragma once
#include <QtCore/QObject>
#include <QtWebSockets/QWebSocket>
#include <QtNetwork/QSslError>
#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QUrl>
#include "QTimer"
#include "base_data_channel.h"

class OwnDataChannel :public BaseDataChannel
{
	Q_OBJECT
public:
	OwnDataChannel(bool b_master);
	~OwnDataChannel();

	virtual void JoinChannel(std::string channel_id, JoinChannelCallback& cb);
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

public slots:
	void SlotConnected();
	void SlotTextMessageReceived(const QString &message);
	void SlotSslErrors(const QList<QSslError> &errors);
	void SlotDisConnected();
	void SlotReconnect();
	void SlotHeartBeat();
	void SlotError(QAbstractSocket::SocketError error);
	void SlotPong(quint64 elapsedTime, const QByteArray &payload);

private:
	void OnDisconnect();
private:
	QWebSocket m_webSocket_;
	QTimer m_reconnect_timer_;
	QTimer m_heart_beat_timer_;
	std::string ssl_url_;
	bool b_connect_;
	int ping_seq_;
	int pong_seq_;
	int heart_beat_interval_;

	bool b_init_join_;
	bool b_disconnect_tips_;      //认为已经断网提示
	int disconnect_count_;   //计数三次ping不通就认为网络断开  提示外部重连
	DataChannelConnectStateCb connect_state_cb_;
	DataChannelMessageCb receive_message_cb_;
	JoinChannelCallback m_joinchannelCb_;
};