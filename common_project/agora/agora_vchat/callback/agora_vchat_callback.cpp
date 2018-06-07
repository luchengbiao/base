#include "agora_vchat_callback.h"
#include "manager\agora_vchat_manager.h"

void AGVchatCallback::OnJoinChannelSuccess(std::string channel_id, uint32_t uid, int elapsed)
{
	AGVchatManager::GetInstance()->OnJoinChannelSuccess(channel_id, uid, elapsed);
}

void AGVchatCallback::OnRejoinChannelSuccess(std::string channel_id, uint32_t uid, int elapsed)
{
	AGVchatManager::GetInstance()->OnRejoinChannelSuccess(channel_id, uid, elapsed);
}

void AGVchatCallback::OnError(int err, std::string msg)
{
	AGVchatManager::GetInstance()->OnError(err, msg);
}

void AGVchatCallback::OnLeaveChannel()
{
	AGVchatManager::GetInstance()->OnLeaveChannel();
}

void AGVchatCallback::OnUserJoined(uint32_t uid, int elapsed)
{
	AGVchatManager::GetInstance()->OnUserJoined(uid, elapsed);
}

void AGVchatCallback::OnUserOffline(uint32_t uid)
{
	AGVchatManager::GetInstance()->OnUserOffline(uid);
}

void AGVchatCallback::OnConnectionLost()
{
	AGVchatManager::GetInstance()->OnConnectionLost();
}

void AGVchatCallback::onAudioQuality(uint32_t uid, int quality, uint16_t delay, uint16_t lost)
{
	AGVchatManager::GetInstance()->onAudioQuality(uid, quality, delay, lost);
}

void AGVchatCallback::onNetworkQuality(uint32_t uid, int txQuality, int rxQuality)
{
	AGVchatManager::GetInstance()->onNetworkQuality(uid, txQuality, rxQuality);
}

