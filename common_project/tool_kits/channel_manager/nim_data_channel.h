#pragma once
#include "base_data_channel.h"
#include "nim_sdk_manager\helper\nim_doc_trans_helper.h"

class NimDataChannel : public BaseDataChannel
{
	Q_OBJECT
public:
	NimDataChannel(bool master);
	~NimDataChannel();

	virtual void JoinChannel(std::string channel_id, JoinChannelCallback& cb, std::string url = "");
	virtual void ReJoinChannel(JoinChannelCallback& cb);
	virtual void LeaveChannel(std::string session_id, std::string notified_account, LeaveChannelCallback& cb);
	virtual void SendChannelMessage(std::string channel_id, std::string data);
	virtual void SendP2PMessage(std::string session_id, uint32_t uid, std::string msg);

	virtual void SetConnection(bool b_connect);
	virtual void GetChannelInfo(ChannelInfo& info);
	virtual NetworkServiceType GetChannelType();
	virtual std::string GetSessionOrChannelID();
	virtual void SetSessionOrChannelID(std::string session_id){ rts_session_id_ = session_id; };
	virtual void SetConnectStateCb(DataChannelConnectStateCb connect_cb){}
	virtual void SetChannelMessageCb(DataChannelMessageCb msg_cb){}

private:
	void CreateChannelCb(nim::NIMResCode res_code);
	void JoinChannelCb(nim::NIMResCode res_code, const std::string& session_id, __int64 channel_id, const std::string& custom_info);
	void JoinChannelPrivate(std::string channel_id);
	void EndChannel();

private:
	bool rts_login_;
	std::string rts_session_id_;
	std::string course_channel_id_;
	bool b_connection_;
	std::string s_channel_id_;
	JoinChannelCallback m_joinchannelCb_;
};
