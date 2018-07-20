#include "channel_manager.h"
#include "base/macros.h"
#include "agora_chat_channel.h"
#include "agora_data_channel.h"
#include "nim_data_channel.h"
#include "nim_chat_channel.h"
#include "own_data_channel.h"

ChannelManager::ChannelManager()
{
	data_channel_ = nullptr;
	chat_channel_ = nullptr;
}

ChannelManager::~ChannelManager()
{

}

BaseDataChannel* ChannelManager::CreateDataChannel(NetworkServiceType type, bool b_creator)
{
	SAFE_DELETE(data_channel_);

	switch (type)
	{
	case SWITCH_NETWORK_NETEASE:
	{
		data_channel_ = new NimDataChannel(b_creator);
		break;
	}
	case SWITCH_NETWORK_AGORA:
		data_channel_ = new AgoraDataChannel(b_creator);
		break;
	case SWITCH_NETWORK_OWN:
		data_channel_ = new OwnDataChannel(b_creator);
	default:
		break;
	}
	return data_channel_;
}

BaseChatChannel* ChannelManager::CreateChatChannel(NetworkServiceType type, bool b_creator)
{
	SAFE_DELETE(chat_channel_);

	switch (type)
	{
	case SWITCH_NETWORK_NETEASE:
		chat_channel_ = new NimChatChannel(b_creator);
		break;
	case SWITCH_NETWORK_AGORA:
		chat_channel_ = new AgoraChatChannel(b_creator);
		break;
	default:
		break;
	}
	return chat_channel_;
}

void ChannelManager::ReleaseDataChannel(std::string notified_account, std::string notified_account_pc)
{
	if (data_channel_)
	{
		std::string session_id = data_channel_->GetSessionOrChannelID();
		data_channel_->LeaveChannel(session_id, notified_account, LeaveChannelCallback());
		if (data_channel_->GetChannelType() == SWITCH_NETWORK_AGORA)
		{
			data_channel_->LeaveChannel(session_id, notified_account_pc, LeaveChannelCallback());
		}

		SAFE_DELETE(data_channel_);
	}
}

void ChannelManager::ReleaseChatChannel()
{
	if (chat_channel_)
	{
		chat_channel_->LeaveChannel();
		SAFE_DELETE(chat_channel_);
	}
}

void ChannelManager::ReleaseObserveDataChannel()
{
	if (data_channel_)
	{
		std::string session_id = data_channel_->GetSessionOrChannelID();
		data_channel_->LeaveChannel(session_id, "", LeaveChannelCallback());

		SAFE_DELETE(data_channel_);
	}
}

void ChannelManager::ReleaseObserveChatChannel()
{
	ReleaseChatChannel();
}
