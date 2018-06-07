#pragma once
#include <stdint.h>
#include <string>

class AGVchatCallback
{
public:
	static void OnJoinChannelSuccess(std::string channel_id, uint32_t uid, int elapsed);
	static void OnRejoinChannelSuccess(std::string channel_id, uint32_t uid, int elapsed);
	static void OnError(int err, std::string msg);
	static void OnLeaveChannel();
	static void OnUserJoined(uint32_t uid, int elapsed);
	static void OnUserOffline(uint32_t uid);
	static void OnConnectionLost();
	static void onAudioQuality(uint32_t uid, int quality, uint16_t delay, uint16_t lost);
	static void onNetworkQuality(uint32_t uid, int txQuality, int rxQuality);
};