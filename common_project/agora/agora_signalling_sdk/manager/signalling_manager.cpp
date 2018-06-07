#include "signalling_manager.h"
#include "callback\signalling_callback.h"

agora_sdk_win::ICallBack* AGSignallingManager::m_lpSignallingEventHandler = NULL;
agora_sdk_win::IAgoraAPI * AGSignallingManager::m_lpAgoraInst = NULL;
AGSignallingManager* AGSignallingManager::m_lpAgoraSignalling = NULL;

AGSignallingManager::AGSignallingManager(void)
{

}

AGSignallingManager::~AGSignallingManager(void)
{

}

void AGSignallingManager::Login(std::string s_vendorID, std::string s_account, std::string s_token, uint32_t uid, std::string s_deviceID)
{
	if (m_lpAgoraInst)
	{
		m_lpAgoraInst->login(s_vendorID.c_str(), s_vendorID.size(), s_account.c_str(), s_account.size(),
			s_token.c_str(), s_token.size(), uid, s_deviceID.c_str(), s_deviceID.size());
	}
}

void AGSignallingManager::Login(std::string s_vendorID, std::string s_account, std::string s_token, uint32_t uid, std::string s_deviceID, int retry_time_in_s, int retry_count)
{
	if (m_lpAgoraInst)
	{
		m_lpAgoraInst->login2(s_vendorID.c_str(), s_vendorID.size(), s_account.c_str(), s_account.size(),
			s_token.c_str(), s_token.size(), uid, s_deviceID.c_str(), s_deviceID.size(), retry_time_in_s, retry_count);
	}
}

void AGSignallingManager::Logout()
{
	if (m_lpAgoraInst)
	{
		m_lpAgoraInst->logout();
	}
}

void AGSignallingManager::ChannelJoin(std::string s_channelID)
{
	if (m_lpAgoraInst)
	{
		m_lpAgoraInst->channelJoin(s_channelID.c_str(), s_channelID.size());
	}
}

void AGSignallingManager::ChannelLeave(std::string s_channelID)
{
	if (m_lpAgoraInst)
	{
		m_lpAgoraInst->channelLeave(s_channelID.c_str(), s_channelID.size());
	}
}

void AGSignallingManager::ChannelQueryUserNum(std::string s_channelID)
{
	if (m_lpAgoraInst)
	{
		m_lpAgoraInst->channelQueryUserNum(s_channelID.c_str(), s_channelID.size());
	}
}

void AGSignallingManager::ChannelSetAttr(std::string s_channelID, std::string s_name, std::string s_value)
{
	if (m_lpAgoraInst)
	{
		m_lpAgoraInst->channelSetAttr(s_channelID.c_str(), s_channelID.size(), s_name.c_str(), s_name.size(), s_value.c_str(), s_value.size());
	}
}

void AGSignallingManager::ChannelDelAttr(std::string s_channelID, std::string s_name)
{
	if (m_lpAgoraInst)
	{
		m_lpAgoraInst->channelDelAttr(s_channelID.c_str(), s_channelID.size(), s_name.c_str(), s_name.size());
	}
}

void AGSignallingManager::ChannelClearAttr(std::string s_channelID)
{
	if (m_lpAgoraInst)
	{
		m_lpAgoraInst->channelClearAttr(s_channelID.c_str(), s_channelID.size());
	}
}

void AGSignallingManager::ChannelInviteUser(std::string s_channelID, std::string s_account, uint32_t uid /*= 0*/)
{
	if (m_lpAgoraInst)
	{
		m_lpAgoraInst->channelInviteUser(s_channelID.c_str(), s_channelID.size(), s_account.c_str(), s_account.size(), uid);
	}
}

void AGSignallingManager::ChannelInviteUser(std::string s_channelID, std::string s_account, std::string s_extra)
{
	if (m_lpAgoraInst)
	{
		m_lpAgoraInst->channelInviteUser2(s_channelID.c_str(), s_channelID.size(), s_account.c_str(), s_account.size(), s_extra.c_str(), s_extra.size());
	}
}

void AGSignallingManager::ChannelInvitePhone(std::string s_channelID, std::string s_phoneNum, uint32_t uid /*= 0*/)
{
	if (m_lpAgoraInst)
	{
		m_lpAgoraInst->channelInvitePhone(s_channelID.c_str(), s_channelID.size(), s_phoneNum.c_str(), s_phoneNum.size(), uid);
	}
}

