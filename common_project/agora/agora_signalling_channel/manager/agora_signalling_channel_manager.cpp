#include "agora_signalling_channel_manager.h"
#include "..\agora_signalling_sdk\manager\signalling_manager.h"

AGSignallingChannelManager::AGSignallingChannelManager()
{

}

AGSignallingChannelManager::~AGSignallingChannelManager()
{

}

UnregisterCallback AGSignallingChannelManager::RegChannelJoinCb(const SignallingChannelJoinCallback& callback)
{
	SignallingChannelJoinCallback* new_callback = new SignallingChannelJoinCallback(callback);
	int cb_id = (int)new_callback;
	channel_join_cb_list_[cb_id].reset(new_callback);

	auto cb = ToWeakCallback([this, cb_id]() {
		channel_join_cb_list_.erase(cb_id);
	});
	return cb;
}

UnregisterCallback AGSignallingChannelManager::RegUserJoinedCb(const SignallingUserJoinedCallback& callback)
{
	SignallingUserJoinedCallback* new_callback = new SignallingUserJoinedCallback(callback);
	int cb_id = (int)new_callback;
	user_join_cb_list_[cb_id].reset(new_callback);

	auto cb = ToWeakCallback([this, cb_id]() {
		user_join_cb_list_.erase(cb_id);
	});
	return cb;
}

UnregisterCallback AGSignallingChannelManager::RegUserLeavedCb(const SignallingUserLeavedCallback& callback)
{
	SignallingUserLeavedCallback* new_callback = new SignallingUserLeavedCallback(callback);
	int cb_id = (int)new_callback;
	user_leave_cb_list_[cb_id].reset(new_callback);

	auto cb = ToWeakCallback([this, cb_id]() {
		user_leave_cb_list_.erase(cb_id);
	});
	return cb;
}

UnregisterCallback AGSignallingChannelManager::RegChannelLeavedCb(const SignallingChannelLeavedCallback& callback)
{
	SignallingChannelLeavedCallback* new_callback = new SignallingChannelLeavedCallback(callback);
	int cb_id = (int)new_callback;
	channel_leave_cb_list_[cb_id].reset(new_callback);

	auto cb = ToWeakCallback([this, cb_id]() {
		channel_leave_cb_list_.erase(cb_id);
	});
	return cb;
}

//UnregisterCallback AGSignallingChannelManager::RegUserListCb(const SignallingUserListCallback& callback)
//{
//	SignallingUserListCallback* new_callback = new SignallingUserListCallback(callback);
//	int cb_id = (int)new_callback;
//	user_list_cb_list_[cb_id].reset(new_callback);
//
//	auto cb = ToWeakCallback([this, cb_id]() {
//		user_list_cb_list_.erase(cb_id);
//	});
//	return cb;
//}

UnregisterCallback AGSignallingChannelManager::RegMsgInstantRecCb(const MsgInstantRecCallback& callback)
{
	MsgInstantRecCallback* new_callback = new MsgInstantRecCallback(callback);
	int cb_id = (int)new_callback;
	msg_instant_rec_cb_list_[cb_id].reset(new_callback);

	auto cb = ToWeakCallback([this, cb_id]() {
		msg_instant_rec_cb_list_.erase(cb_id);
	});
	return cb;
}

UnregisterCallback AGSignallingChannelManager::RegMsgChannelRecCb(const MsgChannelRecCallback& callback)
{
	MsgChannelRecCallback* new_callback = new MsgChannelRecCallback(callback);
	int cb_id = (int)new_callback;
	msg_channel_rec_cb_list_[cb_id].reset(new_callback);

	auto cb = ToWeakCallback([this, cb_id]() {
		msg_channel_rec_cb_list_.erase(cb_id);
	});
	return cb;
}

UnregisterCallback AGSignallingChannelManager::RegChannelJoinFailCb(const SignallingChannelJoinFailCallback& callback)
{
	SignallingChannelJoinFailCallback* new_callback = new SignallingChannelJoinFailCallback(callback);
	int cb_id = (int)new_callback;
	channel_join_fail_cb_list_[cb_id].reset(new_callback);

	auto cb = ToWeakCallback([this, cb_id]() {
		channel_join_fail_cb_list_.erase(cb_id);
	});
	return cb;
}

UnregisterCallback AGSignallingChannelManager::RegMsgSendErrorCb(const MsgSendErrorCallback& callback)
{
	MsgSendErrorCallback* new_callback = new MsgSendErrorCallback(callback);
	int cb_id = (int)new_callback;
	msg_send_error_cb_list_[cb_id].reset(new_callback);

	auto cb = ToWeakCallback([this, cb_id]() {
		msg_send_error_cb_list_.erase(cb_id);
	});
	return cb;
}

