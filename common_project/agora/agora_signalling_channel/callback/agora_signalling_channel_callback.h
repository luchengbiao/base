#pragma once
#include <stdint.h>
#include <string>
#include <vector>

class AGSignallingChannelCallback
{
public:
	static void OnChannelJoined(std::string channel_id);

	static void OnChannelUserJoined(std::string account, uint32_t uid);

	static void OnChannelUserLeaved(std::string account, uint32_t uid);

	static void OnChannelLeaved(std::string channel_id, int e_code);

	static void OnChannelUserList(std::vector<std::string> accounts, std::vector<uint32_t> uids);

	static void OnMessageInstantReceive(std::string account, uint32_t uid, std::string msg);

	static void OnMessageChannelReceive(std::string channel_id, std::string account, uint32_t uid, std::string msg);

	static void OnChannelJoinFailed(std::string channel_id, int ecode);

	static void OnMessageSendError(std::string message_id, int ecode);

	static void OnMessageSendSuccess(std::string message_id);
};