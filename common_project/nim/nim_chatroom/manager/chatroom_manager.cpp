#include "chatroom_manager.h"
#include "nim_sdk_manager\api\nim_chatroom_cpp.h"
#include "callback\chatroom_callback.h"
#include "nim_sdk_manager\api\nim_chatroom_cpp.h"
#include "nim\nim_chatroom_c_sdk\include\nim_chatroom_def.h"
#include "nim_sdk_manager\api\nim_cpp_plugin_in.h"

namespace nim_comp
{
	ChatroomManager::ChatroomManager()
	{
		InitChatroomCallback();
	}

	ChatroomManager::~ChatroomManager()
	{

	}

	void ChatroomManager::InitChatroomCallback()
	{
		nim_chatroom::ChatRoom::RegReceiveMsgCb(nbase::Bind(&ChatroomCallback::OnReceiveMsgCallback, std::placeholders::_1, std::placeholders::_2));
		nim_chatroom::ChatRoom::RegSendMsgAckCb(nbase::Bind(&ChatroomCallback::OnSendMsgCallback, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
		nim_chatroom::ChatRoom::RegEnterCb(nbase::Bind(&ChatroomCallback::OnEnterCallback, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5));
		nim_chatroom::ChatRoom::RegExitCb(nbase::Bind(&ChatroomCallback::OnExitCallback, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
		nim_chatroom::ChatRoom::RegNotificationCb(nbase::Bind(&ChatroomCallback::OnNotificationCallback, std::placeholders::_1, std::placeholders::_2));
		nim_chatroom::ChatRoom::RegLinkConditionCb(nbase::Bind(&ChatroomCallback::OnRegLinkConditionCallback, std::placeholders::_1, std::placeholders::_2));
	}

	UnregisterCallback ChatroomManager::RegReceiveMsgCallback(const nim_chatroom::ChatRoom::ReceiveMsgCallback& callback)
	{
		nim_chatroom::ChatRoom::ReceiveMsgCallback* new_callback = new nim_chatroom::ChatRoom::ReceiveMsgCallback(callback);
		int cb_id = (int)new_callback;

		receive_msg_cb_list_[cb_id].reset(new_callback);
		auto cb = ToWeakCallback([this, cb_id]() {
			receive_msg_cb_list_.erase(cb_id);
		});
		return cb;
	}

	UnregisterCallback ChatroomManager::RegSendMsgAckCallback(const nim_chatroom::ChatRoom::SendMsgAckCallback& callback)
	{
		nim_chatroom::ChatRoom::SendMsgAckCallback* new_callback = new nim_chatroom::ChatRoom::SendMsgAckCallback(callback);
		int cb_id = (int)new_callback;

		send_msg_ack_cb_list_[cb_id].reset(new_callback);
		auto cb = ToWeakCallback([this, cb_id]() {
			send_msg_ack_cb_list_.erase(cb_id);
		});
		return cb;
	}

	UnregisterCallback ChatroomManager::RegEnterCallback(const nim_chatroom::ChatRoom::EnterCallback& callback)
	{
		nim_chatroom::ChatRoom::EnterCallback* new_callback = new nim_chatroom::ChatRoom::EnterCallback(callback);
		int cb_id = (int)new_callback;

		enter_cb_list_[cb_id].reset(new_callback);
		auto cb = ToWeakCallback([this, cb_id]() {
			enter_cb_list_.erase(cb_id);
		});
		return cb;
	}

	UnregisterCallback ChatroomManager::RegExitCallback(const nim_chatroom::ChatRoom::ExitCallback& callback)
	{
		nim_chatroom::ChatRoom::ExitCallback* new_callback = new nim_chatroom::ChatRoom::ExitCallback(callback);
		int cb_id = (int)new_callback;

		exit_cb_list_[cb_id].reset(new_callback);
		auto cb = ToWeakCallback([this, cb_id]() {
			exit_cb_list_.erase(cb_id);
		});
		return cb;
	}

	UnregisterCallback ChatroomManager::RegNotificationCallback(const nim_chatroom::ChatRoom::NotificationCallback& callback)
	{
		nim_chatroom::ChatRoom::NotificationCallback* new_callback = new nim_chatroom::ChatRoom::NotificationCallback(callback);
		int cb_id = (int)new_callback;

		notification_cb_list_[cb_id].reset(new_callback);
		auto cb = ToWeakCallback([this, cb_id]() {
			notification_cb_list_.erase(cb_id);
		});
		return cb;
	}

	UnregisterCallback ChatroomManager::RegLinkConditionCallback(const nim_chatroom::ChatRoom::LinkConditionCallback& callback)
	{
		nim_chatroom::ChatRoom::LinkConditionCallback* new_callback = new nim_chatroom::ChatRoom::LinkConditionCallback(callback);
		int cb_id = (int)new_callback;

		link_condition_cb_list_[cb_id].reset(new_callback);
		auto cb = ToWeakCallback([this, cb_id]() {
			link_condition_cb_list_.erase(cb_id);
		});
		return cb;
	}

	void ChatroomManager::OnReceiveMsgCallback(int64_t room_id, const nim_chatroom::ChatRoomMessage& result)
	{
		for (auto& it : receive_msg_cb_list_)
		{
			(*(it.second))(room_id, result);
		}
	}

	void ChatroomManager::OnSendMsgAckCallback(int64_t room_id, int error_code, const nim_chatroom::ChatRoomMessage& result)
	{
		for (auto& it : send_msg_ack_cb_list_)
		{
			(*(it.second))(room_id, error_code, result);
		}
	}

	void ChatroomManager::OnEnterCallback(int64_t room_id, int step, int error_code, 
					const nim_chatroom::ChatRoomInfo& info, const nim_chatroom::ChatRoomMemberInfo& my_info)
	{
		for (auto& it : enter_cb_list_)
		{
			(*(it.second))(room_id, (nim_chatroom::NIMChatRoomEnterStep)step, error_code, info, my_info);
		}
	}

	void ChatroomManager::OnExitCallback(int64_t room_id, int error_code, int exit_reason)
	{
		for (auto& it : exit_cb_list_)
		{
			(*(it.second))(room_id, error_code, (nim_chatroom::NIMChatRoomExitReason)exit_reason);
		}
	}

	void ChatroomManager::OnNotificationCallback(int64_t room_id, const nim_chatroom::ChatRoomNotification& notification)
	{
		for (auto& it : notification_cb_list_)
		{
			(*(it.second))(room_id, notification);
		}
	}

	void ChatroomManager::OnLinkConditionCallback(int64_t room_id, int condition)
	{
		for (auto& it : link_condition_cb_list_)
		{
			(*(it.second))(room_id, (nim_chatroom::NIMChatRoomLinkCondition)condition);
		}
	}

	bool ChatroomManager::Init(const std::string& app_install_dir, const std::string& json_extension /*= ""*/)
	{
		return nim_chatroom::ChatRoom::Init(app_install_dir, json_extension);
	}

	bool ChatroomManager::Enter(const int64_t room_id, const std::string& request_login_data, 
		const nim_chatroom::ChatRoomEnterInfo& info /*= ChatRoomEnterInfo()*/, const std::string& json_extension /*= ""*/)
	{
		return nim_chatroom::ChatRoom::Enter(room_id, request_login_data, info, json_extension);
	}

	void ChatroomManager::Exit(const int64_t room_id, const std::string& json_extension /*= ""*/)
	{
		nim_chatroom::ChatRoom::Exit(room_id, json_extension);
	}

	nim_chatroom::NIMChatRoomLoginState ChatroomManager::GetLoginState(const int64_t room_id, const std::string& json_extension /*= ""*/)
	{
		return nim_chatroom::ChatRoom::GetLoginState(room_id, json_extension);
	}

	void ChatroomManager::Cleanup(const std::string& json_extension /*= ""*/)
	{
		nim_chatroom::ChatRoom::Cleanup(json_extension);
	}

	void ChatroomManager::SendMsg(const int64_t room_id, const std::string& json_msg, const std::string& json_extension /*= ""*/)
	{
		nim_chatroom::ChatRoom::SendMsg(room_id, json_msg, json_extension);
	}

	std::string ChatroomManager::CreateRoomMessage(int msg_type, const std::string& client_msg_id, 
		const std::string& attach, const nim_chatroom::ChatRoomMessageSetting& msg_setting, int64_t timetag /*= 0*/)
	{
		return nim_chatroom::ChatRoom::CreateRoomMessage((nim_chatroom::NIMChatRoomMsgType)msg_type, client_msg_id, attach, msg_setting, timetag);
	}

	void ChatroomManager::GetMembersOnlineAsync(const int64_t room_id, const nim_chatroom::ChatRoomGetMembersParameters &parameters,
		const nim_chatroom::ChatRoom::GetMembersCallback &callback, const std::string& json_extension /*= ""*/)
	{
		nim_chatroom::ChatRoom::GetMembersOnlineAsync(room_id, parameters, callback, json_extension);
	}

	void ChatroomManager::GetMessageHistoryOnlineAsync(const int64_t room_id, const nim_chatroom::ChatRoomGetMsgHistoryParameters &parameters,
		const nim_chatroom::ChatRoom::GetMsgHistoryCallback &callback, const std::string& json_extension /*= ""*/)
	{
		nim_chatroom::ChatRoom::GetMessageHistoryOnlineAsync(room_id, parameters, callback, json_extension);
	}

	void ChatroomManager::SetMemberAttributeOnlineAsync(const int64_t room_id, const nim_chatroom::ChatRoomSetMemberAttributeParameters &parameters,
		const nim_chatroom::ChatRoom::SetMemberAttributeCallback &callback, const std::string& json_extension /*= ""*/)
	{
		nim_chatroom::ChatRoom::SetMemberAttributeOnlineAsync(room_id, parameters, callback, json_extension);
	}

	void ChatroomManager::GetInfoAsync(const int64_t room_id, const nim_chatroom::ChatRoom::GetChatRoomInfoCallback& callback, const std::string &json_extension /*= ""*/)
	{
		nim_chatroom::ChatRoom::GetInfoAsync(room_id, callback, json_extension);
	}

	void ChatroomManager::GetMemberInfoByIDsAsync(const int64_t room_id, const std::list<std::string>& ids, 
		const nim_chatroom::ChatRoom::GetMembersCallback& callback, const std::string &json_extension /*= ""*/)
	{
		nim_chatroom::ChatRoom::GetMemberInfoByIDsAsync(room_id, ids, callback, json_extension);
	}

	void ChatroomManager::KickMemberAsync(const int64_t room_id, const std::string& id, const std::string& notify_ext, 
		const nim_chatroom::ChatRoom::KickMemberCallback &callback, const std::string &json_extension /*= ""*/)
	{
		nim_chatroom::ChatRoom::KickMemberAsync(room_id, id, notify_ext, callback, json_extension);
	}

	void ChatroomManager::TempMuteMemberAsync(const int64_t room_id, const std::string& accid, const int64_t duration, 
		bool need_notify, const std::string& notify_ext, const nim_chatroom::ChatRoom::TempMuteMemberCallback& callback, const std::string &json_extension /*= ""*/)
	{
		nim_chatroom::ChatRoom::TempMuteMemberAsync(room_id, accid, duration, need_notify, notify_ext, callback, json_extension);
	}

	void ChatroomManager::UpdateRoomInfoAsync(const int64_t room_id, const nim_chatroom::ChatRoomInfo& info, bool need_notify, const std::string& notify_ext,
		const nim_chatroom::ChatRoom::UpdateRoomInfoCallback& callback, const std::string &json_extension /*= ""*/)
	{
		nim_chatroom::ChatRoom::UpdateRoomInfoAsync(room_id, info, need_notify, notify_ext, callback, json_extension);
	}

	void ChatroomManager::UpdateMyRoomRoleAsync(const int64_t room_id, const nim_chatroom::ChatRoomMemberInfo& info, bool need_notify, const std::string& notify_ext,
		const nim_chatroom::ChatRoom::UpdateMyRoomRoleCallback& callback, const std::string &json_extension /*= ""*/)
	{
		nim_chatroom::ChatRoom::UpdateMyRoomRoleAsync(room_id, info, need_notify, notify_ext, callback, json_extension);
	}

	void ChatroomManager::QueueOfferAsync(const int64_t room_id, const nim_chatroom::ChatRoomQueueElement& element,
		const nim_chatroom::ChatRoom::QueueOfferCallback& callback, const std::string &json_extension /*= ""*/)
	{
		nim_chatroom::ChatRoom::QueueOfferAsync(room_id, element, callback, json_extension);
	}

	void ChatroomManager::QueuePollAsync(const int64_t room_id, const std::string& element_key, 
		const nim_chatroom::ChatRoom::QueuePollCallback& callback, const std::string &json_extension /*= ""*/)
	{
		nim_chatroom::ChatRoom::QueuePollAsync(room_id, element_key, callback, json_extension);
	}

	void ChatroomManager::QueueListAsync(const int64_t room_id, const nim_chatroom::ChatRoom::QueueListCallback& callback, const std::string &json_extension /*= ""*/)
	{
		nim_chatroom::ChatRoom::QueueListAsync(room_id, callback, json_extension);
	}

	void ChatroomManager::QueueDropAsync(const int64_t room_id, const nim_chatroom::ChatRoom::QueueDropCallback& callback, const std::string &json_extension /*= ""*/)
	{
		nim_chatroom::ChatRoom::QueueDropAsync(room_id, callback, json_extension);
	}

	void ChatroomManager::UnregChatroomCb()
	{
		nim_chatroom::ChatRoom::UnregChatroomCb();
	}

	void ChatroomManager::RequestEnterAsync(const __int64 room_id, const RequestEnterCallback &callback, const std::string& json_extension /*= ""*/)
	{
		nim::PluginIn::ChatRoomRequestEnterAsync(room_id, callback, json_extension);
	}

}