UnregisterCallback AGSignallingChannelManager::RegMsgSendSuccessCb(const MsgSendSuccessCallback& callback)
{
	MsgSendSuccessCallback* new_callback = new MsgSendSuccessCallback(callback);
	int cb_id = (int)new_callback;
	msg_send_success_cb_list_[cb_id].reset(new_callback);

	auto cb = ToWeakCallback([this, cb_id]() {
		msg_send_success_cb_list_.erase(cb_id);
	});
	return cb;
}

void AGSignallingChannelManager::OnChannelJoined(std::string channel_id)
{
	for (auto& it : channel_join_cb_list_)
	{
		(*(it.second))(channel_id);
	}
}

void AGSignallingChannelManager::OnChannelUserJoined(std::string account, uint32_t uid)
{
	for (auto& it : user_join_cb_list_)
	{
		(*(it.second))(account, uid);
	}
}

void AGSignallingChannelManager::OnChannelUserLeaved(std::string account, uint32_t uid)
{
	for (auto& it : user_leave_cb_list_)
	{
		(*(it.second))(account, uid);
	}
}

void AGSignallingChannelManager::OnChannelLeaved(std::string channel_id, int e_code)
{
	for (auto& it : channel_leave_cb_list_)
	{
		(*(it.second))(channel_id, e_code);
	}
}

//void AGSignallingChannelManager::OnChannelUserList(std::vector<std::string> accounts, std::vector<uint32_t> uids)
//{
//	for (auto& it : user_list_cb_list_)
//	{
//		(*(it.second))(accounts, uids);
//	}
//}

void AGSignallingChannelManager::OnMessageInstantReceive(std::string account, uint32_t uid, std::string msg)
{
	for (auto& it : msg_instant_rec_cb_list_)
	{
		(*(it.second))(account, uid, msg);
	}
}

void AGSignallingChannelManager::OnMessageChannelReceive(std::string channel_id, std::string account, uint32_t uid, std::string msg)
{
	for (auto& it : msg_channel_rec_cb_list_)
	{
		(*(it.second))(channel_id, account, uid, msg);
	}
}

void AGSignallingChannelManager::OnChannelJoinFailed(std::string channel_id, int ecode)
{
	for (auto& it : channel_join_fail_cb_list_)
	{
		(*(it.second))(channel_id, ecode);
	}
}

void AGSignallingChannelManager::OnMessageSendError(std::string message_id, int ecode)
{
	for (auto& it : msg_send_error_cb_list_)
	{
		(*(it.second))(message_id, ecode);
	}
}

void AGSignallingChannelManager::OnMessageSendSuccess(std::string message_id)
{
	for (auto& it : msg_send_success_cb_list_)
	{
		(*(it.second))(message_id);
	}
}

void AGSignallingChannelManager::ChannelJoin(std::string channel_id)
{
	AGSignallingManager::GetAGSignalling()->ChannelJoin(channel_id);
}

void AGSignallingChannelManager::ChannelLeave(std::string channel_id)
{
	AGSignallingManager::GetAGSignalling()->ChannelLeave(channel_id);
}

void AGSignallingChannelManager::ChannelQueryUserNum(std::string channel_id)
{
	AGSignallingManager::GetAGSignalling()->ChannelQueryUserNum(channel_id);
}

void AGSignallingChannelManager::MessageAppSend(std::string msg, std::string msg_id)
{
	AGSignallingManager::GetAGSignalling()->MessageAppSend(msg, msg_id);
}

void AGSignallingChannelManager::MessageInstantSend(std::string account, uint32_t uid, std::string msg, std::string msg_id)
{
	AGSignallingManager::GetAGSignalling()->MessageInstantSend(account, uid, msg, msg_id);
}

void AGSignallingChannelManager::MessageChannelSend(std::string channel_id, std::string msg, std::string msg_id)
{
	AGSignallingManager::GetAGSignalling()->MessageChannelSend(channel_id, msg, msg_id);
}

void AGSignallingChannelManager::MessageChannelSendFast(std::string channel_id, std::string msg, std::string msg_id)
{
	AGSignallingManager::GetAGSignalling()->MessageChannelSendFast(channel_id, msg, msg_id);
}

void AGSignallingChannelManager::MessagePushSend(std::string account, uint32_t uid, std::string msg, std::string msg_id)
{
	AGSignallingManager::GetAGSignalling()->MessagePushSend(account, uid, msg, msg_id);
}

void AGSignallingChannelManager::MessageChatSend(std::string account, uint32_t uid, std::string msg, std::string msg_id)
{
	AGSignallingManager::GetAGSignalling()->MessageChatSend(account, uid, msg, msg_id);
}

void AGSignallingChannelManager::MessageDTMFSend(uint32_t uid, std::string msg, std::string msg_id)
{
	AGSignallingManager::GetAGSignalling()->MessageDTMFSend(uid, msg, msg_id);
}

