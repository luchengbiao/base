#include "engine_manager.h"
#include "callback\engine_callback.h"
#include <assert.h>
#include <tchar.h>
#include "agora_resource_visitor.h"


AGEngineManager *AGEngineManager::m_lpAgoraObject = NULL;
IRtcEngine *AGEngineManager::m_lpAgoraEngine = NULL;
IRtcEngineEventHandler* AGEngineManager::m_lpEngineEventHandler = NULL;
std::string AGEngineManager::m_strVendorKey = "";

AGEngineManager::AGEngineManager(void)
	: m_dwEngineFlag(0)
	, m_bVideoEnable(FALSE)
	, m_bLocalAudioMuted(FALSE)
	, m_bScreenCapture(FALSE)
	, m_nSelfUID(0)
{
	m_strChannelName = L"";
	m_bLocalVideoMuted = FALSE;
}

AGEngineManager::~AGEngineManager(void)
{
}

std::string AGEngineManager::GetSDKVersion()
{
	int nBuildNumber = 0;
	const char *lpszEngineVer = getAgoraRtcEngineVersion(&nBuildNumber);

	return std::string(lpszEngineVer);
}

int AGEngineManager::StartRecordingService(std::string s_channel_key)
{
	int ret = 0;
	if (m_lpAgoraEngine) {
		RtcEngineParameters rep(*m_lpAgoraEngine);

		ret = rep.startRecordingService(s_channel_key.c_str());
	}
	return ret;
}

int AGEngineManager::StopRecordingService(std::string s_channel_key)
{
	int ret = 0;
	if (m_lpAgoraEngine) {
		RtcEngineParameters rep(*m_lpAgoraEngine);

		ret = rep.stopRecordingService(s_channel_key.c_str());
	}
	return ret;
}

void AGEngineManager::SetChannelProfile(CHANNEL_PROFILE_TYPE profile)
{
	if (m_lpAgoraEngine) {
		m_lpAgoraEngine->setChannelProfile(profile);
	}
}

void AGEngineManager::SetClientRole(CLIENT_ROLE_TYPE role, std::string permissionKey)
{
	if (m_lpAgoraEngine) {
		m_lpAgoraEngine->setClientRole(role, permissionKey.c_str());
	}
}

void AGEngineManager::EnableDualStreamMode(bool enabled)
{
	if (m_lpAgoraEngine) {
		RtcEngineParameters rep(*m_lpAgoraEngine);
		rep.enableDualStreamMode(enabled);
	}
}

IRtcEngine *AGEngineManager::GetEngine()
{
	if (m_lpAgoraEngine == NULL) {
		m_lpAgoraEngine = (IRtcEngine *)createAgoraRtcEngine();
	}

	return m_lpAgoraEngine;
}

AGEngineManager *AGEngineManager::GetAGEngine(std::string s_vendorKey)
{
	if (m_lpAgoraObject == NULL) {
		m_lpAgoraObject = new AGEngineManager();
	}

	if (m_lpAgoraEngine == NULL) {
		m_lpAgoraEngine = (IRtcEngine *)createAgoraRtcEngine();
	}

	// 如果VendorKey为空则直接返回对象
	if (s_vendorKey.empty()) {
		return m_lpAgoraObject;
	}

	if (m_lpEngineEventHandler == NULL) {
		m_lpEngineEventHandler = new AGEngineEventHandler();
	}

	RtcEngineContext ctx;
	ctx.eventHandler = m_lpEngineEventHandler;
	ctx.appId = s_vendorKey.c_str();
	int res = m_lpAgoraEngine->initialize(ctx);

	if (!s_vendorKey.empty()) {
		m_strVendorKey = s_vendorKey;
	}

	return m_lpAgoraObject;
}

void AGEngineManager::CloseAGEngine()
{
	if (m_lpAgoraEngine != NULL) {
		m_lpAgoraEngine->release();
	}

	if (m_lpAgoraObject != NULL) {
		delete m_lpAgoraObject;
	}

	if (m_lpEngineEventHandler != NULL){
		delete m_lpEngineEventHandler;
	}

	m_lpEngineEventHandler = NULL;
	m_lpAgoraEngine = NULL;
	m_lpAgoraObject = NULL;
}

void AGEngineManager::SetNetworkTestFlag(BOOL bEnable)
{
	if (bEnable){
		m_dwEngineFlag |= AG_ENGFLAG_ENNETTEST;
	}
	else {
		m_dwEngineFlag &= (~AG_ENGFLAG_ENNETTEST);
	}
}

BOOL AGEngineManager::GetNetworkTestFlag()
{
	return (m_dwEngineFlag & AG_ENGFLAG_ENNETTEST) != 0;
}

