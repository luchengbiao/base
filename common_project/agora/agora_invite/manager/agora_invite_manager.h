#pragma once
#include "base\memory\singleton.h"
#include "base\callback\callback.h"
#include "base\util\auto_unregister.h"
#include <map>
#include <memory>
#include <functional>

typedef std::function<void(std::string channel_id, std::string account, uint32_t uid, std::string extra_info)>	AgoraInviteExtraCallback;
typedef std::function<void(std::string channel_id, std::string account, uint32_t uid)>	AgoraInviteCallback;
typedef std::function<void(std::string channel_id, std::string account, uint32_t uid, int ecode, std::string extra_info)>	AgoraInviteFailCallback;

class AGInviteManager : public nbase::SupportWeakCallback
{
public:
	SINGLETON_DEFINE(AGInviteManager);
	AGInviteManager();
	~AGInviteManager();

	// reg cb
	UnregisterCallback RegInviteReceivedCb(const AgoraInviteExtraCallback& callback);
	UnregisterCallback RegInviteReceivedByPeerCb(const AgoraInviteCallback& callback);
	UnregisterCallback RegInviteAcceptedByPeerCb(const AgoraInviteExtraCallback& callback);
	UnregisterCallback RegInviteRefusedByPeerCb(const AgoraInviteExtraCallback& callback);
	UnregisterCallback RegInviteEndByPeerCb(const AgoraInviteExtraCallback& callback);
	UnregisterCallback RegInviteFailedCb(const AgoraInviteFailCallback& callback);
	UnregisterCallback RegInviteEndByMyselfCb(const AgoraInviteCallback& callback);

	// cb
	void OnInviteReceivedCallback(std::string channel_id, std::string account, uint32_t uid, std::string extra_info);
	void OnInviteReceivedByPeerCallback(std::string channel_id, std::string account, uint32_t uid);
	void OnInviteAcceptedByPeerCallback(std::string channel_id, std::string account, uint32_t uid, std::string extra_info);
	void OnInviteRefusedByPeerCallback(std::string channel_id, std::string account, uint32_t uid, std::string extra_info);
	void OnInviteEndByPeerCallback(std::string channel_id, std::string account, uint32_t uid, std::string extra_info);
	void OnInviteFailedCallback(std::string channel_id, std::string account, uint32_t uid, int ecode, std::string extra_info);
	void OnInviteEndByMyselfCallback(std::string channel_id, std::string account, uint32_t uid);

	// 
	void ChannelInviteUser(std::string s_channelID, std::string s_account, uint32_t uid = 0);
	void ChannelInviteUser(std::string s_channelID, std::string s_account, std::string s_extra);
	void ChannelInvitePhone(std::string s_channelID, std::string s_phoneNum, uint32_t uid = 0);
	void ChannelInvitePhone(std::string s_channelID, std::string s_phoneNum, std::string s_sourcesNum);
	void ChannelInvitePhone(std::string s_channelID, std::string s_phoneNum, std::string s_sourcesNum, std::string s_extra);
	void ChannelInviteDTMF(std::string s_channelID, std::string s_phoneNum, std::string s_dtmf);
	void ChannelInviteAccept(std::string s_channelID, std::string s_account, uint32_t uid);
	void ChannelInviteRefuse(std::string s_channelID, std::string s_account, uint32_t uid, std::string s_extra);
	void ChannelInviteEnd(std::string s_channelID, std::string s_account, uint32_t uid);

private:
	std::map<int, std::unique_ptr<AgoraInviteExtraCallback>> rec_cb_list_;
	std::map<int, std::unique_ptr<AgoraInviteCallback>> rec_by_peer_cb_list_;
	std::map<int, std::unique_ptr<AgoraInviteExtraCallback>> accept_by_peer_cb_list_;
	std::map<int, std::unique_ptr<AgoraInviteExtraCallback>> refuse_by_peer_cb_list_;
	std::map<int, std::unique_ptr<AgoraInviteExtraCallback>> end_by_peer_cb_list_;
	std::map<int, std::unique_ptr<AgoraInviteFailCallback>> fail_cb_list_;
	std::map<int, std::unique_ptr<AgoraInviteCallback>> end_by_myself_cb_list_;
};
