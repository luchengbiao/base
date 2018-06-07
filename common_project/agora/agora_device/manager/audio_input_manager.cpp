#include "audio_input_manager.h"
#include <assert.h>
#include "..\agora_engine_sdk\manager\engine_manager.h"

AgoraAudInputManager::AgoraAudInputManager()
: m_ptrDeviceManager(NULL)
, m_lpCollection(NULL)
, m_bTestingOn(FALSE)
{
}


AgoraAudInputManager::~AgoraAudInputManager()
{
	Close();
}

BOOL AgoraAudInputManager::Create(agora::rtc::IRtcEngine *lpRtcEngine)
{
	m_ptrDeviceManager = new agora::rtc::AAudioDeviceManager(lpRtcEngine);
	if (m_ptrDeviceManager->get() == NULL) {
		return FALSE;
	}

	ReloadDevice();

	return m_lpCollection != NULL ? TRUE : FALSE;
}

void AgoraAudInputManager::Close()
{
	if (m_lpCollection != NULL) {
		m_lpCollection->release();
		m_lpCollection = NULL;
	}

	if (m_ptrDeviceManager != NULL) {
		m_ptrDeviceManager->release();
		m_ptrDeviceManager = NULL;
	}
}

UINT AgoraAudInputManager::GetVolume()
{
	int nVol = 0;

	if (m_ptrDeviceManager != NULL && *m_ptrDeviceManager != NULL) {
		(*m_ptrDeviceManager)->getRecordingDeviceVolume(&nVol);
	}

	return (UINT)nVol;
}

BOOL AgoraAudInputManager::SetVolume(UINT nVol)
{
	int nRet = -1;

	if (m_ptrDeviceManager == NULL || *m_ptrDeviceManager == NULL)
	{
		return nRet == 0 ? TRUE : FALSE;
	}

	if (m_ptrDeviceManager != NULL && *m_ptrDeviceManager != NULL) {
		nRet = (*m_ptrDeviceManager)->setRecordingDeviceVolume((int)nVol);
	}

	return nRet == 0 ? TRUE : FALSE;
}

UINT AgoraAudInputManager::GetDeviceCount()
{
	if (m_lpCollection != NULL) {
		return (UINT)m_lpCollection->getCount();
	}

	return 0;
}

BOOL AgoraAudInputManager::GetDevice(UINT nIndex, std::string &rDeviceName, std::string &rDeviceID)
{
	char szDeviceName[agora::rtc::MAX_DEVICE_ID_LENGTH] = { 0 };
	char szDeviceID[agora::rtc::MAX_DEVICE_ID_LENGTH] = { 0 };

	assert(nIndex < GetDeviceCount());

	if (nIndex >= GetDeviceCount()) {
		return FALSE;
	}

	if (m_lpCollection == NULL) {
		return FALSE;
	}

	int nRet = m_lpCollection->getDevice(nIndex, szDeviceName, szDeviceID);
	if (nRet != 0) {
		return FALSE;
	}

	rDeviceName = std::string(szDeviceName);
	rDeviceID = std::string(szDeviceID);

	return TRUE;
}

std::string AgoraAudInputManager::GetCurDeviceID()
{
	std::string	str;
	char szDeviceID[agora::rtc::MAX_DEVICE_ID_LENGTH] = { 0 };

	if (m_ptrDeviceManager == NULL || *m_ptrDeviceManager == NULL)
	{
		return str;
	}

	if (m_ptrDeviceManager != NULL && *m_ptrDeviceManager != NULL) {
		(*m_ptrDeviceManager)->getRecordingDevice(szDeviceID);
	}

	str = std::string(szDeviceID);

	return str;
}

BOOL AgoraAudInputManager::SetCurDevice(LPCTSTR lpDeviceID)
{
	if (m_ptrDeviceManager == NULL || *m_ptrDeviceManager == NULL) {
		return FALSE;
	}

#ifdef UNICODE
	CHAR szDeviceID[128] = { 0 };
	::WideCharToMultiByte(CP_ACP, 0, lpDeviceID, -1, szDeviceID, 128, NULL, NULL);
	int nRet = (*m_ptrDeviceManager)->setRecordingDevice(szDeviceID);
#else
	int nRet = (*m_ptrDeviceManager)->setRecordingDevice(lpDeviceID);
#endif

	return nRet == 0 ? TRUE : FALSE;
}

void AgoraAudInputManager::TestAudInputDevice(BOOL bTestOn)
{
	if (m_ptrDeviceManager == NULL || *m_ptrDeviceManager == NULL) {
		return;
	}

	if (bTestOn && !m_bTestingOn) {
		IRtcEngine *lpRtcEngine = AGEngineManager::GetEngine();
		RtcEngineParameters rep(*lpRtcEngine);
		rep.enableAudioVolumeIndication(300, 3);
		(*m_ptrDeviceManager)->startRecordingDeviceTest(300);
	}
	else if (!bTestOn && m_bTestingOn){
		(*m_ptrDeviceManager)->stopRecordingDeviceTest();
	}

	m_bTestingOn = bTestOn;

}

BOOL AgoraAudInputManager::IsTesting()
{
	return m_bTestingOn;
}

UINT AgoraAudInputManager::ReloadDevice()
{
	if (m_lpCollection != NULL) {
		m_lpCollection->release();
		m_lpCollection = NULL;
	}

	if (m_ptrDeviceManager != NULL && *m_ptrDeviceManager != NULL)
	{
		m_lpCollection = (*m_ptrDeviceManager)->enumerateRecordingDevices();
		return GetDeviceCount();
	}
	return 0;
}