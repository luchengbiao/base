#include "invite_manager.h"
#include "base\macros.h"
#include "nim_invite.h"
#include "agora_invite.h"
#include "channel_manager\channel_manager.h"
#include "nim_rts\manager\rts_manager.h"


InviteManager::InviteManager()
{
	base_invite_ = nullptr;
	invite_status_ = false;
	chat_channel_status_ = false;
	data_channel_status_ = false;
}

InviteManager::~InviteManager()
{

}

std::string InviteManager::GetCallExternInfo()
{
	return call_info_.extra_info_;
}

void InviteManager::OnClearAllErrorStatus()
{
	invite_status_ = true;
	chat_channel_status_ = true;
	data_channel_status_ = true;
}

void InviteManager::UpdateInviteStatus(bool status)
{
	invite_status_ = status;
}

void InviteManager::UpdateDataChannelStatus(bool status)
{
	data_channel_status_ = status;
}

void InviteManager::UpdateChatChannelStatus(bool status)
{
	chat_channel_status_ = status;
}

void InviteManager::DoAgoraJoinSuccessCB(const std::string s_channel_id, uint32_t uid)
{
	OnClearAllErrorStatus();
	for (auto it : join_chat_succ_cb_list_)
	{
		it(s_channel_id,uid);
	}
}

void InviteManager::RegAgoraJoinSuccessCB(JoinChatChannelSuccessCallBack cb)
{
	join_chat_succ_cb_list_.push_back(cb);
}

std::string InviteManager::GetCallFailStatus()
{
	std::string status = "";
	if (invite_status_)
	{
		status += "1";
	}
	else
	{
		status += "0";
	}

	if (data_channel_status_)
	{
		status += "1";
	}
	else
	{
		status += "0";
	}

	if (chat_channel_status_)
	{
		status += "1";
	}
	else
	{
		status += "0";
	}

	return status;
}

void InviteManager::InitInvite(One2OneCallInfo call_info)
{
	call_info_ = call_info;

	if (base_invite_)
	{
		base_invite_->CancelAllService();
		SAFE_DELETE(base_invite_);
	}

	base_invite_ = CreateInvite(call_info_.m_setvice_type_);
	if (!call_info_.b_creator_)
	{
		base_invite_->OnSetCallInfo(call_info_);
	}
}

void InviteManager::StartInvite()
{
	if (base_invite_)
	{
		base_invite_->OnInitInvite(call_info_);
	}
}

void InviteManager::AgoraStartInvite()
{
	if (base_invite_)
	{
		base_invite_->StartInvite();
	}
}

BaseInvite* InviteManager::CreateInvite(NetworkServiceType type)
{
	switch (type)
	{
	case SWITCH_NETWORK_NETEASE:
		return new NimInvite();
		break;
	case SWITCH_NETWORK_AGORA:
		return new AgoraInvite();
		break;
	default:
		break;
	}
	return nullptr;
}

void InviteManager::OnSafeDeleteChannel()
{
	ChannelManager::GetInstance()->ReleaseDataChannel(call_info_.peer_agora_account_, call_info_.peer_agora_account_pc_);
	ChannelManager::GetInstance()->ReleaseChatChannel();
}

void InviteManager::OnSafeObserveLeaveChannel()
{
	ChannelManager::GetInstance()->ReleaseObserveDataChannel();
	ChannelManager::GetInstance()->ReleaseObserveChatChannel();
}

void InviteManager::CloseInvite()
{
	if (base_invite_)
	{
		base_invite_->CancelAllService();
		SAFE_DELETE(base_invite_);
	}
	OnSafeDeleteChannel();
}

void InviteManager::EndFailInvite()
{
	OnClearAllErrorStatus();
	if (!base_invite_)
	{
		return;
	}
	base_invite_->EndFailInvite(call_info_.b_creator_);
}

void InviteManager::OnUnregisterInviteCallBack()
{
	session_id_cb_ = nullptr;
	restart_call_cb_ = nullptr;
	nim_ack_cb_ = nullptr;
	nim_call_fail_cb_ = nullptr;

	for (auto it : join_chat_succ_cb_list_)
	{
		it = nullptr;
	}
	join_chat_succ_cb_list_.clear();

	for (auto it : register_cb_list_)
	{
		it();
		it = nullptr;
	}
	register_cb_list_.clear();
}

