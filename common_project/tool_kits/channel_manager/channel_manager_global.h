#pragma once
#include <functional>
#include <stdint.h> 
//增加回调和数据结构定义


typedef std::function<void(std::string, int)> JoinChannelCallback;
typedef std::function<void(std::string, int)> LeaveChannelCallback;
typedef std::function<void(int, std::string, uint32_t, std::string)> JoinChatCb;

enum NetworkServiceType
{
	SWITCH_NETWORK_NETEASE = 0,		/** 网易云信 **/
	SWITCH_NETWORK_AGORA = 1		/** 声网    **/
};

struct ChannelInfo
{
	std::string s_nim_rts_session_id_;
	bool b_channel_connect_;
	std::string nim_channel_id;
	std::string agora_channel_id_;
};

