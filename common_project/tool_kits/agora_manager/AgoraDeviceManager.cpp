#include "AgoraDeviceManager.h"
#include "base/macros.h"

AgoraDeviceManager::AgoraDeviceManager()
{
	p_agoraObject_ = NULL;
	p_rtcEngine_ = NULL;
	device_param_ = NULL;
	s_app_id_ = "";
	channel_type_ = AgoraVChatChannel_NONE;
}

AgoraDeviceManager::~AgoraDeviceManager()
{

}

void AgoraDeviceManager::SetAppID(std::string app_id)
{
	s_app_id_ = app_id;
}

DeviceManagerParam* AgoraDeviceManager::CreateDeviceManager()
{
	if (channel_type_ == AgoraVChatChannel_NONE)
	{
		return NULL;
	}

	FreeDeviceManager();
	p_rtcEngine_ = p_agoraObject_->GetEngine();
	p_agoraObject_->EnableLocalVideo(true);

	AgoraCameraManager*	m_agCamera = new AgoraCameraManager();
	AgoraPlayoutManager* m_agPlayout = new AgoraPlayoutManager();
	AgoraAudInputManager* m_agAudioin = new AgoraAudInputManager();

	m_agCamera->Create(p_rtcEngine_);
	m_agPlayout->Create(p_rtcEngine_);
	m_agAudioin->Create(p_rtcEngine_);

	device_param_ = new DeviceManagerParam;
	device_param_->m_agCamera_ = m_agCamera;
	device_param_->m_agAudioin_ = m_agAudioin;
	device_param_->m_agPlayout_ = m_agPlayout;

	return device_param_;
}

AGEngineManager* AgoraDeviceManager::GetEngineManager()
{
	return p_agoraObject_;
}

IRtcEngine* AgoraDeviceManager::GetRtcEngine()
{
	return p_rtcEngine_;
}

void AgoraDeviceManager::FreeDeviceManager()
{
	if (!device_param_)
	{
		return;
	}

	if (device_param_->m_agCamera_)
	{
		device_param_->m_agCamera_->TestCameraDevice(NULL, FALSE);
		device_param_->m_agCamera_->Close();
		SAFE_DELETE(device_param_->m_agCamera_);
	}
	if (device_param_->m_agAudioin_)
	{
		device_param_->m_agAudioin_->Close();
		SAFE_DELETE(device_param_->m_agAudioin_);
	}
	if (device_param_->m_agPlayout_)
	{
		device_param_->m_agPlayout_->Close();
		SAFE_DELETE(device_param_->m_agPlayout_);
	}

	SAFE_DELETE(device_param_);
}

DeviceManagerParam* AgoraDeviceManager::GetDeviceManager()
{
	return device_param_;
}

void AgoraDeviceManager::SetChannelProfile(AgoraVChatChannelProfile type)
{
	if (channel_type_ != type)
	{
		channel_type_ = type;
		FreeDeviceManager();
		AGEngineManager::CloseAGEngine();
		p_agoraObject_ = NULL;
		p_rtcEngine_ = NULL;

		p_agoraObject_ = AGEngineManager::GetAGEngine(s_app_id_);
		p_rtcEngine_ = p_agoraObject_->GetEngine();

		if (p_agoraObject_)
		{
			p_agoraObject_->EnableVideo(TRUE);
		}
		if (p_rtcEngine_)
		{
			if (type == AgoraVChatChannel_COMMUNICATION)
			{
				p_rtcEngine_->setChannelProfile(CHANNEL_PROFILE_COMMUNICATION);
			}else if (type == AgoraVChatChannel_LIVE)
			{
				p_rtcEngine_->setChannelProfile(CHANNEL_PROFILE_LIVE_BROADCASTING);
			}
		}
	}
}

AgoraVChatChannelProfile AgoraDeviceManager::GetChannelProfile()
{
	return channel_type_;
}

int AgoraDeviceManager::setClientRole(CLIENT_ROLE_TYPE role, const char* permissionKey)
{
	if (p_rtcEngine_)
	{
		return p_rtcEngine_->setClientRole(role, permissionKey);
	}

	return -1;
}

void AgoraDeviceManager::CloseAGEngine()
{
	AGEngineManager::CloseAGEngine();
}