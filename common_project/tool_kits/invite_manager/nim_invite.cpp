#include "nim_invite.h"
#include "invite_manager.h"
#include "base\util\string_util.h"
#include "nim_sdk_manager\api\nim_cpp_rts.h"
#include "nim_rts\manager\rts_manager.h"
#include "qthread_manager\closure.h"
#include "nim_base\util.h"
#include "..\class_log_manager\client_log.h"

NimInvite::NimInvite()
{

}

NimInvite::~NimInvite()
{
	call_repeat_.Cancel();
}

void NimInvite::OnInitInvite(One2OneCallInfo info, bool b_force_join)
{
	info_ = info;

	StartInvite();
}

void NimInvite::StartInvite()
{
	if (!info_.b_creator_)
	{
		session_id_ = info_.nim_session_id_;
		course_channel_id_ = info_.nim_rts_channel_id_;
		return;
	}

	if (session_id_.empty()){
		//TODO
		session_id_ = QNimString::GetGUID();
	}
	std::string apns("");

	apns.append(nbase::UTF16ToUTF8(L"呼叫您进行在线辅导"));

	nim::Rts::StartChannelCallback cb = nbase::Bind(&NimInvite::OnStartRtsCb,
		this, session_id_, std::placeholders::_1, std::placeholders::_2,
		std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);

	nim::RtsStartInfo rts_info;
	rts_info.apns_ = apns;
	rts_info.custom_info_ = InviteManager::GetInstance()->GetCallExternInfo();
	rts_info.data_record_ = true;
	rts_info.audio_record_ = true;
	rts_info.webrtc_ = false;

	nim_comp::RtsManager::GetInstance()->StartChannel(nim::kNIMRtsChannelTypeTcp | nim::kNIMRtsChannelTypeVchat, info_.peer_nim_account_, rts_info, cb);
	call_repeat_.Cancel();
	StdClosure closure2 = nbase::Bind(&NimInvite::StartInvite, this);
	auto week_callback = call_repeat_.ToWeakCallback(closure2);
	qtbase::Post2RepeatedTask(kThreadUIHelper, week_callback, nbase::TimeDelta::FromMilliseconds(3000));
}

void NimInvite::AcceptInvite()
{
	auto cb = ToWeakCallback([=](nim::NIMResCode res_code, const std::string session_id, int channel_type, bool accept){
		InviteManager::GetInstance()->DoNimAckCB(res_code, session_id, channel_type, accept);
	});
	nim_comp::RtsManager::GetInstance()->Ack(session_id_, nim::kNIMRtsChannelTypeTcp | nim::kNIMRtsChannelTypeVchat, true, true, true,
		nbase::Bind(cb, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
}

void NimInvite::RefuseInvite()
{
	auto cb = ToWeakCallback([=](nim::NIMResCode res_code, const std::string session_id, int channel_type, bool accept){
		InviteManager::GetInstance()->DoNimAckCB(res_code, session_id, channel_type, accept);
	});
	nim_comp::RtsManager::GetInstance()->Ack(session_id_, nim::kNIMRtsChannelTypeTcp | nim::kNIMRtsChannelTypeVchat, false, true, true,
		nbase::Bind(cb, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
}

void NimInvite::CancelInvite()
{
	nim_comp::RtsManager::GetInstance()->Hangup(session_id_, nim::Rts::HangupCallback());
}

void NimInvite::EndInvite(std::string peer_account)
{
	nim_comp::RtsManager::GetInstance()->Ack(session_id_, nim::kNIMRtsChannelTypeTcp | nim::kNIMRtsChannelTypeVchat, false, true, true, nim::Rts::AckCallback());
}

void NimInvite::OnStartRtsCb(std::string old_id, nim::NIMResCode res_code,
	const std::string session_id, int channel_type, const std::string uid, const std::string channel_id)
{
	if (session_id_ == old_id)
	{
		session_id_ = session_id;
		InviteManager::GetInstance()->DoNimSessionIdCb(session_id_);
		course_channel_id_ = channel_id;
		if (res_code == nim::kNIMResSuccess){
			call_repeat_.Cancel();
			InviteManager::GetInstance()->OnClearAllErrorStatus();
		}
		else{
			call_repeat_.Cancel();
			InviteManager::GetInstance()->DoCallFailCb(res_code);
			InviteManager::GetInstance()->UpdateInviteStatus(false);
			//InviteManager::GetInstance()->RestartCalling();
		}
	}
}

void NimInvite::CancelAllService()
{
	call_repeat_.Cancel();
}

void NimInvite::EndFailInvite(bool b_creator)
{
	if (!b_creator)
	{
		nim_comp::RtsManager::GetInstance()->Ack(session_id_, nim::kNIMRtsChannelTypeTcp | nim::kNIMRtsChannelTypeVchat, false, true, true, nim::Rts::AckCallback());
	}

	nim_comp::RtsManager::GetInstance()->Hangup(session_id_, nim::Rts::HangupCallback());
}

bool NimInvite::GetDetail(BaseInviteDetail& detail)
{
	detail.nim_channel_id = course_channel_id_;
	return true;
}

void NimInvite::StartInviteByAccount(std::string account)
{

}




