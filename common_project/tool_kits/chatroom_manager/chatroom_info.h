#pragma once
#include "base\callback\callback.h"
#include "channel_manager\channel_manager_global.h"


enum MeetingOptType//command
{
	Mot_AllStatus = 1,	//主持人通知有权限的成员列表					《1》《2》	{"type":10,"data":{"room_id":"123","command":1,"uids":["a","b"]}}
	Mot_GetStatus = 2,	//成员向所有人请求有权限的成员					《1》		{"type":10,"data":{"room_id":"123","command":2}}
	Mot_StatusResponse = 3,	//有权限的成员向请求者返回自己有权限的通知		《2》		{"type":10,"data":{"room_id":"123","command":3,"uids":["myid"]}}
	Mot_SpeekRequest = 10,	//向主持人请求连麦权限							《2》		{"type":10,"data":{"room_id":"123","command":10}}
	Mot_SpeekAccept = 11,	//主持人同意连麦请求,主持人同时发送群发1消息	《2》		{"type":10,"data":{"room_id":"123","command":11}}
	Mot_SpeekReject = 12,	//主持人拒绝或关闭连麦,主持人同时发送群发1消息	《2》		{"type":10,"data":{"room_id":"123","command":12}}
	Mot_SpeekRequestCancel = 13,	//取消向主持人请求连麦权限						《2》		{"type":10,"data":{"room_id":"123","command":13}}
	Mot_ChannelCreate = 14,	//创建频道通知 所有人
	Mot_AllSilent = 15,	//全场禁言
	Mot_AllSilentCancel = 16,	//取消全场禁言
	Mot_StuSpeakStart = 17,	//学生开始说话
	Mot_StuSpeakEnd = 18,	//学生结束说话
	Mot_LessionIdInfo = 19,	//学生进入房间时，告诉学生当前的lessionId		p2p消息 {"data":{"room_id":"123","command":19,"lession_id":929,"uid":1}}
	Mot_StartLession = 20,	//开始上课 聊天室消息
	Mot_EndLession = 21,	//结束上课
	Mot_MasterVolumn = 22,	//主持人说话的音量{"data":{"room_id":"123","command":19,"volumes":"10,23,32,43,0,0"}}
	Mot_ForbbidenSpeak = 23,	//禁止发言	p2p
	Mot_ShareScreenRatio = 24,	//分享分辨率	p2p
	Mot_PlayClassOverRing = 25,	//房间消息， 播放下课铃声
	Mot_CourseConnect = 26,   //主持人课程已连接上
	Mot_CourseDisconnect = 27,   //主持人课程断开
};

enum CustomMsgType
{
	CustomMsgType_Jsb = 1,	//石头剪子布
	CustomMsgType_SnapChat,	//阅后即焚
	CustomMsgType_Sticker,	//贴图
	CustomMsgType_Rts,		//白板的发起结束消息
	CustomMsgType_Meeting = 10,		//多人会议控制协议
};

struct ChannelJoinParam
{
	NetworkServiceType data_service_;
	NetworkServiceType chat_service_;
	DataChannelConnectStateCb connect_state_cb_;
	DataChannelMessageCb rev_msg_cb_;
	bool b_tea_;				//创建时候根据角色设置给麦方式
	std::string channel_id_;	
	uint32_t agora_uid_;
	std::string channel_key_;

	std::string own_url_;

	ChannelJoinParam()
	{
		data_service_ = SWITCH_NETWORK_OWN;
		chat_service_ = SWITCH_NETWORK_AGORA;
		own_url_ = "";
	}
};

enum ClassroomChannelType
{
	CLASSROOM_CHANNEL_TYPE_DATA = 0,
	CALSSROOM_CHANNEL_TYPE_CHAT = 1,
};

typedef std::function<void(int, int)> ClassroomChannelJoinFailCb;
typedef std::function<void(const std::string s_channel_id, uint32_t uid)> ClassroomChatJoinChannelSuccessCb;