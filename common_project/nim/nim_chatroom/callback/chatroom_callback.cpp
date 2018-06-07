#include "chatroom_callback.h"
#include "log_manager\log.h"
#include "nim_sdk_manager\api\nim_chatroom_cpp.h"
#include "base\callback\callback.h"
#include "qthread_manager\closure.h"
#include "manager\chatroom_manager.h"

namespace nim_comp
{

void ChatroomCallback::OnReceiveMsgCallback(__int64 room_id, const nim_chatroom::ChatRoomMessage& result)
{
	QLOG_PRO(L"Chatroom:OnReceiveMsgCallback: {0} from client: {1}") << result.client_msg_id_ << result.from_client_type_;
	
	auto task = [=](){
		ChatroomManager::GetInstance()->OnReceiveMsgCallback(room_id, result);
	};

	qtbase::Post2Task(kThreadUIHelper, task);
}


void ChatroomCallback::OnSendMsgCallback(__int64 room_id, int error_code, const nim_chatroom::ChatRoomMessage& result)
{
	QLOG_APP(L"Chatroom:OnSendMsgCallback: id={0} msg_id={1} code={2}") << result.room_id_ << result.client_msg_id_ << error_code;

	auto task = [=](){
		ChatroomManager::GetInstance()->OnSendMsgAckCallback(room_id, error_code, result);
	};

	qtbase::Post2Task(kThreadUIHelper, task);
}


void ChatroomCallback::OnEnterCallback(__int64 room_id, int step, int error_code, 
		const nim_chatroom::ChatRoomInfo& info, const nim_chatroom::ChatRoomMemberInfo& my_info)
{
	QLOG_APP(L"Chatroom:OnEnterCallback: id={0} step={1} code={2}") << room_id << step << error_code;

	if (step != nim_chatroom::kNIMChatRoomEnterStepRoomAuthOver) {
		return;
	}

	auto task = [=](){
		ChatroomManager::GetInstance()->OnEnterCallback(room_id, step, error_code, info, my_info);
	};

	qtbase::Post2Task(kThreadUIHelper, task);
}

void ChatroomCallback::OnExitCallback(__int64 room_id, int error_code, nim_chatroom::NIMChatRoomExitReason exit_reason)
{
	QLOG_APP(L"Chatroom:OnExitCallback: id={0} code={1}") << room_id << error_code;

	auto task = [=](){
		ChatroomManager::GetInstance()->OnExitCallback(room_id, error_code, exit_reason);
	};

	qtbase::Post2Task(kThreadUIHelper, task);
}

void ChatroomCallback::OnNotificationCallback(__int64 room_id, const nim_chatroom::ChatRoomNotification& notification)
{
	QLOG_APP(L"Chatroom:OnNotificationCallback: id={0}") << room_id;

	auto task = [=](){
		ChatroomManager::GetInstance()->OnNotificationCallback(room_id, notification);
	};
	qtbase::Post2Task(kThreadUIHelper, task);
}

void ChatroomCallback::OnRegLinkConditionCallback(__int64 room_id, int condition)
{
	QLOG_APP(L"Chatroom:OnRegLinkConditionCallback: id={0} condition={1}") << room_id << condition;

	auto task = [=](){
		ChatroomManager::GetInstance()->OnLinkConditionCallback(room_id, condition);
	};
	qtbase::Post2Task(kThreadUIHelper, task);
}

}