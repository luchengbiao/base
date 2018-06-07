#pragma once
#include <stdint.h>
#include <string>

class AGInviteCallback
{
public:
	static void OnInviteReceived(std::string channel_id, std::string account, uint32_t uid, std::string extra_info);

	static void OnInviteReceivedByPeer(std::string channel_id, std::string account, uint32_t uid);

	static void OnInviteAcceptedByPeer(std::string channel_id, std::string account, uint32_t uid, std::string extra_info);

	static void OnInviteRefusedByPeer(std::string channel_id, std::string account, uint32_t uid, std::string extra_info);

	static void OnInviteEndByPeer(std::string channel_id, std::string account, uint32_t uid, std::string extra_info);

	static void OnInviteFailed(std::string channel_id, std::string account, uint32_t uid, int ecode, std::string extra_info);

	static void OnInviteEndByMyself(std::string channel_id, std::string account, uint32_t uid);
};