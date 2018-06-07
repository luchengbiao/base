
#include "..\agora_signalling_sdk\manager\signalling_manager.h"
#include "agora_invite_manager.h"

AGInviteManager::AGInviteManager()
{

}

AGInviteManager::~AGInviteManager()
{

}

UnregisterCallback AGInviteManager::RegInviteReceivedCb(const AgoraInviteExtraCallback& callback)
{
	AgoraInviteExtraCallback* new_callback = new AgoraInviteExtraCallback(callback);
	int cb_id = (int)new_callback;
	rec_cb_list_[cb_id].reset(new_callback);

	auto cb = ToWeakCallback([this, cb_id]() {
		rec_cb_list_.erase(cb_id);
	});
	return cb;
}

UnregisterCallback AGInviteManager::RegInviteReceivedByPeerCb(const AgoraInviteCallback& callback)
{
	AgoraInviteCallback* new_callback = new AgoraInviteCallback(callback);
	int cb_id = (int)new_callback;
	rec_by_peer_cb_list_[cb_id].reset(new_callback);

	auto cb = ToWeakCallback([this, cb_id]() {
		rec_by_peer_cb_list_.erase(cb_id);
	});
	return cb;
}

UnregisterCallback AGInviteManager::RegInviteAcceptedByPeerCb(const AgoraInviteExtraCallback& callback)
{
	AgoraInviteExtraCallback* new_callback = new AgoraInviteExtraCallback(callback);
	int cb_id = (int)new_callback;
	accept_by_peer_cb_list_[cb_id].reset(new_callback);

	auto cb = ToWeakCallback([this, cb_id]() {
		accept_by_peer_cb_list_.erase(cb_id);
	});
	return cb;
}

UnregisterCallback AGInviteManager::RegInviteRefusedByPeerCb(const AgoraInviteExtraCallback& callback)
{
	AgoraInviteExtraCallback* new_callback = new AgoraInviteExtraCallback(callback);
	int cb_id = (int)new_callback;
	refuse_by_peer_cb_list_[cb_id].reset(new_callback);

	auto cb = ToWeakCallback([this, cb_id]() {
		refuse_by_peer_cb_list_.erase(cb_id);
	});
	return cb;
}

UnregisterCallback AGInviteManager::RegInviteEndByPeerCb(const AgoraInviteExtraCallback& callback)
{
	AgoraInviteExtraCallback* new_callback = new AgoraInviteExtraCallback(callback);
	int cb_id = (int)new_callback;
	end_by_peer_cb_list_[cb_id].reset(new_callback);

	auto cb = ToWeakCallback([this, cb_id]() {
		end_by_peer_cb_list_.erase(cb_id);
	});
	return cb;
}

UnregisterCallback AGInviteManager::RegInviteFailedCb(const AgoraInviteFailCallback& callback)
{
	AgoraInviteFailCallback* new_callback = new AgoraInviteFailCallback(callback);
	int cb_id = (int)new_callback;
	fail_cb_list_[cb_id].reset(new_callback);

	auto cb = ToWeakCallback([this, cb_id]() {
		fail_cb_list_.erase(cb_id);
	});
	return cb;
}

UnregisterCallback AGInviteManager::RegInviteEndByMyselfCb(const AgoraInviteCallback& callback)
{
	AgoraInviteCallback* new_callback = new AgoraInviteCallback(callback);
	int cb_id = (int)new_callback;
	end_by_myself_cb_list_[cb_id].reset(new_callback);

	auto cb = ToWeakCallback([this, cb_id]() {
		end_by_myself_cb_list_.erase(cb_id);
	});
	return cb;
}

void AGInviteManager::OnInviteReceivedCallback(std::string channel_id, std::string account, uint32_t uid, std::string extra_info)
{
	for (auto& it : rec_cb_list_)
	{
		(*(it.second))(channel_id, account, uid, extra_info);
	}
}

