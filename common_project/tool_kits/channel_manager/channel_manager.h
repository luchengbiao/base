#pragma once
#include "base\memory\singleton.h"
#include "channel_manager_global.h"
#include "base_data_channel.h"
#include "base_chat_channel.h"

//����Ƶ ����ͨ����������
class ChannelManager
{
public:
	SINGLETON_DEFINE(ChannelManager);
	ChannelManager();
	~ChannelManager();

	BaseDataChannel* CreateDataChannel(NetworkServiceType type, bool b_creator);    //��������Ƶ������
	BaseChatChannel* CreateChatChannel(NetworkServiceType type, bool b_creator);	//��������Ƶ������

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
