#pragma once
#include "nim_sdk_manager\api\nim_cpp_rts.h"
#include "base\callback\callback.h"
#include "base\memory\singleton.h"
#include <string>
#include "base\util\auto_unregister.h"
#include <memory>

namespace nim_comp
{
class RtsManager : public nbase::SupportWeakCallback
{
public:
	SINGLETON_DEFINE(RtsManager);
	RtsManager();
	~RtsManager();

	// reg cb
	UnregisterCallback RegStartChannelMsgCb(const nim::Rts::StartNotifyCallback& callback);
	UnregisterCallback RegAckCb(const nim::Rts::AckNotifyCallback& callback);
	UnregisterCallback RegSyncAckCb(const nim::Rts::SyncAckNotifyCallback& callback);
	UnregisterCallback RegConnectCb(const nim::Rts::ConnectNotifyCallback& callback);
	UnregisterCallback RegMemberCb(const nim::Rts::MemberNotifyCallback& callback);
	UnregisterCallback RegHangupCb(const nim::Rts::HangupNotifyCallback& callback);
	UnregisterCallback RegControlCb(const nim::Rts::ControlNotifyCallback& callback);
	UnregisterCallback RegRecDataCb(const nim::Rts::RecDataCallback& callback);

	// cb
	void OnStartRtsNotify(const std::string& session_id, int channel_type, const std::string& uid, const std::string& custom_info, const std::string& channel_id);
	void OnAckNotifyCallback(const std::string& session_id, int channel_type, bool accept, const std::string& uid);
	void OnSyncAckNotifyCallback(const std::string& session_id, int channel_type, bool accept, const std::string& json_ext);
	void OnConnectNotifyCallback(const std::string& session_id, int channel_type, int code, const std::string& json);
	void OnMemberNotifyCallback(const std::string& session_id, int channel_type, const std::string& uid, int code);
	void OnHangupNotifyCallback(const std::string& session_id, const std::string& uid);
	void OnControlNotifyCallback(const std::string& session_id, const std::string& info, const std::string& uid);
	void OnRecDataCallback(const std::string& session_id, int channel_type, const std::string& uid, const std::string& data);

	// 
	void StartChannel(int channel_type, const std::string& uid, nim::RtsStartInfo info, const nim::Rts::StartChannelCallback& cb);
	void CreateConf(const std::string& name, const std::string& custom_info, const nim::Rts::CreateConfCallback& cb);
	void JoinConf(const std::string& name, bool record, const nim::Rts::JoinConfCallback& cb);
	void Ack(const std::string& session_id, int channel_type, bool accept, bool data_record, bool audio_record, const nim::Rts::AckCallback& cb);
	void Control(const std::string& session_id, const std::string& info, const nim::Rts::ControlCallback& cb);
	void Hangup(const std::string& session_id, const nim::Rts::HangupCallback& cb);
	void SendData(const std::string& session_id, int channel_type, const std::string& data, const std::string& uid = "");

private:
	std::map<int, std::unique_ptr<nim::Rts::StartNotifyCallback>> start_cb_list_;
	std::map<int, std::unique_ptr<nim::Rts::AckNotifyCallback>> ack_cb_list_;
	std::map<int, std::unique_ptr<nim::Rts::SyncAckNotifyCallback>> sync_ack_cb_list_;
	std::map<int, std::unique_ptr<nim::Rts::ConnectNotifyCallback>> connect_cb_list_;
	std::map<int, std::unique_ptr<nim::Rts::MemberNotifyCallback>> menber_cb_list_;
	std::map<int, std::unique_ptr<nim::Rts::HangupNotifyCallback>> hangup_cb_list_;
	std::map<int, std::unique_ptr<nim::Rts::ControlNotifyCallback>> control_cb_list_;
	std::map<int, std::unique_ptr<nim::Rts::RecDataCallback>> rec_data_cb_list_;

};
}