void AGSignallingManager::ChannelInvitePhone(std::string s_channelID, std::string s_phoneNum, std::string s_sourcesNum)
{
	if (m_lpAgoraInst)
	{
		m_lpAgoraInst->channelInvitePhone2(s_channelID.c_str(), s_channelID.size(), s_phoneNum.c_str(), s_phoneNum.size(), s_sourcesNum.c_str(), s_sourcesNum.size());
	}
}

void AGSignallingManager::ChannelInvitePhone(std::string s_channelID, std::string s_phoneNum, std::string s_sourcesNum, std::string s_extra)
{
	if (m_lpAgoraInst)
	{
		m_lpAgoraInst->channelInvitePhone3(s_channelID.c_str(), s_channelID.size(), s_phoneNum.c_str(), s_phoneNum.size(), 
			s_sourcesNum.c_str(), s_sourcesNum.size(), s_extra.c_str(), s_extra.size());
	}
}

void AGSignallingManager::ChannelInviteDTMF(std::string s_channelID, std::string s_phoneNum, std::string s_dtmf)
{
	if (m_lpAgoraInst)
	{
		m_lpAgoraInst->channelInviteDTMF(s_channelID.c_str(), s_channelID.size(), s_phoneNum.c_str(), s_phoneNum.size(),
			s_dtmf.c_str(), s_dtmf.size());
	}
}

void AGSignallingManager::ChannelInviteAccept(std::string s_channelID, std::string s_account, uint32_t uid)
{
	if (m_lpAgoraInst)
	{
		m_lpAgoraInst->channelInviteAccept(s_channelID.c_str(), s_channelID.size(), s_account.c_str(), s_account.size(), uid);
	}
}

void AGSignallingManager::ChannelInviteRefuse(std::string s_channelID, std::string s_account, uint32_t uid, std::string s_extra)
{
	if (m_lpAgoraInst)
	{
		m_lpAgoraInst->channelInviteRefuse(s_channelID.c_str(), s_channelID.size(), s_account.c_str(), s_account.size(), uid, s_extra.c_str(), s_extra.size());
	}
}

void AGSignallingManager::ChannelInviteEnd(std::string s_channelID, std::string s_account, uint32_t uid)
{
	if (m_lpAgoraInst)
	{
		m_lpAgoraInst->channelInviteEnd(s_channelID.c_str(), s_channelID.size(), s_account.c_str(), s_account.size(), uid);
	}
}

void AGSignallingManager::MessageAppSend(std::string s_msg, std::string s_msgID)
{
	if (m_lpAgoraInst)
	{
		m_lpAgoraInst->messageAppSend(s_msg.c_str(), s_msg.size(), s_msgID.c_str(), s_msgID.size());
	}
}

void AGSignallingManager::MessageInstantSend(std::string s_account, uint32_t uid, std::string s_msg, std::string s_msgID)
{
	if (m_lpAgoraInst)
	{
		m_lpAgoraInst->messageInstantSend(s_account.c_str(), s_account.size(), uid, s_msg.c_str(), s_msg.size(), s_msgID.c_str(), s_msgID.size());
	}
}

void AGSignallingManager::MessageChannelSend(std::string s_channelID, std::string s_msg, std::string s_msgID)
{
	if (m_lpAgoraInst)
	{
		m_lpAgoraInst->messageChannelSend(s_channelID.c_str(), s_channelID.size(), s_msg.c_str(), s_msg.size(), s_msgID.c_str(), s_msgID.size());
	}
}

void AGSignallingManager::MessageChannelSendFast(std::string s_channelID, std::string s_msg, std::string s_msgID)
{
	if (m_lpAgoraInst)
	{
		//m_lpAgoraInst->messageChannelSendFast(s_channelID.c_str(), s_channelID.size(), s_msg.c_str(), s_msg.size(), s_msgID.c_str(), s_msgID.size());
	}
}

void AGSignallingManager::MessagePushSend(std::string s_account, uint32_t uid, std::string s_msg, std::string s_msgID)
{
	if (m_lpAgoraInst)
	{
		m_lpAgoraInst->messagePushSend(s_account.c_str(), s_account.size(), uid, s_msg.c_str(), s_msg.size(), s_msgID.c_str(), s_msgID.size());
	}
}

void AGSignallingManager::MessageChatSend(std::string s_account, uint32_t uid, std::string s_msg, std::string s_msgID)
{
	if (m_lpAgoraInst)
	{
		m_lpAgoraInst->messageChatSend(s_account.c_str(), s_account.size(), uid, s_msg.c_str(), s_msg.size(), s_msgID.c_str(), s_msgID.size());
	}
}

void AGSignallingManager::MessageDTMFSend(uint32_t uid, std::string s_msg, std::string s_msgID)
{
	if (m_lpAgoraInst)
	{
		m_lpAgoraInst->messageDTMFSend(uid, s_msg.c_str(), s_msg.size(), s_msgID.c_str(), s_msgID.size());
	}
}

