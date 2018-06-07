#pragma once
#include "base\memory\singleton.h"
#include "base\callback\callback.h"
#include "base\util\auto_unregister.h"
#include <map>
#include <memory>
#include <functional>
#include <vector>
#include <stdint.h>
#include <string>

typedef std::function<void(std::string channel_id)>	SignallingChannelJoinCallback;
typedef std::function<void(std::string account, uint32_t uid)>	SignallingUserJoinedCallback;
typedef std::function<void(std::string account, uint32_t uid)>	SignallingUserLeavedCallback;
typedef std::function<void(std::string channel_id, int e_code)>	SignallingChannelLeavedCallback;
//typedef std::function<void(std::vector<std::string> accounts, std::vector<uint32_t> uids)>	SignallingUserListCallback;
typedef std::function<void(std::string account, uint32_t uid, std::string msg)>	MsgInstantRecCallback;
typedef std::function<void(std::string channel_id, std::string account, uint32_t uid, std::string msg)>	MsgChannelRecCallback;
typedef std::function<void(std::string channel_id, int ecode)>	SignallingChannelJoinFailCallback;
typedef std::function<void(std::string message_id, int ecode)>	MsgSendErrorCallback;
typedef std::function<void(std::string message_id)>	MsgSendSuccessCallback;

class AGSignallingChannelManager : public nbase::SupportWeakCallback
{
public:
	SINGLETON_DEFINE(AGSignallingChannelManager);
	AGSignallingChannelManager();
	~AGSignallingChannelManager();

	// reg cb
	UnregisterCallback RegChannelJoinCb(const SignallingChannelJoinCallback& callback);
	UnregisterCallback RegUserJoinedCb(const SignallingUserJoinedCallback& callback);
	UnregisterCallback RegUserLeavedCb(const SignallingUserLeavedCallback& callback);
	UnregisterCallback RegChannelLeavedCb(const SignallingChannelLeavedCallback& callback);
	//UnregisterCallback RegUserListCb(const SignallingUserListCallback& callback);
	UnregisterCallback RegMsgInstantRecCb(const MsgInstantRecCallback& callback);
	UnregisterCallback RegMsgChannelRecCb(const MsgChannelRecCallback& callback);
	UnregisterCallback RegChannelJoinFailCb(const SignallingChannelJoinFailCallback& callback);
	UnregisterCallback RegMsgSendErrorCb(const MsgSendErrorCallback& callback);
	UnregisterCallback RegMsgSendSuccessCb(const MsgSendSuccessCallback& callback);

	// cb
	void OnChannelJoined(std::string channel_id);
	void OnChannelUserJoined(std::string account, uint32_t uid);
	void OnChannelUserLeaved(std::string account, uint32_t uid);
	void OnChannelLeaved(std::string channel_id, int e_code);
	//void OnChannelUserList(std::vector<std::string> accounts, std::vector<uint32_t> uids);
	void OnMessageInstantReceive(std::string account, uint32_t uid, std::string msg);
	void OnMessageChannelReceive(std::string channel_id, std::string account, uint32_t uid, std::string msg);
	void OnChannelJoinFailed(std::string channel_id, int ecode);
	void OnMessageSendError(std::string message_id, int ecode);
	void OnMessageSendSuccess(std::string message_id);


	//
	void ChannelJoin(std::string channel_id);
	void ChannelLeave(std::string channel_id);
	void ChannelQueryUserNum(std::string channel_id);
	void MessageAppSend(std::string msg, std::string msg_id);
	void MessageInstantSend(std::string account, uint32_t uid, std::string msg, std::string msg_id);
	void MessageChannelSend(std::string channel_id, std::string msg, std::string msg_id);
	void MessageChannelSendFast(std::string channel_id, std::string msg, std::string msg_id);
	void MessagePushSend(std::string account, uint32_t uid, std::string msg, std::string msg_id);
	void MessageChatSend(std::string account, uint32_t uid, std::string msg, std::string msg_id);
	void MessageDTMFSend(uint32_t uid, std::string msg, std::string msg_id);

private:
	std::map<int, std::unique_ptr<SignallingChannelJoinCallback>> channel_join_cb_list_;
	std::map<int, std::unique_ptr<SignallingUserJoinedCallback>> user_join_cb_list_;
	std::map<int, std::unique_ptr<SignallingUserLeavedCallback>> user_leave_cb_list_;
	std::map<int, std::unique_ptr<SignallingChannelLeavedCallback>> channel_leave_cb_list_;
	//std::map<int, std::unique_ptr<SignallingUserListCallback>> user_list_cb_list_;
	std::map<int, std::unique_ptr<MsgInstantRecCallback>> msg_instant_rec_cb_list_;
	std::map<int, std::unique_ptr<MsgChannelRecCallback>> msg_channel_rec_cb_list_;
	std::map<int, std::unique_ptr<SignallingChannelJoinFailCallback>> channel_join_fail_cb_list_;
	std::map<int, std::unique_ptr<MsgSendErrorCallback>> msg_send_error_cb_list_;
	std::map<int, std::unique_ptr<MsgSendSuccessCallback>> msg_send_success_cb_list_;
};