void AGInviteManager::OnInviteReceivedByPeerCallback(std::string channel_id, std::string account, uint32_t uid)
{
	for (auto& it : rec_by_peer_cb_list_)
	{
		(*(it.second))(channel_id, account, uid);
	}
}

void AGInviteManager::OnInviteAcceptedByPeerCallback(std::string channel_id, std::string account, uint32_t uid, std::string extra_info)
{
	for (auto& it : accept_by_peer_cb_list_)
	{
		(*(it.second))(channel_id, account, uid, extra_info);
	}
}

void AGInviteManager::OnInviteRefusedByPeerCallback(std::string channel_id, std::string account, uint32_t uid, std::string extra_info)
{
	for (auto& it : refuse_by_peer_cb_list_)
	{
		(*(it.second))(channel_id, account, uid, extra_info);
	}
}

void AGInviteManager::OnInviteEndByPeerCallback(std::string channel_id, std::string account, uint32_t uid, std::string extra_info)
{
	for (auto& it : end_by_peer_cb_list_)
	{
		(*(it.second))(channel_id, account, uid, extra_info);
	}
}

void AGInviteManager::OnInviteFailedCallback(std::string channel_id, std::string account, uint32_t uid, int ecode, std::string extra_info)
{
	for (auto& it : fail_cb_list_)
	{
		(*(it.second))(channel_id, account, uid, ecode, extra_info);
	}
}

void AGInviteManager::OnInviteEndByMyselfCallback(std::string channel_id, std::string account, uint32_t uid)
{
	for (auto& it : end_by_myself_cb_list_)
	{
		(*(it.second))(channel_id, account, uid);
	}
}

void AGInviteManager::ChannelInviteUser(std::string s_channelID, std::string s_account, uint32_t uid /*= 0*/)
{
	AGSignallingManager::GetAGSignalling()->ChannelInviteUser(s_channelID, s_account, uid);
}

void AGInviteManager::ChannelInviteUser(std::string s_channelID, std::string s_account, std::string s_extra)
{
	AGSignallingManager::GetAGSignalling()->ChannelInviteUser(s_channelID, s_account, s_extra);
}

void AGInviteManager::ChannelInvitePhone(std::string s_channelID, std::string s_phoneNum, uint32_t uid /*= 0*/)
{
	AGSignallingManager::GetAGSignalling()->ChannelInvitePhone(s_channelID, s_phoneNum, uid);
}

void AGInviteManager::ChannelInvitePhone(std::string s_channelID, std::string s_phoneNum, std::string s_sourcesNum)
{
	AGSignallingManager::GetAGSignalling()->ChannelInvitePhone(s_channelID, s_phoneNum, s_sourcesNum);
}

void AGInviteManager::ChannelInvitePhone(std::string s_channelID, std::string s_phoneNum, std::string s_sourcesNum, std::string s_extra)
{
	AGSignallingManager::GetAGSignalling()->ChannelInvitePhone(s_channelID, s_phoneNum, s_sourcesNum, s_extra);
}

void AGInviteManager::ChannelInviteDTMF(std::string s_channelID, std::string s_phoneNum, std::string s_dtmf)
{
	AGSignallingManager::GetAGSignalling()->ChannelInviteDTMF(s_channelID, s_phoneNum, s_dtmf);
}

void AGInviteManager::ChannelInviteAccept(std::string s_channelID, std::string s_account, uint32_t uid)
{
	AGSignallingManager::GetAGSignalling()->ChannelInviteAccept(s_channelID, s_account, uid);
}

void AGInviteManager::ChannelInviteRefuse(std::string s_channelID, std::string s_account, uint32_t uid, std::string s_extra)
{
	AGSignallingManager::GetAGSignalling()->ChannelInviteRefuse(s_channelID, s_account, uid, s_extra);
}

void AGInviteManager::ChannelInviteEnd(std::string s_channelID, std::string s_account, uint32_t uid)
{
	AGSignallingManager::GetAGSignalling()->ChannelInviteEnd(s_channelID, s_account, uid);
}
