#pragma once

#include "nim_sdk_manager\helper\nim_chatroom_helper.h"


namespace nim_comp
{
/** @class ChatroomCallback
  * @brief 聊天室相关回调类
  * @copyright (c) 2015, NetEase Inc. All rights reserved
  * @author redrain
  * @date 2016/1/6
  */
class ChatroomCallback
{
public:
	/**
	* 收到普通消息的回调，需要在程序开始运行时就注册好。
	* @param[in] room_id 消息所属房间号
	* @param[in] result 消息内容及其他相关信息
	* @return void	无返回值
	*/
	static void OnReceiveMsgCallback(__int64 room_id, const nim_chatroom::ChatRoomMessage& result);

	/**
	* 发送消息的结果回调，需要在程序开始运行时就注册好。
	* @param[in] room_id 消息所属房间号
	* @param[in] result 消息内容及其他相关信息
	* @return void	无返回值
	*/
	static void OnSendMsgCallback(__int64 room_id, int error_code, const nim_chatroom::ChatRoomMessage& result);

	/**
	* 进入某聊天室的结果回调，需要在程序开始运行时就注册好。
	* @param[in] room_id 消息所属房间号
	* @param[in] error_code 错误码
	* @param[in] info 聊天室相关信息
	* @return void	无返回值
	*/
	static void OnEnterCallback(__int64 room_id, int step, int error_code, 
		const nim_chatroom::ChatRoomInfo& info, const nim_chatroom::ChatRoomMemberInfo& my_info);

	/**
	* 退出某聊天室的结果回调，需要在程序开始运行时就注册好。
	* @param[in] room_id 消息所属房间号
	* @param[in] error_code 错误码
	* @param[in] exit_reason 退出原因
	* @return void	无返回值
	*/
	static void OnExitCallback(__int64 room_id, int error_code, nim_chatroom::NIMChatRoomExitReason exit_reason);

	/**
	* 收到聊天室通知的结果回调，需要在程序开始运行时就注册好。
	* @param[in] room_id 消息所属房间号
	* @param[in] notification 通知信息
	* @return void	无返回值
	*/
	static void OnNotificationCallback(__int64 room_id, const nim_chatroom::ChatRoomNotification& notification);

	/**
	* 注册全局聊天室链接情况回调
	* @param[in] room_id 消息所属房间号
	* @param[in] condition 聊天室链接情况，一般都是有本地网路情况引起
	* @return void 无返回值
	*/
	static void OnRegLinkConditionCallback(__int64 room_id, int condition);
};

}