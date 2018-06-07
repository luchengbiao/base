#include "agora_invite.h"
#include "invite_manager.h"
#include "channel_manager\channel_manager.h"
#include "agora_invite\agora_invite_manager.h"
#include "agora_signalling_channel\agora_signalling_channel_manager.h"
#include "agora_manager\AgoraDeviceManager.h"

AgoraInvite::AgoraInvite()
{
	data_cb_ = nbase::Bind(&AgoraInvite::DataJoinCallBack, this, std::placeholders::_1, std::placeholders::_2);
	chat_cb_ = nbase::Bind(&AgoraInvite::ChatJoinCallBack, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
}

AgoraInvite::~AgoraInvite()
{

}

void AgoraInvite::OnInitInvite(One2OneCallInfo info, bool b_force_join)
{
	info_ = info;
	if (!b_force_join && !info.b_creator_)
	{
		return;
	}
	BaseDataChannel* data_channel = ChannelManager::GetInstance()->CreateDataChannel(info.data_type_, info.b_creator_);
	BaseChatChannel* chat_channel = ChannelManager::GetInstance()->CreateChatChannel(info.chat_type_, info.b_creator_);
	if (data_channel && chat_channel)
	{
		data_channel->JoinChannel(info_.s_channel_id_, data_cb_);
	}
}

void AgoraInvite::StartInvite()
{
	// 说明老师已经加入信令通道与通信通道，然后开始呼叫对方
	std::string s_invite_extern = InviteManager::GetInstance()->GetCallExternInfo();
	//QONLINELOG_CALL(L"CallingForm::OnJoinMediaChannelSuccess {0}") << L"主动加入频道成功后，发起邀请";
	// 呼叫app
	AGInviteManager::GetInstance()->ChannelInviteUser(info_.s_channel_id_, info_.peer_agora_account_, s_invite_extern);
	// 呼叫pc
	AGInviteManager::GetInstance()->ChannelInviteUser(info_.s_channel_id_, info_.peer_agora_account_pc_, s_invite_extern);
}

void AgoraInvite::DataJoinCallBack(std::string channel_id, int code)
{
	if (channel_id != info_.s_channel_id_)
	{
		return;
	}

	if (code >= 0)
	{
		AGEngineManager* Agora_Object = AgoraDeviceManager::GetInstance()->GetEngineManager();
		Agora_Object->SetChannelProfile(agora::rtc::CHANNEL_PROFILE_COMMUNICATION);
		BaseChatChannel* chat_channel = ChannelManager::GetInstance()->GetChatChannel();
		if (chat_channel)
		{
			uint32_t agora_uid = info_.my_agora_uid_;
			chat_channel->JoinChannel(chat_cb_, agora_uid, info_.s_channel_id_, info_.agora_chatchannel_key_);
		}
	}
	else
	{
		//加入频道失败
		InviteManager::GetInstance()->UpdateDataChannelStatus(false);
		InviteManager::GetInstance()->DoRestartCallCB();
		//ApiCommTool::GetInstance()->ApiSubmitClientLog(UserInfo::GetInstance()->GetUserId(), "JoinChannelFail", "StuAgoraImDataJoinError", code);
	}
}

void AgoraInvite::ChatJoinCallBack(int code, std::string channel_id, uint32_t uid, std::string extra_str)
{
	if (channel_id != info_.s_channel_id_)
	{
		return;
	}

	if (code >= 0)
	{
		InviteManager::GetInstance()->DoAgoraJoinSuccessCB(info_.s_channel_id_, uid);
	}
	else
	{
		//加入频道失败
		InviteManager::GetInstance()->UpdateChatChannelStatus(false);
		InviteManager::GetInstance()->DoRestartCallCB();
		//ApiCommTool::GetInstance()->ApiSubmitClientLog(UserInfo::GetInstance()->GetUserId(), "JoinChannelFail", "StuAgoraImChatJoinError", code);
	}
}

void AgoraInvite::AcceptInvite()
{
	AGInviteManager::GetInstance()->ChannelInviteAccept(info_.s_channel_id_, info_.peer_agora_account_, 0);
}

void AgoraInvite::RefuseInvite()
{
	AGInviteManager::GetInstance()->ChannelInviteRefuse(info_.s_channel_id_, info_.peer_agora_account_, 0, "");
}

void AgoraInvite::EndInvite(std::string peer_account)
{
	AGInviteManager::GetInstance()->ChannelInviteEnd(info_.s_channel_id_, peer_account, 0);
}

void AgoraInvite::CancelAllService()
{

}

void AgoraInvite::CancelInvite()
{
	AGSignallingChannelManager::GetInstance()->ChannelLeave(info_.s_channel_id_);
	AGInviteManager::GetInstance()->ChannelInviteEnd(info_.s_channel_id_, info_.peer_agora_account_, 0);
	AGInviteManager::GetInstance()->ChannelInviteEnd(info_.s_channel_id_, info_.peer_agora_account_pc_, 0);
}

void AgoraInvite::EndFailInvite(bool b_creator)
{
	if (b_creator)
	{
		CancelInvite();
	}
	/*else
	{
	RefuseInvite();
	}*/
}

bool AgoraInvite::GetDetail(BaseInviteDetail& detail)
{
	return true;
}

void AgoraInvite::StartInviteByAccount(std::string account)
{
	// 说明老师已经加入信令通道与通信通道，然后开始呼叫对方
	std::string s_invite_extern = InviteManager::GetInstance()->GetCallExternInfo();
	//QONLINELOG_CALL(L"CallingForm::OnJoinMediaChannelSuccess {0}") << L"主动加入频道成功后，发起邀请";
	AGInviteManager::GetInstance()->ChannelInviteUser(info_.s_channel_id_, account, s_invite_extern);
}