void AGEngineManager::SetEchoTestFlag(BOOL bEnable)
{
	if (bEnable) {
		m_dwEngineFlag |= AG_ENGFLAG_ECHOTEST;
	}
	else {
		m_dwEngineFlag &= (~AG_ENGFLAG_ECHOTEST);
	}
}

BOOL AGEngineManager::GetEchoTestFlag()
{
	return (m_dwEngineFlag & AG_ENGFLAG_ECHOTEST) != 0;
}

void AGEngineManager::SetSpeakerphoneTestFlag(BOOL bEnable)
{
	if (bEnable) {
		m_dwEngineFlag |= AG_ENGFLAG_SPKPHTEST;
	}
	else {
		m_dwEngineFlag &= (~AG_ENGFLAG_SPKPHTEST);
	}
}

BOOL AGEngineManager::GetSpeakerphoneTestFlag()
{
	return (m_dwEngineFlag & AG_ENGFLAG_SPKPHTEST) != 0;
}

void AGEngineManager::SetMicrophoneTestFlag(BOOL bEnable)
{
	if (bEnable) {
		m_dwEngineFlag |= AG_ENGFLAG_MICPHTEST;
	}
	else {
		m_dwEngineFlag &= (~AG_ENGFLAG_MICPHTEST);
	}
}

BOOL AGEngineManager::GetMicrophoneTestFlag()
{
	return (m_dwEngineFlag & AG_ENGFLAG_MICPHTEST) != 0;
}


void AGEngineManager::SetVideoTestFlag(BOOL bEnable)
{
	if (bEnable) {
		m_dwEngineFlag |= AG_ENGFLAG_VIDEOTEST;
	}
	else {
		m_dwEngineFlag &= (~AG_ENGFLAG_VIDEOTEST);
	}
}

BOOL AGEngineManager::GetVideoTestFlag()
{
	return (m_dwEngineFlag & AG_ENGFLAG_VIDEOTEST) != 0;
}

BOOL AGEngineManager::SetLogFilePath(LPCTSTR lpLogPath)
{
	assert(m_lpAgoraEngine != NULL);

	CHAR szLogPathA[MAX_PATH];
	CHAR szLogPathTrans[MAX_PATH];

	int ret = 0;
	RtcEngineParameters rep(*m_lpAgoraEngine);

	if (::GetFileAttributes(lpLogPath) == INVALID_FILE_ATTRIBUTES) {
		::GetModuleFileNameA(NULL, szLogPathA, MAX_PATH);
		LPSTR lpLastSlash = strrchr(szLogPathA, '\\')+1;
		strcpy_s(lpLastSlash, 256, "AgoraSDK.log");
	}
	else {
#ifdef UNICODE
		::WideCharToMultiByte(CP_UTF8, 0, lpLogPath, -1, szLogPathA, MAX_PATH, NULL, NULL);
#else
		::MultiByteToWideChar(CP_UTF8, 0, lpLogPath, -1, (WCHAR *)szLogPathA, MAX_PATH, NULL, NULL);
#endif
	}

	AGResourceVisitor::TransWinPathA(szLogPathA, szLogPathTrans, MAX_PATH);

	ret = rep.setLogFile(szLogPathTrans);

	return ret == 0 ? TRUE : FALSE;
}

bool AGEngineManager::JoinChannel(LPCTSTR lpChannelKey, LPCTSTR lpChannelName, UINT nUID)
{
	int nRet = 0;

//	m_lpAgoraEngine->setVideoProfile(VIDEO_PROFILE_720P);
#ifdef UNICODE
	CHAR szChannelName[128];
	CHAR szChannelKey[128];

	::WideCharToMultiByte(CP_ACP, 0, lpChannelName, -1, szChannelName, 128, NULL, NULL);
	::WideCharToMultiByte(CP_ACP, 0, lpChannelKey, -1, szChannelKey, 128, NULL, NULL);
	nRet = m_lpAgoraEngine->joinChannel(szChannelKey, szChannelName, NULL, nUID);
#else
	nRet = m_lpAgoraEngine->joinChannel(NULL, lpChannelName, NULL, nUID);
#endif

	if (nRet == 0)
		m_strChannelName = lpChannelName;
	
	return nRet == 0;
}

BOOL AGEngineManager::LeaveCahnnel()
{
	m_lpAgoraEngine->stopPreview();
	int nRet = m_lpAgoraEngine->leaveChannel();

	return nRet == 0 ? TRUE : FALSE;
}

std::wstring AGEngineManager::GetChanelName()
{
	return m_strChannelName;
}

