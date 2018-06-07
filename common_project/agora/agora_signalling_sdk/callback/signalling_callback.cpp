#include "signalling_callback.h"
#include "qthread_manager\closure.h"
#include "..\agora_login\callback\agora_login_callback.h"
#include "..\agora_signalling_channel\callback\agora_signalling_channel_callback.h"
#include "..\agora_invite\callback\agora_invite_callback.h"

AGSignallingCallBack::AGSignallingCallBack()
{

}

AGSignallingCallBack::~AGSignallingCallBack()
{
	
}

void AGSignallingCallBack::onLoginSuccess(uint32_t uid, int fd)
{
	auto task = [=]()
	{
		AGLoginCallback::OnLoginSuccessCallback(uid, fd);
	};
	qtbase::Post2Task(kThreadUIHelper, task);
}

void AGSignallingCallBack::onMessageAppReceived(char const * msg, size_t msg_size)
{

}

void AGSignallingCallBack::onChannelQueryUserNumResult(char const * channelID, size_t channelID_size, int ecode, int num)
{

}

void AGSignallingCallBack::onLoginFailed(int ecode)
{
	auto task = [=]()
	{
		AGLoginCallback::OnLoginFailedCallback(ecode);
	};
	qtbase::Post2Task(kThreadUIHelper, task);
}

void AGSignallingCallBack::onLogout(int ecode)
{
	auto task = [=]()
	{
		AGLoginCallback::OnLogoutCallback(ecode);
	};
	qtbase::Post2Task(kThreadUIHelper, task);
}

void AGSignallingCallBack::onChannelJoined(char const * channelID, size_t channelID_size)
{
	std::string s_channel_id = std::string(channelID);
	auto task = [=]()
	{
		AGSignallingChannelCallback::OnChannelJoined(s_channel_id);
	};
	qtbase::Post2Task(kThreadUIHelper, task);
}

void AGSignallingCallBack::onChannelUserJoined(char const * account, size_t account_size, uint32_t uid)
{
	std::string s_account = std::string(account);
	auto task = [=]()
	{
		AGSignallingChannelCallback::OnChannelUserJoined(s_account, uid);
	};
	qtbase::Post2Task(kThreadUIHelper, task);
}

void AGSignallingCallBack::onChannelUserLeaved(char const * account, size_t account_size, uint32_t uid)
{
	std::string s_account = std::string(account);
	auto task = [=]()
	{
		AGSignallingChannelCallback::OnChannelUserLeaved(s_account, uid);
	};
	qtbase::Post2Task(kThreadUIHelper, task);
}

void AGSignallingCallBack::onChannelLeaved(char const * channelID, size_t channelID_size, int e_code)
{
	std::string s_channel_id = std::string(channelID);
	auto task = [=]()
	{
		AGSignallingChannelCallback::OnChannelUserLeaved(s_channel_id, e_code);
	};
	qtbase::Post2Task(kThreadUIHelper, task);
}

void AGSignallingCallBack::onChannelUserList(int n, char **accounts, uint32_t* uids)
{

}

void AGSignallingCallBack::onInviteReceived(char const * channelID, size_t channelID_size, char const * account, size_t account_size,
	uint32_t uid, char const * extra, size_t extra_size)
{
	std::string s_channel_id = std::string(channelID);
	std::string s_account = std::string(account);
	std::string s_extra = std::string(extra);
	auto task = [=]()
	{
		AGInviteCallback::OnInviteReceived(s_channel_id, s_account, uid, s_extra);
	};
	qtbase::Post2Task(kThreadUIHelper, task);
}

void AGSignallingCallBack::onInviteReceivedByPeer(char const * channelID, size_t channelID_size, char const * account, size_t account_size, uint32_t uid)
{
	std::string s_channel_id = std::string(channelID);
	std::string s_account = std::string(account);
	auto task = [=]()
	{
		AGInviteCallback::OnInviteReceivedByPeer(s_channel_id, s_account, uid);
	};
	qtbase::Post2Task(kThreadUIHelper, task);

}

void AGSignallingCallBack::onInviteAcceptedByPeer(char const * channelID, size_t channelID_size, char const * account, size_t account_size,
	uint32_t uid, char const * extra, size_t extra_size)
{
	std::string s_channel_id = std::string(channelID);
	std::string s_account = std::string(account);
	std::string s_extra = std::string(extra);
	auto task = [=]()
	{
		AGInviteCallback::OnInviteAcceptedByPeer(s_channel_id, s_account, uid, s_extra);
	};
	qtbase::Post2Task(kThreadUIHelper, task);
}

