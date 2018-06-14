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
	int platsource_;		//来源 默认 一对一 班课
	AgoraCameraManager* m_agCamera_;
	AgoraPlayoutManager* m_agPlayout_;
	AgoraAudInputManager* m_agAudioin_;

	DeviceManagerParam()
	{
		platsource_ = 0;
		m_agCamera_ = nullptr;
		m_agPlayout_ = nullptr;
		m_agAudioin_ = nullptr;
	}
};

typedef std::map<int, DeviceManagerParam*> DeviceManagerParamMap;

class AgoraDeviceManager : public nbase::SupportWeakCallback
{
public:
	SINGLETON_DEFINE(AgoraDeviceManager);

	AgoraDeviceManager();
	~AgoraDeviceManager();
	void OnInit(std::string s_vendorKey = "");
	void OnUninit();
	void OnCreate(DeviceManagerParam &param);
	AGEngineManager* GetEngineManager();
	IRtcEngine* GetRtcEngine();
	void FreeDeviceManager(int type);
	void GetDeviceManagerParam(DeviceManagerParam &param);

private:
	void OnClose(DeviceManagerParam *param);
private:
	AGEngineManager* p_agoraObject_;
	IRtcEngine *p_rtcEngine_;
	DeviceManagerParamMap device_manager_map_;
};