std::string AGEngineManager::GetCallID()
{
	agora::util::AString uid;

	m_lpAgoraEngine->getCallId(uid);

	return std::string(uid->c_str());
}

BOOL AGEngineManager::EnableVideo(BOOL bEnable)
{
	int nRet = 0;

	if (bEnable) {
		nRet = m_lpAgoraEngine->enableVideo();
	}
	else {
		nRet = m_lpAgoraEngine->disableVideo();
	}

	if (nRet == 0) {
		m_bVideoEnable = bEnable;
	}

	return nRet == 0 ? TRUE : FALSE;
}

BOOL AGEngineManager::IsVideoEnabled()
{
	return m_bVideoEnable;
}

BOOL AGEngineManager::EnableScreenCapture(HWND hWnd, BOOL bEnable)
{
	assert(m_lpAgoraEngine != NULL);

	int ret = 0;
	RtcEngineParameters rep(*m_lpAgoraEngine);

	if (bEnable) {
		ret = rep.startScreenCapture(hWnd, 0, NULL);
	}
	else {
		ret = rep.stopScreenCapture();
	}

	if (ret == 0) {
		m_bScreenCapture = bEnable;
	}

	return ret == 0 ? TRUE : FALSE;
}

BOOL AGEngineManager::IsScreenCaptureEnabled()
{
	return m_bScreenCapture;
}

BOOL AGEngineManager::MuteLocalAudio(BOOL bMuted)
{
	assert(m_lpAgoraEngine != NULL);

	RtcEngineParameters rep(*m_lpAgoraEngine);

	int ret = rep.muteLocalAudioStream((bool)bMuted);
	if (ret == 0) {
		m_bLocalAudioMuted = bMuted;
	}

	return ret == 0 ? TRUE : FALSE;
}

BOOL AGEngineManager::IsLocalAudioMuted()
{
	return m_bLocalAudioMuted;
}

BOOL AGEngineManager::EnableLocalVideo(BOOL bMuted /*= TRUE*/)
{
	assert(m_lpAgoraEngine != NULL);

	RtcEngineParameters rep(*m_lpAgoraEngine);

	int ret = rep.enableLocalVideo((bool)bMuted);

	return ret == 0 ? TRUE : FALSE;
}

BOOL AGEngineManager::MuteLocalVideo(BOOL bMuted)
{
	assert(m_lpAgoraEngine != NULL);

	RtcEngineParameters rep(*m_lpAgoraEngine);

	int ret = rep.muteLocalVideoStream((bool)bMuted);
	if (ret == 0) {
		m_bLocalVideoMuted = bMuted;
	}

	return ret == 0 ? TRUE : FALSE;
}

BOOL AGEngineManager::IsLocalVideoMuted()
{
	return m_bLocalVideoMuted;
}

BOOL AGEngineManager::SetLocalVideoMirror(BOOL bEnable /*= TRUE*/)
{
	assert(m_lpAgoraEngine != NULL);

	RtcEngineParameters rep(*m_lpAgoraEngine);

	int ret = rep.setLocalVideoMirrorMode(bEnable ? VIDEO_MIRROR_MODE_ENABLED : VIDEO_MIRROR_MODE_DISABLED);

	return ret == 0 ? TRUE : FALSE;
}

BOOL AGEngineManager::EnableAudioRecording(BOOL bEnable, LPCTSTR lpFilePath)
{
	int ret = 0;

	RtcEngineParameters rep(*m_lpAgoraEngine);

	if (bEnable) {
#ifdef UNICODE
		CHAR szFilePath[MAX_PATH];
		::WideCharToMultiByte(CP_ACP, 0, lpFilePath, -1, szFilePath, MAX_PATH, NULL, NULL);
		ret = rep.startAudioRecording(szFilePath, AUDIO_RECORDING_QUALITY_HIGH);
#else
		ret = rep.startAudioRecording(lpFilePath);
#endif
	}
	else
		ret = rep.stopAudioRecording();

	return ret == 0 ? TRUE : FALSE;
}

BOOL AGEngineManager::EnableNetworkTest(BOOL bEnable)
{
	int ret = 0;

	
	if (bEnable) {
		ret = m_lpAgoraEngine->enableLastmileTest();
	}
	else {
		ret = m_lpAgoraEngine->disableLastmileTest();
	}

	return ret == 0 ? TRUE : FALSE;
}

BOOL AGEngineManager::RemoteVideoPreview(HWND hVideoWnd, uid_t uid)
{
	int nRet = 0;
	VideoCanvas vc;
	vc.uid = uid;
	vc.view = hVideoWnd;
	vc.renderMode = RENDER_MODE_TYPE::RENDER_MODE_HIDDEN;
		
	nRet = m_lpAgoraEngine->setupRemoteVideo(vc);
	return nRet == 0 ? TRUE : FALSE;
}


