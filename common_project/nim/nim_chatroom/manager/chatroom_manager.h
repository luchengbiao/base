#pragma once
#include "base\callback\callback.h"
#include "base\memory\singleton.h"
#include <map>
#include <memory>
#include "base\util\auto_unregister.h"
#include "nim_sdk_manager\api\nim_chatroom_cpp.h"

namespace nim_comp
{
	typedef std::function<void(int error_code, const std::string& result)>	RequestEnterCallback;	/**<  获取聊天室登录信息回调*/

	class ChatroomManager : public nbase::SupportWeakCallback
	{
	public:
		SINGLETON_DEFINE(ChatroomManager);
		ChatroomManager();
		~ChatroomManager();

		// reg cb
		UnregisterCallback RegReceiveMsgCallback(const nim_chatroom::ChatRoom::ReceiveMsgCallback& callback);
		UnregisterCallback RegSendMsgAckCallback(const nim_chatroom::ChatRoom::SendMsgAckCallback& callback);
		UnregisterCallback RegEnterCallback(const nim_chatroom::ChatRoom::EnterCallback& callback);
		UnregisterCallback RegExitCallback(const nim_chatroom::ChatRoom::ExitCallback& callback);
		UnregisterCallback RegNotificationCallback(const nim_chatroom::ChatRoom::NotificationCallback& callback);
		UnregisterCallback RegLinkConditionCallback(const nim_chatroom::ChatRoom::LinkConditionCallback& callback);

		// cb
		void OnReceiveMsgCallback(int64_t room_id, const nim_chatroom::ChatRoomMessage& result);
		void OnSendMsgAckCallback(int64_t room_id, int error_code, const nim_chatroom::ChatRoomMessage& result);
		void OnEnterCallback(int64_t room_id, int step, int error_code,
								const nim_chatroom::ChatRoomInfo& info, const nim_chatroom::ChatRoomMemberInfo& my_info);
		void OnExitCallback(int64_t room_id, int error_code, int exit_reason);
		void OnNotificationCallback(int64_t room_id, const nim_chatroom::ChatRoomNotification& notification);
		void OnLinkConditionCallback(int64_t room_id, int condition);

		//
		bool Init(const std::string& app_install_dir, const std::string& json_extension = "");

		bool Enter(const int64_t room_id, const std::string& request_login_data, 
			const nim_chatroom::ChatRoomEnterInfo& info = nim_chatroom::ChatRoomEnterInfo(), const std::string& json_extension = "");

		void Exit(const int64_t room_id, const std::string& json_extension = "");

		nim_chatroom::NIMChatRoomLoginState GetLoginState(const int64_t room_id, const std::string& json_extension = "");

		void Cleanup(const std::string& json_extension = "");

		void SendMsg(const int64_t room_id, const std::string& json_msg, const std::string& json_extension = "");

		std::string CreateRoomMessage(int msg_type, const std::string& client_msg_id, const std::string& attach
			, const nim_chatroom::ChatRoomMessageSetting& msg_setting, int64_t timetag = 0);

		void GetMembersOnlineAsync(const int64_t room_id, const nim_chatroom::ChatRoomGetMembersParameters &parameters,
			const nim_chatroom::ChatRoom::GetMembersCallback &callback, const std::string& json_extension = "");

		void GetMessageHistoryOnlineAsync(const int64_t room_id, const nim_chatroom::ChatRoomGetMsgHistoryParameters &parameters,
			const nim_chatroom::ChatRoom::GetMsgHistoryCallback &callback, const std::string& json_extension = "");

		void SetMemberAttributeOnlineAsync(const int64_t room_id, const nim_chatroom::ChatRoomSetMemberAttributeParameters &parameters,
			const nim_chatroom::ChatRoom::SetMemberAttributeCallback &callback, const std::string& json_extension = "");

		void GetInfoAsync(const int64_t room_id, const nim_chatroom::ChatRoom::GetChatRoomInfoCallback& callback, const std::string &json_extension = "");

		void GetMemberInfoByIDsAsync(const int64_t room_id, const std::list<std::string>& ids,
			const nim_chatroom::ChatRoom::GetMembersCallback& callback, const std::string &json_extension = "");

		void KickMemberAsync(const int64_t room_id, const std::string& id, const std::string& notify_ext,
			const nim_chatroom::ChatRoom::KickMemberCallback &callback, const std::string &json_extension = "");

		void TempMuteMemberAsync(const int64_t room_id, const std::string& accid, const int64_t duration, bool need_notify
			, const std::string& notify_ext, const nim_chatroom::ChatRoom::TempMuteMemberCallback& callback, const std::string &json_extension = "");

		void UpdateRoomInfoAsync(const int64_t room_id, const nim_chatroom::ChatRoomInfo& info, bool need_notify
			, const std::string& notify_ext, const nim_chatroom::ChatRoom::UpdateRoomInfoCallback& callback, const std::string &json_extension = "");

		void UpdateMyRoomRoleAsync(const int64_t room_id, const nim_chatroom::ChatRoomMemberInfo& info, bool need_notify, const std::string& notify_ext,
			const nim_chatroom::ChatRoom::UpdateMyRoomRoleCallback& callback, const std::string &json_extension = "");

		void QueueOfferAsync(const int64_t room_id, const nim_chatroom::ChatRoomQueueElement& element
			, const nim_chatroom::ChatRoom::QueueOfferCallback& callback, const std::string &json_extension = "");

		void QueuePollAsync(const int64_t room_id, const std::string& element_key
			, const nim_chatroom::ChatRoom::QueuePollCallback& callback, const std::string &json_extension = "");

		void QueueListAsync(const int64_t room_id, const nim_chatroom::ChatRoom::QueueListCallback& callback, const std::string &json_extension = "");

		void QueueDropAsync(const int64_t room_id, const nim_chatroom::ChatRoom::QueueDropCallback& callback, const std::string &json_extension = "");

		void RequestEnterAsync(const __int64 room_id, const RequestEnterCallback &callback, const std::string& json_extension = "");

		void UnregChatroomCb();

	private:
		void InitChatroomCallback();

	private:
		std::map<int, std::unique_ptr<nim_chatroom::ChatRoom::ReceiveMsgCallback>> receive_msg_cb_list_;
		std::map<int, std::unique_ptr<nim_chatroom::ChatRoom::SendMsgAckCallback>> send_msg_ack_cb_list_;
		std::map<int, std::unique_ptr<nim_chatroom::ChatRoom::EnterCallback>> enter_cb_list_;
		std::map<int, std::unique_ptr<nim_chatroom::ChatRoom::ExitCallback>> exit_cb_list_;
		std::map<int, std::unique_ptr<nim_chatroom::ChatRoom::NotificationCallback>> notification_cb_list_;
		std::map<int, std::unique_ptr<nim_chatroom::ChatRoom::LinkConditionCallback>> link_condition_cb_list_;

	};
}