void InviteManager::AcceptInvite()
{
	if (!base_invite_)
	{
		return;
	}
	if (call_info_.m_setvice_type_ == SWITCH_NETWORK_AGORA)
	{
		base_invite_->OnInitInvite(call_info_, true);
	}
	else
	{
		base_invite_->AcceptInvite();
	}
}

void InviteManager::AgoraAcceptInvite()
{
	OnClearAllErrorStatus();
	if (!base_invite_)
	{
		return;
	}
	base_invite_->AcceptInvite();
}

void InviteManager::RefuseInvite()
{
	OnClearAllErrorStatus();
	if (!base_invite_)
	{
		return;
	}
	base_invite_->RefuseInvite();
}

void InviteManager::CancelInvite()
{
	OnClearAllErrorStatus();
	if (!base_invite_)
	{
		return;
	}
	base_invite_->CancelInvite();
}

void InviteManager::EndInviteAlone()
{
	OnClearAllErrorStatus();
	if (base_invite_)
	{
		base_invite_->CancelAllService();
		SAFE_DELETE(base_invite_);
	}
	OnSafeObserveLeaveChannel();
}

void InviteManager::DoRestartCallCB()
{
	if (restart_call_cb_)
	{
		restart_call_cb_();
	}
}

void InviteManager::RegNIMStartChannelMsgCb(const nim::Rts::StartNotifyCallback& callback)
{
	UnregisterCallback cb = nim_comp::RtsManager::GetInstance()->RegStartChannelMsgCb(callback);
	register_cb_list_.push_back(cb);
}

void InviteManager::RegNIMAckNotifyCb(const nim::Rts::AckNotifyCallback& callback)
{
	UnregisterCallback cb = nim_comp::RtsManager::GetInstance()->RegAckCb(callback);
	register_cb_list_.push_back(cb);
}

void InviteManager::RegNIMSyncAckCb(const nim::Rts::SyncAckNotifyCallback& callback)
{
	UnregisterCallback cb = nim_comp::RtsManager::GetInstance()->RegSyncAckCb(callback);
	register_cb_list_.push_back(cb);
}

void InviteManager::RegNIMConnectCb(const nim::Rts::ConnectNotifyCallback& callback)
{
	UnregisterCallback cb = nim_comp::RtsManager::GetInstance()->RegConnectCb(callback);
	register_cb_list_.push_back(cb);
}

void InviteManager::RegNIMMemberCb(const nim::Rts::MemberNotifyCallback& callback)
{
	UnregisterCallback cb = nim_comp::RtsManager::GetInstance()->RegMemberCb(callback);
	register_cb_list_.push_back(cb);
}

void InviteManager::RegNIMHangupCb(const nim::Rts::HangupNotifyCallback& callback)
{
	UnregisterCallback cb = nim_comp::RtsManager::GetInstance()->RegHangupCb(callback);
	register_cb_list_.push_back(cb);
}

void InviteManager::RegNIMControlCb(const nim::Rts::ControlNotifyCallback& callback)
{
	UnregisterCallback cb = nim_comp::RtsManager::GetInstance()->RegControlCb(callback);
	register_cb_list_.push_back(cb);
}

void InviteManager::RegNIMRecDataCb(const nim::Rts::RecDataCallback& callback)
{
	UnregisterCallback cb = nim_comp::RtsManager::GetInstance()->RegRecDataCb(callback);
	register_cb_list_.push_back(cb);
}

void InviteManager::RegAgoraInviteReceivedCb(const AgoraInviteExtraCallback& callback)
{
	UnregisterCallback cb = AGInviteManager::GetInstance()->RegInviteReceivedCb(callback);
	register_cb_list_.push_back(cb);
}

void InviteManager::RegAgoraInviteReceivedByPeerCb(const AgoraInviteCallback& callback)
{
	UnregisterCallback cb = AGInviteManager::GetInstance()->RegInviteReceivedByPeerCb(callback);
	register_cb_list_.push_back(cb);
}

void InviteManager::RegAgoraInviteAcceptedByPeerCb(const AgoraInviteExtraCallback& callback)
{
	UnregisterCallback cb = AGInviteManager::GetInstance()->RegInviteAcceptedByPeerCb(callback);
	register_cb_list_.push_back(cb);
}

void InviteManager::RegAgoraInviteRefusedByPeerCb(const AgoraInviteExtraCallback& callback)
{
	UnregisterCallback cb = AGInviteManager::GetInstance()->RegInviteRefusedByPeerCb(callback);
	register_cb_list_.push_back(cb);
}

