#pragma once
#include "QObject"
#include "base\callback\callback.h"
#include "nim_sdk_manager\helper\nim_chatroom_helper.h"
#include <string>
#include <stdint.h>
#include "nim_sdk_manager\helper\nim_sysmsg_helper.h"

class UIChatroomCallback : public QObject, public nbase::SupportWeakCallback
{
	Q_OBJECT
public:
	UIChatroomCallback();
	~UIChatroomCallback();

signals:
	void SignalReceiveMsgTypeText(std::string, std::string, std::string);
	void SignalReceiveMsgTypeNotification(int, std::string, std::string);
	void SignalReceiveMsgTypeMeeting(std::string, std::string);
	void SignalEnterRoom(int, int, int64_t, std::string);
	void SignalExitRoom(int64_t, int);
	void SignalLinkCondition(int64_t, int);

private:
	void OnReceiveMsgCallback(int64_t room_id, const nim_chatroom::ChatRoomMessage& result);
	void OnSendMsgAckCallback(int64_t room_id, int error_code, const nim_chatroom::ChatRoomMessage& result);
	void OnEnterCallback(int64_t room_id, int step, int error_code,
		const nim_chatroom::ChatRoomInfo& info, const nim_chatroom::ChatRoomMemberInfo& my_info);
	void OnExitCallback(int64_t room_id, int error_code, int exit_reason);
	void OnNotificationCallback(int64_t room_id, const nim_chatroom::ChatRoomNotification& notification);
	void OnLinkConditionCallback(int64_t room_id, int condition);
	void OnReceiveSysmsgCallback(const nim::SysMessage& msg);


	void ReceiveNotifyMsg(const nim_chatroom::ChatRoomMessage& msg);
	void ReceiveTextMsg(const nim_chatroom::ChatRoomMessage& msg);
	void ReceiveCustomMsg(const nim_chatroom::ChatRoomMessage& msg);
};