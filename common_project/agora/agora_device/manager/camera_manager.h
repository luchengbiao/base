#pragma once
#include <wtypes.h>
#include "agora\IAgoraRtcEngine.h"
#include <string>

class AgoraCameraManager
{
public:
	AgoraCameraManager();
	~AgoraCameraManager();

	BOOL Create(agora::rtc::IRtcEngine *lpRtcEngine);
	void Close();

	UINT ReloadDevice();
	UINT GetDeviceCount();

	std::string GetCurDeviceID();
	BOOL SetCurDevice(LPCTSTR lpDeviceID);

	BOOL GetDevice(UINT nIndex, std::string &rDeviceName, std::string &rDeviceID);
	void TestCameraDevice(HWND hVideoWnd, BOOL bTestOn = TRUE, BOOL bDualStreamMode = FALSE);
	void TestRemoteCameraDevice(HWND hVideoWnd, int uid, BOOL bTestOn = TRUE);

	BOOL IsTesting();

private:
	agora::rtc::AVideoDeviceManager	*m_ptrDeviceManager;
	agora::rtc::IVideoDeviceCollection	*m_lpCollection;
	BOOL m_bTestingOn;
};

