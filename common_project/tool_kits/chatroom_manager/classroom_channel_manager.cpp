#include "classroom_channel_manager.h"
#include "channel_manager\channel_manager.h"
#include "agora_manager\AgoraDeviceManager.h"

ClassroomChannelManager::ClassroomChannelManager()
{
	data_cb_ = nbase::Bind(&ClassroomChannelManager::DataJoinCallBack, this, std::placeholders::_1, std::placeholders::_2);
	chat_cb_ = nbase::Bind(&ClassroomChannelManager::ChatJoinCallBack, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
}

ClassroomChannelManager::~ClassroomChannelManager()
{

}

void ClassroomChannelManager::JoinDataChannel(ClassroomChatJoinChannelSuccessCb join_success_cb,ClassroomChannelJoinFailCb join_fail_cb, ChannelJoinParam join_param)
{
	join_success_cb_ = join_success_cb;
	join_fail_cb_ = join_fail_cb;
	join_param_ = join_param;
	BaseDataChannel* data_channel = ChannelManager::GetInstance()->CreateDataChannel(join_param.data_service_, false);
	BaseChatChannel* chat_channel = ChannelManager::GetInstance()->CreateChatChannel(join_param.chat_service_, false);

	if (join_param.data_service_ == SWITCH_NETWORK_OWN)
	{
		data_channel->SetConnectStateCb(join_param.connect_state_cb_);
		data_channel->SetChannelMessageCb(join_param.rev_msg_cb_);
	}
	data_channel->JoinChannel(join_param_.channel_id_, data_cb_, join_param_.own_url_);
}

void ClassroomChannelManager::JoinChatChannel()
{
	AGEngineManager* Agora_Object = AgoraDeviceManager::GetInstance()->GetEngineManager();
	if (Agora_Object)
	{
		Agora_Object->SetChannelProfile(agora::rtc::CHANNEL_PROFILE_LIVE_BROADCASTING);
		if (join_param_.b_tea_)
		{
			Agora_Object->SetClientRole(agora::rtc::CLIENT_ROLE_BROADCASTER, "");
		}
		else
		{
			Agora_Object->SetClientRole(agora::rtc::CLIENT_ROLE_AUDIENCE, "");
		}
	}
	
	BaseChatChannel* chat_channel = ChannelManager::GetInstance()->GetChatChannel();
	if (chat_channel)
	{
		uint32_t agora_uid = join_param_.agora_uid_;
		chat_channel->JoinChannel(chat_cb_, agora_uid, join_param_.channel_id_, join_param_.channel_key_);
	}
}

void ClassroomChannelManager::DataJoinCallBack(std::string channel_id, int code)
{
	if (channel_id != join_param_.channel_id_)
	{
		return;
	}

	if (code >= 0)
	{
		JoinChatChannel();
	}
	else
	{
		//加入频道失败
		join_fail_cb_(CLASSROOM_CHANNEL_TYPE_DATA, code);
	}
}

void ClassroomChannelManager::ChatJoinCallBack(int code, std::string channel_id, uint32_t uid, std::string extra_str)
{
	if (channel_id != join_param_.channel_id_)
	{
		return;
	}

	if (code >= 0)
	{
		join_success_cb_(channel_id,uid);
	}
	else
	{
		//加入频道失败
		join_fail_cb_(CALSSROOM_CHANNEL_TYPE_CHAT, code);
	}
}

void ClassroomChannelManager::OnSafeDeleteChannel()
{
	ChannelManager::GetInstance()->ReleaseObserveDataChannel();
	ChannelManager::GetInstance()->ReleaseObserveChatChannel();
}

void ClassroomChannelManager::OnSetClientRole(bool b_geimai)
{
	AGEngineManager* Agora_Object = AgoraDeviceManager::GetInstance()->GetEngineManager();
	if (Agora_Object)
	{
		if (b_geimai)
		{
			Agora_Object->SetClientRole(agora::rtc::CLIENT_ROLE_BROADCASTER, "");
		}
		else
		{
			Agora_Object->SetClientRole(agora::rtc::CLIENT_ROLE_AUDIENCE, "");
		}
	}
}
