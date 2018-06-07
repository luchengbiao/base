#include "ui_chatroom_callback.h"
#include "nim_chatroom\manager\chatroom_manager.h"
#include "chatroom_info.h"
#include "nim_session/manager/team_manager.h"

UIChatroomCallback::UIChatroomCallback()
{
	auto rec_msg = nbase::Bind(&UIChatroomCallback::OnReceiveMsgCallback, this, std::placeholders::_1, std::placeholders::_2);
	nim_comp::ChatroomManager::GetInstance()->RegReceiveMsgCallback(rec_msg);

	auto send_msg = nbase::Bind(&UIChatroomCallback::OnSendMsgAckCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	nim_comp::ChatroomManager::GetInstance()->RegSendMsgAckCallback(send_msg);

	auto enetr_room = nbase::Bind(&UIChatroomCallback::OnEnterCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
	nim_comp::ChatroomManager::GetInstance()->RegEnterCallback(enetr_room);

	auto exit_room = nbase::Bind(&UIChatroomCallback::OnExitCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	nim_comp::ChatroomManager::GetInstance()->RegExitCallback(exit_room);

	auto notification = nbase::Bind(&UIChatroomCallback::OnNotificationCallback, this, std::placeholders::_1, std::placeholders::_2);
	nim_comp::ChatroomManager::GetInstance()->RegNotificationCallback(notification);

	auto link = nbase::Bind(&UIChatroomCallback::OnLinkConditionCallback, this, std::placeholders::_1, std::placeholders::_2);
	nim_comp::ChatroomManager::GetInstance()->RegLinkConditionCallback(link);

	auto sysmsg = nbase::Bind(&UIChatroomCallback::OnReceiveSysmsgCallback, this, std::placeholders::_1);
	nim_comp::TeamManager::GetInstance()->RegReceiveSysmsg(sysmsg);
}

UIChatroomCallback::~UIChatroomCallback()
{

}

void UIChatroomCallback::OnReceiveMsgCallback(int64_t room_id, const nim_chatroom::ChatRoomMessage& result)
{
	if (result.msg_type_ == nim_chatroom::kNIMChatRoomMsgTypeText)
	{
		ReceiveTextMsg(result);
	}
	else if (result.msg_type_ == nim_chatroom::kNIMChatRoomMsgTypeNotification)
	{
		//ReceiveNotifyMsg(result);
	}
	else if (result.msg_type_ == nim_chatroom::kNIMChatRoomMsgTypeCustom)
	{
		ReceiveCustomMsg(result);
	}
}

void UIChatroomCallback::OnSendMsgAckCallback(int64_t room_id, int error_code, const nim_chatroom::ChatRoomMessage& result)
{

}

void UIChatroomCallback::OnEnterCallback(int64_t room_id, int step, int error_code, const nim_chatroom::ChatRoomInfo& info, const nim_chatroom::ChatRoomMemberInfo& my_info)
{
	emit SignalEnterRoom(error_code, step, room_id, info.creator_id_);
}

void UIChatroomCallback::OnExitCallback(int64_t room_id, int error_code, int exit_reason)
{
	emit SignalExitRoom(room_id, exit_reason);
}

void UIChatroomCallback::OnNotificationCallback(int64_t room_id, const nim_chatroom::ChatRoomNotification& notification)
{
	auto it_nick = notification.target_nick_.cbegin();
	auto it_id = notification.target_ids_.cbegin();
	for (;	it_nick != notification.target_nick_.cend(), it_id != notification.target_ids_.cend();
		++it_id, ++it_nick)
	{
		emit SignalReceiveMsgTypeNotification(notification.id_, *it_id, *it_nick);
	}
}

void UIChatroomCallback::OnLinkConditionCallback(int64_t room_id, int condition)
{
	emit SignalLinkCondition(room_id, condition);
}

void UIChatroomCallback::ReceiveNotifyMsg(const nim_chatroom::ChatRoomMessage& msg)
{
	Json::Value json;
	Json::Reader reader;
	if (reader.parse(msg.msg_attach_, json))
	{
		nim_chatroom::ChatRoomNotification notification;
		notification.ParseFromJsonValue(json);
		
		auto it_nick = notification.target_nick_.cbegin();
		auto it_id = notification.target_ids_.cbegin();
		for (;	it_nick != notification.target_nick_.cend(), it_id != notification.target_ids_.cend();
			++it_id, ++it_nick)
		{
			emit SignalReceiveMsgTypeNotification(notification.id_, *it_id, *it_nick);
		}
	}
}

void UIChatroomCallback::ReceiveTextMsg(const nim_chatroom::ChatRoomMessage& msg)
{
	emit SignalReceiveMsgTypeText(msg.msg_attach_, msg.from_id_, msg.from_nick_);
}

void UIChatroomCallback::ReceiveCustomMsg(const nim_chatroom::ChatRoomMessage& msg)
{
	Json::Value json;
	Json::Reader reader;
	if (reader.parse(msg.msg_attach_, json))
	{
		int sub_type = json["type"].asInt();
		if (sub_type == CustomMsgType_Meeting && json["data"].isObject())
		{
			emit SignalReceiveMsgTypeMeeting(msg.msg_attach_, msg.from_id_);
		}
	}
}

void UIChatroomCallback::OnReceiveSysmsgCallback(const nim::SysMessage& msg)
{
	if (msg.type_ == nim::kNIMSysMsgTypeCustomP2PMsg)
	{
		Json::Value json;
		Json::Reader reader;
		if (reader.parse(msg.attach_, json))
		{
			int sub_type = json["type"].asInt();

			if (sub_type == CustomMsgType_Meeting && json["data"].isObject())
			{
				emit SignalReceiveMsgTypeMeeting(msg.attach_, msg.sender_accid_);
			}
		}
	}
}