BOOL AGEngineManager::LocalVideoPreview(HWND hVideoWnd, BOOL bPreviewOn, BOOL bDualStreamMode)
{
	int nRet = 0;

	if (bPreviewOn && m_lpAgoraEngine) {
		VideoCanvas vc;

		vc.uid = 0;
		vc.view = hVideoWnd;
		vc.renderMode = RENDER_MODE_TYPE::RENDER_MODE_HIDDEN;
		MuteLocalVideo(FALSE);
		SetLocalVideoMirror(FALSE);
		m_lpAgoraEngine->setupLocalVideo(vc);
		EnableDualStreamMode(bDualStreamMode);
		nRet = m_lpAgoraEngine->startPreview();
	}
	else
	{
		MuteLocalVideo(TRUE);
		nRet = m_lpAgoraEngine->stopPreview();
	}
		
	return nRet == 0 ? TRUE : FALSE;
}

BOOL AGEngineManager::SetLogFilter(LOG_FILTER_TYPE logFilterType, LPCTSTR lpLogPath)
{
	int nRet = 0;
	RtcEngineParameters rep(*m_lpAgoraEngine);

	nRet = rep.setLogFilter(logFilterType);

#ifdef UNICODE
	CHAR szFilePath[MAX_PATH];
	::WideCharToMultiByte(CP_ACP, 0, lpLogPath, -1, szFilePath, MAX_PATH, NULL, NULL);
	nRet |= rep.setLogFile(szFilePath);
#else
	nRet |= rep.setLogFile(lpLogPath);
#endif

	return nRet == 0 ? TRUE : FALSE;
}

BOOL AGEngineManager::SetEncryptionSecret(LPCTSTR lpKey, int nEncryptType)
{
	CHAR szUTF8[MAX_PATH];

#ifdef UNICODE
	::WideCharToMultiByte(CP_UTF8, 0, lpKey, -1, szUTF8, MAX_PATH, NULL, NULL);
#else
	WCHAR szAnsi[MAX_PATH];
	::MultiByteToWideChar(CP_ACP, 0, lpKey, -1, szAnsi, MAX_PATH);
	::WideCharToMultiByte(CP_UTF8, 0, szAnsi, -1, szUTF8, MAX_PATH, NULL, NULL);
#endif
    switch (nEncryptType)
    {
    case 0:
        m_lpAgoraEngine->setEncryptionMode("aes-128-xts");
    	break;
    case 1:
        m_lpAgoraEngine->setEncryptionMode("aes-256-xts");
        break;
    default:
        m_lpAgoraEngine->setEncryptionMode("aes-128-xts");
        break;
    }
	int nRet = m_lpAgoraEngine->setEncryptionSecret(szUTF8);

	return nRet == 0 ? TRUE : FALSE;
}

BOOL AGEngineManager::EnableLocalRender(BOOL bEnable)
{
    int nRet = 0;

/*    if (bEnable)
        nRet = m_lpAgoraEngine->setParameters("{\"che.video.local.render\":true}");
    else
        nRet = m_lpAgoraEngine->setParameters("{\"che.video.local.render\":false}");
*/
    return nRet == 0 ? TRUE : FALSE;
}

int AGEngineManager::CreateMessageStream()
{
    int nDataStream = 0;
    m_lpAgoraEngine->createDataStream(&nDataStream, true, true);

    return nDataStream;
}

BOOL AGEngineManager::SendChatMessage(int nStreamID, LPCTSTR lpChatMessage)
{
    _ASSERT(nStreamID != 0);
    int nMessageLen = _tcslen(lpChatMessage);
    _ASSERT(nMessageLen < 128);

    CHAR szUTF8[256];

#ifdef UNICODE
    int nUTF8Len = ::WideCharToMultiByte(CP_UTF8, 0, lpChatMessage, nMessageLen, szUTF8, 256, NULL, NULL);
#else
    int nUTF8Len = ::MultiByteToWideChar(CP_UTF8, lpChatMessage, nMessageLen, szUTF8, 256);
#endif

    int nRet = m_lpAgoraEngine->sendStreamMessage(nStreamID, szUTF8, nUTF8Len);

    return nRet == 0 ? TRUE : FALSE;
}

BOOL AGEngineManager::EnableWhiteboardVer(BOOL bEnable)
{

	return TRUE;
}

BOOL AGEngineManager::EnableWhiteboardFeq(BOOL bEnable)
{

	return TRUE;
}


