#include "AgoraDeviceManager.h"
#include "base/macros.h"

AgoraDeviceManager::AgoraDeviceManager()
{

}

AgoraDeviceManager::~AgoraDeviceManager()
{

}

void AgoraDeviceManager::OnInit(std::string s_vendorKey /*= ""*/)
{
	p_agoraObject_ = AGEngineManager::GetAGEngine(s_vendorKey);
	if (p_agoraObject_)
	{
		p_agoraObject_->EnableVideo(TRUE);
	}
}

void AgoraDeviceManager::OnCreate(DeviceManagerParam &param)
{
	FreeDeviceManager(param.platsource_);
	p_rtcEngine_ = p_agoraObject_->GetEngine();
	p_agoraObject_->EnableLocalVideo(true);
	p_agoraObject_->EnableVideo(TRUE);

	AgoraCameraManager*	m_agCamera = new AgoraCameraManager();
	AgoraPlayoutManager* m_agPlayout = new AgoraPlayoutManager();
	AgoraAudInputManager* m_agAudioin = new AgoraAudInputManager();

	m_agCamera->Create(p_rtcEngine_);
	m_agPlayout->Create(p_rtcEngine_);
	m_agAudioin->Create(p_rtcEngine_);

	param.m_agCamera_ = m_agCamera;
	param.m_agAudioin_ = m_agAudioin;
	param.m_agPlayout_ = m_agPlayout;

	DeviceManagerParam* d_param = new DeviceManagerParam;
	d_param->platsource_ = param.platsource_;
	d_param->m_agAudioin_ = m_agAudioin;
	d_param->m_agPlayout_ = m_agPlayout;
	d_param->m_agCamera_ = m_agCamera; 

	device_manager_map_.insert(DeviceManagerParamMap::value_type(d_param->platsource_, d_param));
}

AGEngineManager* AgoraDeviceManager::GetEngineManager()
{
	return p_agoraObject_;
}

IRtcEngine* AgoraDeviceManager::GetRtcEngine()
{
	return p_rtcEngine_;
}

void AgoraDeviceManager::OnClose(DeviceManagerParam *param)
{
	AgoraCameraManager*	m_agCamera = param->m_agCamera_;
	AgoraPlayoutManager* m_agPlayout = param->m_agPlayout_;
	AgoraAudInputManager* m_agAudioin = param->m_agAudioin_;
	if (m_agCamera)
	{
		m_agCamera->TestCameraDevice(NULL, FALSE);
		m_agCamera->Close();
		SAFE_DELETE(m_agCamera);
	}
	if (m_agAudioin)
	{
		m_agAudioin->Close();
		SAFE_DELETE(m_agAudioin);
	}
	if (m_agPlayout)
	{
		m_agPlayout->Close();
		SAFE_DELETE(m_agPlayout);
	}
}

void AgoraDeviceManager::FreeDeviceManager(int type)
{
	auto it = device_manager_map_.find(type);
	if (it != device_manager_map_.end())
	{
		OnClose(it->second);
		SAFE_DELETE(it->second);
		device_manager_map_.erase(it);
	}
}

void AgoraDeviceManager::GetDeviceManagerParam(DeviceManagerParam &param)
{
	auto it = device_manager_map_.find(param.platsource_);
	if (it != device_manager_map_.end())
	{
		DeviceManagerParam* d_param = it->second;
		if (d_param)
		{
			param.m_agAudioin_ = d_param->m_agAudioin_;
			param.m_agCamera_ = d_param->m_agCamera_;
			param.m_agPlayout_ = d_param->m_agPlayout_;
		}
	}
}


