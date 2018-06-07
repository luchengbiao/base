#pragma once
#include <wtypes.h>
#include "agora\IAgoraRtcEngine.h"
#include <string>
class AgoraPlayoutManager
{
public:
	AgoraPlayoutManager();
	~AgoraPlayoutManager();

	BOOL Create(agora::rtc::IRtcEngine *lpRtcEngine);
	void Close();

	UINT ReloadDevice();
	UINT GetVolume();
	BOOL SetVolume(UINT nVol);
	UINT GetDeviceCount();

	std::string GetCurDeviceID();
	BOOL SetCurDevice(LPCTSTR lpDeviceID);

	BOOL GetDevice(UINT nIndex, std::string &rDeviceName, std::string &rDeviceID);
	void TestPlaybackDevice(UINT nWavID, BOOL bTestOn = TRUE);

	BOOL IsTesting();

private:
	agora::rtc::AAudioDeviceManager	*m_ptrDeviceManager;
	agora::rtc::IAudioDeviceCollection	*m_lpCollection;
	BOOL m_bTestingOn;
};