void InviteManager::RegAgoraInviteEndByPeerCb(const AgoraInviteExtraCallback& callback)
{
	UnregisterCallback cb = AGInviteManager::GetInstance()->RegInviteEndByPeerCb(callback);
	register_cb_list_.push_back(cb);
}

void InviteManager::RegAgoraInviteFailedCb(const AgoraInviteFailCallback& callback)
{
	UnregisterCallback cb = AGInviteManager::GetInstance()->RegInviteFailedCb(callback);
	register_cb_list_.push_back(cb);
}

void InviteManager::RegAgoraInviteEndByMyselfCb(const AgoraInviteCallback& callback)
{
	UnregisterCallback cb = AGInviteManager::GetInstance()->RegInviteEndByMyselfCb(callback);
	register_cb_list_.push_back(cb);
}

void InviteManager::RegAgoraChatJoinSuccessCb(const AgoraJoinSuccessCallback& callback)
{
	UnregisterCallback cb = AGVchatManager::GetInstance()->RegJoinSuccessCb(callback);
	register_cb_list_.push_back(cb);
}

void InviteManager::RegAgoraChatReJoinSuccessCb(const AgoraReJoinSuccessCallback& callback)
{
	UnregisterCallback cb = AGVchatManager::GetInstance()->RegReJoinSuccessCb(callback);
	register_cb_list_.push_back(cb);
}

void InviteManager::RegAgoraChatChannelErrorCb(const AgoraChannelErrorCallback& callback)
{
	UnregisterCallback cb = AGVchatManager::GetInstance()->RegChannelErrorCb(callback);
	register_cb_list_.push_back(cb);
}

void InviteManager::RegAgoraChatUserJoinCb(const AgoraUserJoinCallback& callback)
{
	UnregisterCallback cb = AGVchatManager::GetInstance()->RegUserJoinCb(callback);
	register_cb_list_.push_back(cb);
}

void InviteManager::RegAgoraChatUserOfflineCb(const AgoraUserOfflineCallback& callback)
{
	UnregisterCallback cb = AGVchatManager::GetInstance()->RegUserOfflineCb(callback);
	register_cb_list_.push_back(cb);
}

void InviteManager::RegAgoraChatConnectLostCb(const AgoraConnectLostCallback& callback)
{
	UnregisterCallback cb = AGVchatManager::GetInstance()->RegConnectLostCb(callback);
	register_cb_list_.push_back(cb);
}

void InviteManager::RegAgoraChatLeaveChannelCb(const AgoraLeaveChannelCallback& callback)
{
	UnregisterCallback cb = AGVchatManager::GetInstance()->RegLeaveChannelCb(callback);
	register_cb_list_.push_back(cb);
}

void InviteManager::RegAgoraAudioQualityCb(const AgoraAudioQualityCallback& callback)
{
	UnregisterCallback cb = AGVchatManager::GetInstance()->RegAudioQualityCb(callback);
	register_cb_list_.push_back(cb);
}

void InviteManager::RegNetworkQualityCb(const AgoraNetworkQualityCallback& callback)
{
	UnregisterCallback cb = AGVchatManager::GetInstance()->RegNetworkQualityCb(callback);
	register_cb_list_.push_back(cb);
}

void InviteManager::RegAgoraDataChannelJoinCb(const SignallingChannelJoinCallback& callback)
{
	UnregisterCallback cb = AGSignallingChannelManager::GetInstance()->RegChannelJoinCb(callback);
	register_cb_list_.push_back(cb);
}

void InviteManager::RegAgoraDataUserJoinedCb(const SignallingUserJoinedCallback& callback)
{
	UnregisterCallback cb = AGSignallingChannelManager::GetInstance()->RegUserJoinedCb(callback);
	register_cb_list_.push_back(cb);
}

void InviteManager::RegAgoraDataUserLeavedCb(const SignallingUserLeavedCallback& callback)
{
	UnregisterCallback cb = AGSignallingChannelManager::GetInstance()->RegUserLeavedCb(callback);
	register_cb_list_.push_back(cb);
}

void InviteManager::RegAgoraDataChannelLeavedCb(const SignallingChannelLeavedCallback& callback)
{
	UnregisterCallback cb = AGSignallingChannelManager::GetInstance()->RegChannelLeavedCb(callback);
	register_cb_list_.push_back(cb);
}

void InviteManager::RegAgoraDataMsgInstantRecCb(const MsgInstantRecCallback& callback)
{
	UnregisterCallback cb = AGSignallingChannelManager::GetInstance()->RegMsgInstantRecCb(callback);
	register_cb_list_.push_back(cb);
}