void AGSignallingManager::SetBackground(uint32_t bOut)
{
	if (m_lpAgoraInst)
	{
		m_lpAgoraInst->setBackground(bOut);
	}
}

void AGSignallingManager::SetNetworkStatus(uint32_t bOut)
{
	if (m_lpAgoraInst)
	{
		m_lpAgoraInst->setNetworkStatus(bOut);
	}
}

void AGSignallingManager::Ping()
{
	if (m_lpAgoraInst)
	{
		m_lpAgoraInst->ping();
	}
}

void AGSignallingManager::SetAttr(std::string s_name, std::string s_value)
{
	if (m_lpAgoraInst)
	{
		m_lpAgoraInst->setAttr(s_name.c_str(), s_name.size(), s_value.c_str(), s_value.size());
	}
}

void AGSignallingManager::GetAttr(std::string s_name)
{
	if (m_lpAgoraInst)
	{
		m_lpAgoraInst->getAttr(s_name.c_str(), s_name.size());
	}
}

void AGSignallingManager::GetAttrAll()
{
	if (m_lpAgoraInst)
	{
		m_lpAgoraInst->getAttrAll();
	}
}

void AGSignallingManager::GetUserAttr(std::string s_account, std::string s_name)
{
	if (m_lpAgoraInst)
	{
		m_lpAgoraInst->getUserAttr(s_account.c_str(), s_account.size(), s_name.c_str(), s_name.size());
	}
}

void AGSignallingManager::GetUserAttrAll(std::string s_account)
{
	if (m_lpAgoraInst)
	{
		m_lpAgoraInst->getUserAttrAll(s_account.c_str(), s_account.size());
	}
}

void AGSignallingManager::QueryUserStatus(std::string s_account)
{
	if (m_lpAgoraInst)
	{
		m_lpAgoraInst->queryUserStatus(s_account.c_str(), s_account.size());
	}
}

void AGSignallingManager::Invoke(std::string s_name, std::string s_req, std::string call_id)
{
	if (m_lpAgoraInst)
	{
		m_lpAgoraInst->invoke(s_name.c_str(), s_name.size(), s_req.c_str(), s_req.size(), call_id.c_str(), call_id.size());
	}
}

void AGSignallingManager::Start()
{
	if (m_lpAgoraInst)
	{
		m_lpAgoraInst->start();
	}
}

void AGSignallingManager::Stop()
{
	if (m_lpAgoraInst)
	{
		m_lpAgoraInst->stop();
	}
}

bool AGSignallingManager::IsOnline()
{
	if (m_lpAgoraInst)
	{
		return m_lpAgoraInst->isOnline();
	}
	return false;
}

int AGSignallingManager::GetStatus()
{
	if (m_lpAgoraInst)
	{
		return m_lpAgoraInst->getStatus();
	}
	return 0;
}

int AGSignallingManager::GetSdkVersion()
{
	if (m_lpAgoraInst)
	{
		return m_lpAgoraInst->getSdkVersion();
	}
	return 0;
}

void AGSignallingManager::Dbg(std::string s_value1, std::string s_value2)
{
	if (m_lpAgoraInst)
	{
		m_lpAgoraInst->dbg(s_value1.c_str(), s_value1.size(), s_value2.c_str(), s_value2.size());
	}
}

AGSignallingManager * AGSignallingManager::GetAGSignalling(std::string s_vendor)
{
	if (!m_lpAgoraSignalling)
	{
		m_lpAgoraSignalling = new AGSignallingManager();
	}

	if (s_vendor.empty())
	{
		return m_lpAgoraSignalling;
	}

	if (!m_lpAgoraInst)
	{
		m_lpAgoraInst = getAgoraSDKInstanceWin(s_vendor.data(), s_vendor.size());

		m_lpAgoraInst->callbackSet(new AGSignallingCallBack());
		if (m_lpSignallingEventHandler)
		{
			delete m_lpSignallingEventHandler;
			m_lpSignallingEventHandler = NULL;
		}

		m_lpSignallingEventHandler = m_lpAgoraInst->callbackGet();
	}

	return m_lpAgoraSignalling;
}

void AGSignallingManager::CloseAGSignalling()
{
	if (m_lpSignallingEventHandler)
	{
		delete m_lpSignallingEventHandler;
		m_lpSignallingEventHandler = NULL;
	}

	if (m_lpAgoraSignalling)
	{
		delete m_lpAgoraSignalling;
		m_lpAgoraSignalling = NULL;
	}
}

