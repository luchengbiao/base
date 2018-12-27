#pragma once
#include "base\callback\callback.h"
#include "base\memory\singleton.h"
#include "agora_device\playout_manager.h"
#include "agora_device\audio_input_manager.h"
#include "agora_device\camera_manager.h"
#include "agora_engine_sdk\engine_manager.h"
#include "agora\IAgoraRtcEngine.h"

struct DeviceManagerParam
{
	AgoraCameraManager* m_agCamera_;
	AgoraPlayoutManager* m_agPlayout_;
	AgoraAudInputManager* m_agAudioin_;

	DeviceManagerParam()
	{
		m_agCamera_ = nullptr;
		m_agPlayout_ = nullptr;
		m_agAudioin_ = nullptr;
	}
};

enum AgoraVChatChannelProfile
{
	AgoraVChatChannel_NONE = 0,
	AgoraVChatChannel_COMMUNICATION = 1,
	AgoraVChatChannel_LIVE = 2,
};

typedef std::map<int, DeviceManagerParam*> DeviceManagerParamMap;

class AgoraDeviceManager : public nbase::SupportWeakCallback
{
public:
	SINGLETON_DEFINE(AgoraDeviceManager);

	AgoraDeviceManager();
	~AgoraDeviceManager();
	void SetAppID(std::string app_id);
	void SetChannelProfile(AgoraVChatChannelProfile type);
	AgoraVChatChannelProfile GetChannelProfile();
	DeviceManagerParam* CreateDeviceManager();
	AGEngineManager* GetEngineManager();
	IRtcEngine* GetRtcEngine();
	void FreeDeviceManager();
	DeviceManagerParam* GetDeviceManager();
	int setClientRole(CLIENT_ROLE_TYPE role, const char* permissionKey);
	void CloseAGEngine();
private:
	std::string s_app_id_;
	AgoraVChatChannelProfile channel_type_;
	AGEngineManager* p_agoraObject_;
	IRtcEngine *p_rtcEngine_;
	DeviceManagerParam* device_param_;
};