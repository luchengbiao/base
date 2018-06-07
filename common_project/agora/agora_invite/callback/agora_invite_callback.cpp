#include "agora_invite_callback.h"
#include "manager\agora_invite_manager.h"


void AGInviteCallback::OnInviteReceived(std::string channel_id, std::string account, uint32_t uid, std::string extra_info)
{
	AGInviteManager::GetInstance()->OnInviteReceivedCallback(channel_id, account, uid, extra_info);
}

void AGInviteCallback::OnInviteReceivedByPeer(std::string channel_id, std::string account, uint32_t uid)
{
	AGInviteManager::GetInstance()->OnInviteReceivedByPeerCallback(channel_id, account, uid);
}

void AGInviteCallback::OnInviteAcceptedByPeer(std::string channel_id, std::string account, uint32_t uid, std::string extra_info)
{
	AGInviteManager::GetInstance()->OnInviteAcceptedByPeerCallback(channel_id, account, uid, extra_info);
}

void AGInviteCallback::OnInviteRefusedByPeer(std::string channel_id, std::string account, uint32_t uid, std::string extra_info)
{
	AGInviteManager::GetInstance()->OnInviteRefusedByPeerCallback(channel_id, account, uid, extra_info);
}

void AGInviteCallback::OnInviteEndByPeer(std::string channel_id, std::string account, uint32_t uid, std::string extra_info)
{
	AGInviteManager::GetInstance()->OnInviteEndByPeerCallback(channel_id, account, uid, extra_info);
}

void AGInviteCallback::OnInviteFailed(std::string channel_id, std::string account, uint32_t uid, int ecode, std::string extra_info)
{
	AGInviteManager::GetInstance()->OnInviteFailedCallback(channel_id, account, uid, ecode, extra_info);
}

void AGInviteCallback::OnInviteEndByMyself(std::string channel_id, std::string account, uint32_t uid)
{
	AGInviteManager::GetInstance()->OnInviteEndByMyselfCallback(channel_id, account, uid);
}
