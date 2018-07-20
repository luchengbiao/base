#pragma once
#include "base\callback\callback.h"
#include <string>
#include "channel_manager_global.h"
#include <QWidget>

class BaseDataChannel : public QObject, public nbase::SupportWeakCallback
{
	Q_OBJECT
public:
	BaseDataChannel(bool masetr, QObject* parent = 0);
	BaseDataChannel(){};
	~BaseDataChannel();

	virtual void JoinChannel(std::string channel_id, JoinChannelCallback& cb) = 0;
	virtual void ReJoinChannel(JoinChannelCallback& cb) = 0;
	virtual void LeaveChannel(std::string channel_id, std::string notified_account, LeaveChannelCallback& cb) = 0;
	virtual void SendChannelMessage(std::string channel_id, std::string data) = 0;
	virtual void SendP2PMessage(std::string account, uint32_t uid, std::string msg) = 0;

	virtual void SetConnection(bool b_connect) = 0;
	virtual void GetChannelInfo(ChannelInfo& info) = 0;
	virtual NetworkServiceType GetChannelType() = 0;
	virtual std::string GetSessionOrChannelID() = 0;
	virtual void SetSessionOrChannelID(std::string session_id) = 0;
	virtual void SetConnectStateCb(DataChannelConnectStateCb connect_cb)=0;
	virtual void SetChannelMessageCb(DataChannelMessageCb connect_cb) = 0;
public:
	bool b_master_;
};
