#pragma once
#include <wtypes.h>
#include "agora\IAgoraRtcEngine.h"
#include <string>
#include <map>
#include <memory>
#include <functional>
#include "base\util\auto_unregister.h"
#include "base\callback\callback.h"



class AgoraAudInputManager
{
public:
	AgoraAudInputManager();
	~AgoraAudInputManager();

	BOOL Create(agora::rtc::IRtcEngine *lpRtcEngine);
	void Close();
	UINT ReloadDevice();

	UINT GetVolume();
	BOOL SetVolume(UINT nVol);
	UINT GetDeviceCount();

	std::string GetCurDeviceID();
	BOOL SetCurDevice(LPCTSTR lpDeviceID);
	BOOL GetDevice(UINT nIndex, std::string &rDeviceName, std::string &rDeviceID);

	void TestAudInputDevice(BOOL bTestOn);
	BOOL IsTesting();
private:
	BOOL						m_bTestingOn;
	HWND						m_hOldMsgWnd;
	agora::rtc::AAudioDeviceManager	*m_ptrDeviceManager;
	agora::rtc::IAudioDeviceCollection	*m_lpCollection;
	
};