void InviteManager::RegAgoraDataMsgChannelRecCb(const MsgChannelRecCallback& callback)
{
	UnregisterCallback cb = AGSignallingChannelManager::GetInstance()->RegMsgChannelRecCb(callback);
	register_cb_list_.push_back(cb);
}

void InviteManager::RegAgoraDataChannelJoinFailCb(const SignallingChannelJoinFailCallback& callback)
{
	UnregisterCallback cb = AGSignallingChannelManager::GetInstance()->RegChannelJoinFailCb(callback);
	register_cb_list_.push_back(cb);
}

void InviteManager::RegAgoraDataMsgSendErrorCb(const MsgSendErrorCallback& callback)
{
	UnregisterCallback cb = AGSignallingChannelManager::GetInstance()->RegMsgSendErrorCb(callback);
	register_cb_list_.push_back(cb);
}

void InviteManager::RegAgoraDataMsgSendSuccessCb(const MsgSendSuccessCallback& callback)
{
	UnregisterCallback cb = AGSignallingChannelManager::GetInstance()->RegMsgSendSuccessCb(callback);
	register_cb_list_.push_back(cb);
}

void InviteManager::RegNIMAckCb(const nim::Rts::AckCallback& callback)
{
	nim_ack_cb_ = callback;
}

void InviteManager::DoNimAckCB(nim::NIMResCode res_code, const std::string session_id, int channel_type, bool accept)
{
	if (nim_ack_cb_)
	{
		nim_ack_cb_(res_code, session_id, channel_type, accept);
	}
}

void InviteManager::RegNimCallFailCb(const nim::Rts::CreateConfCallback& callback)
{
	nim_call_fail_cb_ = callback;
}

void InviteManager::DoCallFailCb(nim::NIMResCode res_code)
{
	if (nim_call_fail_cb_)
	{
		nim_call_fail_cb_(res_code);
	}
}

void InviteManager::EndInvite(std::string peer_account)
{
	if (!base_invite_)
	{
		return;
	}
	base_invite_->EndInvite(peer_account);
}

void InviteManager::DoNimSessionIdCb(std::string session_id)
{
	call_info_.nim_session_id_ = session_id;
	if (session_id_cb_)
	{
		session_id_cb_(session_id);
	}
}

void InviteManager::SendP2PMessage(NetworkServiceType m_setvice_type, std::string account, uint32_t uid, std::string s_msg)
{
	if (m_setvice_type == SWITCH_NETWORK_NETEASE)
	{
		nim_comp::RtsManager::GetInstance()->Control(account, s_msg, nim::Rts::ControlCallback());
	}
	else
	{
		AGSignallingChannelManager::GetInstance()->MessageInstantSend(account, uid, s_msg, "");
	}
}

void InviteManager::EndWithoutInvite(NetworkServiceType m_setvice_type, std::string s_channel_id, std::string s_account)
{
	if (m_setvice_type == SWITCH_NETWORK_NETEASE)
	{
		nim_comp::RtsManager::GetInstance()->Ack(s_channel_id, nim::kNIMRtsChannelTypeTcp | nim::kNIMRtsChannelTypeVchat, false, true, true, nim::Rts::AckCallback());
	}
	else
	{
		AGInviteManager::GetInstance()->ChannelInviteRefuse(s_channel_id, s_account, 0, "");
	}
}

void InviteManager::OnStartInviteByAccount(std::string account)
{
	if (base_invite_)
	{
		base_invite_->StartInviteByAccount(account);
	}
}

std::string InviteManager::GetNimChannelId()
{
	std::string channel_id = "";
	BaseInviteDetail info;
	if (base_invite_ &&
		base_invite_->GetDetail(info))
	{
		channel_id = info.nim_channel_id;
	}
	return channel_id;
}

void InviteManager::RegAgoraLoginCb(const AgoraLoginCallback& callback)
{
	UnregisterCallback cb = AGLoginManager::GetInstance()->RegLoginCb(callback);
	register_cb_list_.push_back(cb);
}

void InviteManager::RegAgoraLogoutCb(const AgoraLogoutCallback& callback)
{
	UnregisterCallback cb = AGLoginManager::GetInstance()->RegLogoutCb(callback);
	register_cb_list_.push_back(cb);
}

void InviteManager::StartObserveEnter(One2OneCallInfo call_info)
{
	InitInvite(call_info);
	if (base_invite_)
	{
		base_invite_->OnInitInvite(call_info_,true);
	}
}