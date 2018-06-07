#pragma once
#include "base\memory\singleton.h"
#include "channel_manager_global.h"
#include "base_data_channel.h"
#include "base_chat_channel.h"

//音视频 数据通道公共部分
class ChannelManager
{
public:
	SINGLETON_DEFINE(ChannelManager);
	ChannelManager();
	~ChannelManager();

	BaseDataChannel* CreateDataChannel(NetworkServiceType type, bool b_creator);    //创建数据频道工厂
	BaseChatChannel* CreateChatChannel(NetworkServiceType type, bool b_creator);	//创建音视频道工厂

	void ReleaseDataChannel(std::string notified_account, std::string notified_account_pc);
	void ReleaseChatChannel();

	void ReleaseObserveDataChannel();
	void ReleaseObserveChatChannel();

	BaseDataChannel* GetDataChannel(){ return data_channel_; }
	BaseChatChannel* GetChatChannel(){ return chat_channel_; }

private:
	BaseDataChannel* data_channel_;
	BaseChatChannel* chat_channel_;

};
