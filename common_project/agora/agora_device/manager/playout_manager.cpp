#include "playout_manager.h"
#include <assert.h>
#include <tchar.h>
#include "..\agora_engine_sdk\manager\agora_resource_visitor.h"

AgoraPlayoutManager::AgoraPlayoutManager()
: m_ptrDeviceManager(NULL)
, m_lpCollection(NULL)
, m_bTestingOn(FALSE)
{
}


AgoraPlayoutManager::~AgoraPlayoutManager()
{
	Close();
}

BOOL AgoraPlayoutManager::Create(agora::rtc::IRtcEngine *lpRtcEngine)
{
	m_ptrDeviceManager = new agora::rtc::AAudioDeviceManager(lpRtcEngine);
	if (m_ptrDeviceManager->get() == NULL) {
		return FALSE;
	}

	ReloadDevice();

	return m_lpCollection != NULL ? TRUE : FALSE;
}

void AgoraPlayoutManager::Close()
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

UINT AgoraPlayoutManager::ReloadDevice()
{
	if (m_lpCollection != NULL){
		m_lpCollection->release();
		m_lpCollection = NULL;
	}

	if (m_ptrDeviceManager != NULL && *m_ptrDeviceManager != NULL) {
		m_lpCollection = (*m_ptrDeviceManager)->enumeratePlaybackDevices();
	}

	UINT count = GetDeviceCount();
	return count;
}

UINT AgoraPlayoutManager::GetVolume()
{
	int nVol = 0;
	if (m_ptrDeviceManager != NULL && *m_ptrDeviceManager != NULL) {
		(*m_ptrDeviceManager)->getPlaybackDeviceVolume(&nVol);
	}

	return (UINT)nVol;
}

BOOL AgoraPlayoutManager::SetVolume(UINT nVol)
{
	int nRet = -1;

	if (m_ptrDeviceManager != NULL && *m_ptrDeviceManager != NULL) {
		nRet = (*m_ptrDeviceManager)->setPlaybackDeviceVolume((int)nVol);
	}

	return nRet == 0 ? TRUE : FALSE;
}

UINT AgoraPlayoutManager::GetDeviceCount()
{
	if (m_lpCollection)
	{
		return (UINT)m_lpCollection->getCount();
	}
	return 0;
}

BOOL AgoraPlayoutManager::GetDevice(UINT nIndex, std::string &rDeviceName, std::string &rDeviceID)
{
	CHAR szDeviceName[agora::rtc::MAX_DEVICE_ID_LENGTH] = { 0 };
	CHAR szDeviceID[agora::rtc::MAX_DEVICE_ID_LENGTH] = { 0 };

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

std::string AgoraPlayoutManager::GetCurDeviceID()
{
	CHAR szDeviceID[agora::rtc::MAX_DEVICE_ID_LENGTH] = { 0 };
	if (m_ptrDeviceManager == NULL || *m_ptrDeviceManager == NULL)
	{
		return "";
	}

	if (m_ptrDeviceManager != NULL && *m_ptrDeviceManager != NULL) {
		(*m_ptrDeviceManager)->getPlaybackDevice(szDeviceID);
	}

	return std::string(szDeviceID);
}

BOOL AgoraPlayoutManager::SetCurDevice(LPCTSTR lpDeviceID)
{
	if (m_ptrDeviceManager == NULL || *m_ptrDeviceManager == NULL) {
		return FALSE;
	}

#ifdef UNICODE
	CHAR szDeviceID[128];
	::WideCharToMultiByte(CP_ACP, 0, lpDeviceID, -1, szDeviceID, 128, NULL, NULL);
	int nRet = (*m_ptrDeviceManager)->setPlaybackDevice(szDeviceID);
#else
	int nRet = (*m_ptrDeviceManager)->setPlaybackDevice(lpDeviceID);
#endif

	return nRet == 0 ? TRUE : FALSE;
}

void AgoraPlayoutManager::TestPlaybackDevice(UINT nWavID, BOOL bTestOn)
{
	if (m_ptrDeviceManager == NULL || *m_ptrDeviceManager == NULL) {
		return;
	}

	TCHAR	szWavPath[MAX_PATH];

	::GetModuleFileName(NULL, szWavPath, MAX_PATH);
	LPTSTR lpLastSlash = (LPTSTR)_tcsrchr(szWavPath, _T('\\')) + 1;
	_tcscpy_s(lpLastSlash, 27, _T("\\res\\audio\\soundTest.wav"));

	if (bTestOn && !m_bTestingOn) {
		AGResourceVisitor::SaveResourceToFile(_T("WAVE"), nWavID, szWavPath);

#ifdef UNICODE
		CHAR szWavPathA[MAX_PATH];

		::WideCharToMultiByte(CP_ACP, 0, szWavPath, -1, szWavPathA, MAX_PATH, NULL, NULL);
		(*m_ptrDeviceManager)->startPlaybackDeviceTest(szWavPathA);
#else
		(*m_ptrDeviceManager)->startPlaybackDeviceTest(szWavPathA);
#endif
	}
	else if (!bTestOn && m_bTestingOn) {
		(*m_ptrDeviceManager)->stopPlaybackDeviceTest();
	}

	m_bTestingOn = bTestOn;
}

BOOL AgoraPlayoutManager::IsTesting()
{
	return m_bTestingOn;
}
