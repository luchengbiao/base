#include "camera_manager.h"
#include <assert.h>
#include "..\agora_engine_sdk\manager\engine_manager.h"

AgoraCameraManager::AgoraCameraManager()
	: m_ptrDeviceManager(NULL)
	, m_lpCollection(NULL)
	, m_bTestingOn(FALSE)
{
}


AgoraCameraManager::~AgoraCameraManager()
{
	Close();
}

BOOL AgoraCameraManager::Create(agora::rtc::IRtcEngine *lpRtcEngine)
{
	m_ptrDeviceManager = new agora::rtc::AVideoDeviceManager(lpRtcEngine);
	if (m_ptrDeviceManager->get() == NULL) {
		return FALSE;
	}

	ReloadDevice();

	return m_lpCollection != NULL ? TRUE : FALSE;
}

void AgoraCameraManager::Close()
{
	if (m_lpCollection != NULL){
		m_lpCollection->release();
		m_lpCollection = NULL;
	}

	if (m_ptrDeviceManager != NULL) {
		m_ptrDeviceManager->release();
		m_ptrDeviceManager = NULL;
	}
}

UINT AgoraCameraManager::GetDeviceCount()
{
	if (m_lpCollection == NULL) {
		return 0;
	}

	return (UINT)m_lpCollection->getCount();
}

std::string AgoraCameraManager::GetCurDeviceID()
{
	CHAR szDeviceID[agora::rtc::MAX_DEVICE_ID_LENGTH] = {0};

	if (m_ptrDeviceManager != NULL && *m_ptrDeviceManager != NULL) {
		(*m_ptrDeviceManager)->getDevice(szDeviceID);
	}

	return std::string(szDeviceID);
}

BOOL AgoraCameraManager::GetDevice(UINT nIndex, std::string &rDeviceName, std::string &rDeviceID)
{
	CHAR szDeviceName[agora::rtc::MAX_DEVICE_ID_LENGTH] = {0};
	CHAR szDeviceID[agora::rtc::MAX_DEVICE_ID_LENGTH] = {0};

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

BOOL AgoraCameraManager::SetCurDevice(LPCTSTR lpDeviceID)
{
	if (m_ptrDeviceManager == NULL || *m_ptrDeviceManager == NULL) {
		return FALSE;
	}

#ifdef UNICODE
	CHAR szDeviceID[128];
	::WideCharToMultiByte(CP_ACP, 0, lpDeviceID, -1, szDeviceID, 128, NULL, NULL);
	int nRet = (*m_ptrDeviceManager)->setDevice(szDeviceID);
#else
	int nRet = (*m_ptrDeviceManager)->setDevice(lpDeviceID);
#endif

	return nRet == 0 ? TRUE : FALSE;
}

void AgoraCameraManager::TestCameraDevice(HWND hVideoWnd, BOOL bTestOn, BOOL bDualStreamMode)
{
	if (bTestOn) {
		AGEngineManager::GetAGEngine()->LocalVideoPreview(hVideoWnd, TRUE, bDualStreamMode);
	}
	else{
		AGEngineManager::GetAGEngine()->LocalVideoPreview(NULL, FALSE, bDualStreamMode);
	}
}

void AgoraCameraManager::TestRemoteCameraDevice(HWND hVideoWnd, int uid, BOOL bTestOn)
{
	if (bTestOn)
	{
		assert(hVideoWnd != NULL);
		AGEngineManager::GetAGEngine()->RemoteVideoPreview(hVideoWnd, uid);
	}
	else
	{
		AGEngineManager::GetAGEngine()->RemoteVideoPreview(NULL);
	}
	
}

BOOL AgoraCameraManager::IsTesting()
{
	return m_bTestingOn;
}

UINT AgoraCameraManager::ReloadDevice()
{
	if (m_lpCollection != NULL){
		m_lpCollection->release();
		m_lpCollection = NULL;
	}
	if (m_ptrDeviceManager != NULL && *m_ptrDeviceManager != NULL) {
		m_lpCollection = (*m_ptrDeviceManager)->enumerateVideoDevices();
	}

	UINT count = GetDeviceCount();
	return count;
}