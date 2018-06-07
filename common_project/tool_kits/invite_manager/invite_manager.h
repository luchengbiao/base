#pragma once
#include "base\memory\singleton.h"
#include "base_invite.h"
#include "base\util\auto_unregister.h"
#include "nim_sdk_manager\api\nim_cpp_rts.h"
#include "agora_invite\agora_invite_manager.h"
#include "agora_vchat\agora_vchat_manager.h"
#include "agora_signalling_channel\agora_signalling_channel_manager.h"
#include "agora_login\agora_login_manager.h"

//呼叫公共部分
class InviteManager
{
public:
	SINGLETON_DEFINE(InviteManager);
	InviteManager();
	~InviteManager();

	void StartObserveEnter(One2OneCallInfo call_info);   //旁听进入前加入频道
	void InitInvite(One2OneCallInfo call_info);
	void StartInvite();
	void AgoraStartInvite();
	void OnStartInviteByAccount(std::string account);
	void CloseInvite();
	void EndFailInvite();
	void AcceptInvite();		//call UI 点击接受后调用
	void AgoraAcceptInvite();   //声网加入成功后调用
	void RefuseInvite();
	void CancelInvite();
	void EndInviteAlone();

	void EndInvite(std::string peer_account);
	void SendP2PMessage(NetworkServiceType m_setvice_type,std::string account, uint32_t uid, std::string s_msg);
	void EndWithoutInvite(NetworkServiceType m_setvice_type, std::string s_channel_id, std::string s_account);

	void OnSafeDeleteChannel();			//删除频道
	void OnSafeObserveLeaveChannel();	//旁听进来删除频道

	std::string GetCallFailStatus();
	std::string GetCallExternInfo();    //获取所有呼叫的额外信息

	void OnClearAllErrorStatus();
	void UpdateInviteStatus(bool status);
	void UpdateDataChannelStatus(bool status);
	void UpdateChatChannelStatus(bool status);
	uint32_t GetMyAgoraUid(){ return call_info_.my_agora_uid_; }

	std::string GetNimChannelId();   //网易呼叫获取channel_id

	//CB
	void RegAgoraJoinSuccessCB(JoinChatChannelSuccessCallBack cb);
	void RegRestartCallCB(UnregisterCallback cb){ restart_call_cb_ = cb; }
	void DoAgoraJoinSuccessCB(const std::string s_channel_id, uint32_t uid);
	void DoRestartCallCB();

	void RegNimSessionIdCb(SignallingChannelJoinCallback cb){ session_id_cb_ = cb; }
	void DoNimSessionIdCb(std::string session_id);

	//NIM REG CB
	void RegNimCallFailCb(const nim::Rts::CreateConfCallback& callback);
	void RegNIMStartChannelMsgCb(const nim::Rts::StartNotifyCallback& callback);
	void RegNIMAckCb(const nim::Rts::AckCallback& callback);
	void RegNIMAckNotifyCb(const nim::Rts::AckNotifyCallback& callback);
	void RegNIMSyncAckCb(const nim::Rts::SyncAckNotifyCallback& callback);
	void RegNIMConnectCb(const nim::Rts::ConnectNotifyCallback& callback);
	void RegNIMMemberCb(const nim::Rts::MemberNotifyCallback& callback);
	void RegNIMHangupCb(const nim::Rts::HangupNotifyCallback& callback);
	void RegNIMControlCb(const nim::Rts::ControlNotifyCallback& callback);
	void RegNIMRecDataCb(const nim::Rts::RecDataCallback& callback);
	void DoCallFailCb(nim::NIMResCode res_code);
	void DoNimAckCB(nim::NIMResCode res_code, const std::string session_id, int channel_type, bool accept);

	//Agora Invite REG CB
	void RegAgoraInviteReceivedCb(const AgoraInviteExtraCallback& callback);
	void RegAgoraInviteReceivedByPeerCb(const AgoraInviteCallback& callback);
	void RegAgoraInviteAcceptedByPeerCb(const AgoraInviteExtraCallback& callback);
	void RegAgoraInviteRefusedByPeerCb(const AgoraInviteExtraCallback& callback);
	void RegAgoraInviteEndByPeerCb(const AgoraInviteExtraCallback& callback);
	void RegAgoraInviteFailedCb(const AgoraInviteFailCallback& callback);
	void RegAgoraInviteEndByMyselfCb(const AgoraInviteCallback& callback);

	//Agora Chat Join CB
	void RegAgoraChatJoinSuccessCb(const AgoraJoinSuccessCallback& callback);
	void RegAgoraChatReJoinSuccessCb(const AgoraReJoinSuccessCallback& callback);
	void RegAgoraChatChannelErrorCb(const AgoraChannelErrorCallback& callback);
	void RegAgoraChatUserJoinCb(const AgoraUserJoinCallback& callback);
	void RegAgoraChatUserOfflineCb(const AgoraUserOfflineCallback& callback);
	void RegAgoraChatConnectLostCb(const AgoraConnectLostCallback& callback);
	void RegAgoraChatLeaveChannelCb(const AgoraLeaveChannelCallback& callback);
	void RegAgoraAudioQualityCb(const AgoraAudioQualityCallback& callback);				//注册声音质量回调
	void RegNetworkQualityCb(const AgoraNetworkQualityCallback& callback);				//注册频道网络质量回调

	//Agora Data Join CB
	void RegAgoraDataChannelJoinCb(const SignallingChannelJoinCallback& callback);
	void RegAgoraDataUserJoinedCb(const SignallingUserJoinedCallback& callback);
	void RegAgoraDataUserLeavedCb(const SignallingUserLeavedCallback& callback);
	void RegAgoraDataChannelLeavedCb(const SignallingChannelLeavedCallback& callback);
	void RegAgoraDataMsgInstantRecCb(const MsgInstantRecCallback& callback);
	void RegAgoraDataMsgChannelRecCb(const MsgChannelRecCallback& callback);
	void RegAgoraDataChannelJoinFailCb(const SignallingChannelJoinFailCallback& callback);
	void RegAgoraDataMsgSendErrorCb(const MsgSendErrorCallback& callback);
	void RegAgoraDataMsgSendSuccessCb(const MsgSendSuccessCallback& callback);

	//Agora Login Logout
	void RegAgoraLoginCb(const AgoraLoginCallback& callback);
	void RegAgoraLogoutCb(const AgoraLogoutCallback& callback);

private:
	BaseInvite* CreateInvite(NetworkServiceType type);
	void OnUnregisterInviteCallBack();

private:
	bool invite_status_;
	bool data_channel_status_;
	bool chat_channel_status_;

	One2OneCallInfo call_info_;
	BaseInvite* base_invite_;     //根据类型初始化邀请类

	//cb
	SignallingChannelJoinCallback session_id_cb_;    //网易多人白板加入后session_id会改变 先返回保存
	nim::Rts::CreateConfCallback nim_call_fail_cb_;
	nim::Rts::AckCallback nim_ack_cb_;
	UnregisterCallback restart_call_cb_;
	std::list<JoinChatChannelSuccessCallBack> join_chat_succ_cb_list_;
	std::list<UnregisterCallback>  register_cb_list_;    //所有网易 声网的注册回调都放一个容器  后面统一删
};