void AGSignallingCallBack::onInviteRefusedByPeer(char const * channelID, size_t channelID_size, char const * account, size_t account_size,
	uint32_t uid, char const * extra, size_t extra_size)
{
	std::string s_channel_id = std::string(channelID);
	std::string s_account = std::string(account);
	std::string s_extra = std::string(extra);
	auto task = [=]()
	{
		AGInviteCallback::OnInviteRefusedByPeer(s_channel_id, s_account, uid, s_extra);
	};
	qtbase::Post2Task(kThreadUIHelper, task);
}

void AGSignallingCallBack::onInviteEndByPeer(char const * channelID, size_t channelID_size, char const * account, size_t account_size,
	uint32_t uid, char const * extra, size_t extra_size)
{
	std::string s_channel_id = std::string(channelID);
	std::string s_account = std::string(account);
	std::string s_extra = std::string(extra);
	auto task = [=]()
	{
		AGInviteCallback::OnInviteEndByPeer(s_channel_id, s_account, uid, s_extra);
	};
	qtbase::Post2Task(kThreadUIHelper, task);
}

void AGSignallingCallBack::onLog(char const * txt, size_t txt_size)
{

}

void AGSignallingCallBack::onMessageInstantReceive(char const * account, size_t account_size, uint32_t uid, char const * msg, size_t msg_size)
{
	std::string s_account = std::string(account);
	std::string s_msg = std::string(msg);
	
	auto task = [=]()
	{
		AGSignallingChannelCallback::OnMessageInstantReceive(s_account, uid, s_msg);
	};
	qtbase::Post2Task(kThreadUIHelper, task);
}

void AGSignallingCallBack::onMessageChannelReceive(char const * channelID, size_t channelID_size,
	char const * account, size_t account_size, uint32_t uid, char const * msg, size_t msg_size)
{
	std::string s_channel_id = std::string(channelID);
	std::string s_account = std::string(account);
	std::string s_msg = std::string(msg);
	auto task = [=]()
	{
		AGSignallingChannelCallback::OnMessageChannelReceive(s_channel_id, s_account, uid, s_msg);
	};
	qtbase::Post2Task(kThreadUIHelper, task);
}

void AGSignallingCallBack::onInviteFailed(char const * channelID, size_t channelID_size, char const * account, size_t account_size, 
	uint32_t uid, int ecode, char const * extra, size_t extra_size)
{
	std::string s_channel_id = std::string(channelID);
	std::string s_account = std::string(account);
	std::string s_extra = std::string(extra);
	auto task = [=]()
	{
		AGInviteCallback::OnInviteFailed(s_channel_id, s_account, uid, ecode, s_extra);
	};
	qtbase::Post2Task(kThreadUIHelper, task);
}

void AGSignallingCallBack::onInviteEndByMyself(char const * channelID, size_t channelID_size, char const * account, size_t account_size, uint32_t uid)
{
	std::string s_channel_id = std::string(channelID);
	std::string s_account = std::string(account);
	auto task = [=]()
	{
		AGInviteCallback::OnInviteEndByMyself(s_channel_id, s_account, uid);
	};
	qtbase::Post2Task(kThreadUIHelper, task);
}

void AGSignallingCallBack::onReconnecting(uint32_t nretry)
{
	
}

void AGSignallingCallBack::onReconnected(int fd)
{

}

void AGSignallingCallBack::onChannelJoinFailed(char const * channelID, size_t channelID_size, int ecode)
{
	std::string s_channel_id = std::string(channelID);
	auto task = [=]()
	{
		AGSignallingChannelCallback::OnChannelJoinFailed(s_channel_id, ecode);
	};
	qtbase::Post2Task(kThreadUIHelper, task);
}

void AGSignallingCallBack::onMessageSendError(char const * messageID, size_t messageID_size, int ecode)
{
	std::string s_msg_id = "";
	if (messageID && messageID_size > 0)
	{
		s_msg_id = std::string(messageID);
	}
	auto task = [=]()
	{
		AGSignallingChannelCallback::OnChannelJoinFailed(s_msg_id, ecode);
	};
	qtbase::Post2Task(kThreadUIHelper, task);
}

void AGSignallingCallBack::onMessageSendSuccess(char const * messageID, size_t messageID_size)
{
	std::string s_msg_id = "";
	if (messageID && messageID_size > 0)
	{
		s_msg_id = std::string(messageID);
	}
	auto task = [=]()
	{
		AGSignallingChannelCallback::OnMessageSendSuccess(s_msg_id);
	};
	qtbase::Post2Task(kThreadUIHelper, task);
}
