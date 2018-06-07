#include "agora_signalling_channel_callback.h"
#include "manager\agora_signalling_channel_manager.h"

void AGSignallingChannelCallback::OnChannelJoined(std::string channel_id)
{
	AGSignallingChannelManager::GetInstance()->OnChannelJoined(channel_id);
}

void AGSignallingChannelCallback::OnChannelUserJoined(std::string account, uint32_t uid)
{
	AGSignallingChannelManager::GetInstance()->OnChannelUserJoined(account, uid);
}

void AGSignallingChannelCallback::OnChannelUserLeaved(std::string account, uint32_t uid)
{
	AGSignallingChannelManager::GetInstance()->OnChannelUserLeaved(account, uid);
}

void AGSignallingChannelCallback::OnChannelLeaved(std::string channel_id, int e_code)
{
	AGSignallingChannelManager::GetInstance()->OnChannelLeaved(channel_id, e_code);
}

void AGSignallingChannelCallback::OnChannelUserList(std::vector<std::string> accounts, std::vector<uint32_t> uids)
{
	//AGSignallingChannelManager::GetInstance()->OnChannelUserList(accounts, uids);
}

void AGSignallingChannelCallback::OnMessageInstantReceive(std::string account, uint32_t uid, std::string msg)
{
	AGSignallingChannelManager::GetInstance()->OnMessageInstantReceive(account, uid, msg);
}

void AGSignallingChannelCallback::OnMessageChannelReceive(std::string channel_id, std::string account, uint32_t uid, std::string msg)
{
	AGSignallingChannelManager::GetInstance()->OnMessageChannelReceive(channel_id, account, uid, msg);
}

void AGSignallingChannelCallback::OnChannelJoinFailed(std::string channel_id, int ecode)
{
	AGSignallingChannelManager::GetInstance()->OnChannelJoinFailed(channel_id, ecode);
}

void AGSignallingChannelCallback::OnMessageSendError(std::string message_id, int ecode)
{
	AGSignallingChannelManager::GetInstance()->OnMessageSendError(message_id, ecode);
}

void AGSignallingChannelCallback::OnMessageSendSuccess(std::string message_id)
{
	AGSignallingChannelManager::GetInstance()->OnMessageSendSuccess(message_id);
}

