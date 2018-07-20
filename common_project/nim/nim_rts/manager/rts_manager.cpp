#include "rts_manager.h"
#include "log\log.h"


namespace nim_comp
{
	RtsManager::RtsManager()
	{

	}

	RtsManager::~RtsManager()
	{

	}


	void RtsManager::OnStartRtsNotify(const std::string& session_id, int channel_type, const std::string& uid, const std::string& custom_info, const std::string& channel_id)
	{
		for (auto& it : start_cb_list_)
		{
			(*(it.second))(session_id, channel_type, uid, custom_info, channel_id);
		}
	}

	void RtsManager::OnAckNotifyCallback(const std::string& session_id, int channel_type, bool accept, const std::string& uid)
	{
		for (auto& it : ack_cb_list_)
		{
			(*(it.second))(session_id, channel_type, accept, uid);
		}
	}

	void RtsManager::OnSyncAckNotifyCallback(const std::string& session_id, int channel_type, bool accept, const std::string& json_ext)
	{
		for (auto& it : sync_ack_cb_list_)
		{
			(*(it.second))(session_id, channel_type, accept, json_ext);
		}
	}

	void RtsManager::OnConnectNotifyCallback(const std::string& session_id, int channel_type, int code, const std::string& json)
	{
		for (auto& it : connect_cb_list_)
		{
			(*(it.second))(session_id, channel_type, code, json);
		}
	}
	void RtsManager::OnMemberNotifyCallback(const std::string& session_id, int channel_type, const std::string& uid, int code)
	{
		for (auto& it : menber_cb_list_)
		{
			(*(it.second))(session_id, channel_type, uid, code);
		}
	}

	void RtsManager::OnHangupNotifyCallback(const std::string& session_id, const std::string& uid)
	{
		for (auto& it : hangup_cb_list_)
		{
			(*(it.second))(session_id, uid);
		}
	}

	void RtsManager::OnControlNotifyCallback(const std::string& session_id, const std::string& info, const std::string& uid)
	{
		for (auto& it : control_cb_list_)
		{
			(*(it.second))(session_id, info, uid);
		}
		
	}

	void RtsManager::OnRecDataCallback(const std::string& session_id, int channel_type, const std::string& uid, const std::string& data)
	{
		for (auto& it : rec_data_cb_list_)
		{
			(*(it.second))(session_id, channel_type, uid, data);
		}
	}

	UnregisterCallback RtsManager::RegStartChannelMsgCb(const nim::Rts::StartNotifyCallback& callback)
	{
		nim::Rts::StartNotifyCallback* new_callback = new nim::Rts::StartNotifyCallback(callback);
		int cb_id = (int)new_callback;
		start_cb_list_[cb_id].reset(new_callback);

		auto cb = ToWeakCallback([this, cb_id]() {
			start_cb_list_.erase(cb_id);
		});
		return cb;
	}

	UnregisterCallback RtsManager::RegAckCb(const nim::Rts::AckNotifyCallback& callback)
	{
		nim::Rts::AckNotifyCallback* new_callback = new nim::Rts::AckNotifyCallback(callback);
		int cb_id = (int)new_callback;
		ack_cb_list_[cb_id].reset(new_callback);

		auto cb = ToWeakCallback([this, cb_id]() {
			ack_cb_list_.erase(cb_id);
		});
		return cb;
	}

	UnregisterCallback RtsManager::RegSyncAckCb(const nim::Rts::SyncAckNotifyCallback& callback)
	{
		nim::Rts::SyncAckNotifyCallback* new_callback = new nim::Rts::SyncAckNotifyCallback(callback);
		int cb_id = (int)new_callback;
		sync_ack_cb_list_[cb_id].reset(new_callback);

		auto cb = ToWeakCallback([this, cb_id]() {
			sync_ack_cb_list_.erase(cb_id);
		});
		return cb;
	}

	UnregisterCallback RtsManager::RegConnectCb(const nim::Rts::ConnectNotifyCallback& callback)
	{
		nim::Rts::ConnectNotifyCallback* new_callback = new nim::Rts::ConnectNotifyCallback(callback);
		int cb_id = (int)new_callback;
		connect_cb_list_[cb_id].reset(new_callback);

		auto cb = ToWeakCallback([this, cb_id]() {
			connect_cb_list_.erase(cb_id);
		});
		return cb;
	}

	UnregisterCallback RtsManager::RegMemberCb(const nim::Rts::MemberNotifyCallback& callback)
	{
		nim::Rts::MemberNotifyCallback* new_callback = new nim::Rts::MemberNotifyCallback(callback);
		int cb_id = (int)new_callback;
		menber_cb_list_[cb_id].reset(new_callback);

		auto cb = ToWeakCallback([this, cb_id]() {
			menber_cb_list_.erase(cb_id);
		});
		return cb;
	}

	UnregisterCallback RtsManager::RegHangupCb(const nim::Rts::HangupNotifyCallback& callback)
	{
		nim::Rts::HangupNotifyCallback* new_callback = new nim::Rts::HangupNotifyCallback(callback);
		int cb_id = (int)new_callback;
		hangup_cb_list_[cb_id].reset(new_callback);

		auto cb = ToWeakCallback([this, cb_id]() {
			hangup_cb_list_.erase(cb_id);
		});
		return cb;
	}

	UnregisterCallback RtsManager::RegControlCb(const nim::Rts::ControlNotifyCallback& callback)
	{
		nim::Rts::ControlNotifyCallback* new_callback = new nim::Rts::ControlNotifyCallback(callback);
		int cb_id = (int)new_callback;
		control_cb_list_[cb_id].reset(new_callback);

		auto cb = ToWeakCallback([this, cb_id]() {
			control_cb_list_.erase(cb_id);
		});
		return cb;
	}

	UnregisterCallback RtsManager::RegRecDataCb(const nim::Rts::RecDataCallback& callback)
	{
		nim::Rts::RecDataCallback* new_callback = new nim::Rts::RecDataCallback(callback);
		int cb_id = (int)new_callback;
		rec_data_cb_list_[cb_id].reset(new_callback);

		auto cb = ToWeakCallback([this, cb_id]() {
			rec_data_cb_list_.erase(cb_id);
		});
		return cb;
	}

	void RtsManager::StartChannel(int channel_type, const std::string& uid, nim::RtsStartInfo info, const nim::Rts::StartChannelCallback& cb)
	{
		nim::Rts::StartChannel(channel_type, uid, info, cb);
	}

	void RtsManager::CreateConf(const std::string& name, const std::string& custom_info, const nim::Rts::CreateConfCallback& cb)
	{
		nim::Rts::CreateConf(name, custom_info, cb);
	}

	void RtsManager::JoinConf(const std::string& name, bool record, const nim::Rts::JoinConfCallback& cb)
	{
		nim::Rts::JoinConf(name, record, cb);
	}

	void RtsManager::Ack(const std::string& session_id, int channel_type, bool accept, bool data_record, bool audio_record, const nim::Rts::AckCallback& cb)
	{
		nim::Rts::Ack(session_id, channel_type, accept, data_record, audio_record, cb);
	}

	void RtsManager::Control(const std::string& session_id, const std::string& info, const nim::Rts::ControlCallback& cb)
	{
		nim::Rts::Control(session_id, info, cb);
	}

	void RtsManager::Hangup(const std::string& session_id, const nim::Rts::HangupCallback& cb)
	{
		nim::Rts::Hangup(session_id, cb);
	}

	void RtsManager::SendData(const std::string& session_id, int channel_type, const std::string& data, const std::string& uid /*= ""*/)
	{
		nim::Rts::SendData(session_id, channel_type, data, uid);
	}

}