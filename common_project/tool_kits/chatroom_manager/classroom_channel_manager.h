#pragma once
#include <string>
#include <stdint.h>
#include "base\memory\singleton.h"
#include "chatroom_info.h"


class ClassroomChannelManager :public nbase::SupportWeakCallback
{
public:
	SINGLETON_DEFINE(ClassroomChannelManager);
	ClassroomChannelManager();
	~ClassroomChannelManager();

	void JoinDataChannel(ClassroomChatJoinChannelSuccessCb join_success_cb,ClassroomChannelJoinFailCb join_fail_cb, ChannelJoinParam join_param);
	void JoinChatChannel();
	void OnSetClientRole(bool b_geimai);

	void OnSafeDeleteChannel();

private:
	void DataJoinCallBack(std::string channel_id, int code);
	void ChatJoinCallBack(int code, std::string channel_id, uint32_t uid, std::string extra_str);


private:
	ChannelJoinParam join_param_;
	ClassroomChannelJoinFailCb join_fail_cb_;
	ClassroomChatJoinChannelSuccessCb join_success_cb_;
	JoinChatCb chat_cb_;
	JoinChannelCallback data_cb_;
};