#pragma once

#include "agora/IAgoraRtcEngine.h"
#include <string>
#include <wtypes.h>

using namespace agora::rtc;

// 引擎标记位
#define AG_ENGFLAG_ENNETTEST	0x00000001
#define AG_ENGFLAG_ECHOTEST		0x00000002
#define AG_ENGFLAG_SPKPHTEST	0x00000004
#define AG_ENGFLAG_MICPHTEST	0x00000008
#define AG_ENGFLAG_VIDEOTEST	0x00000010


#define AG_CODEC_E264	0x00000000
#define AG_CODEC_EVP	0x00000001
#define AG_CODEC_VP8	0x00000002

class AGEngineManager
{
public:
	AGEngineManager();
	~AGEngineManager(void);

	void SetNetworkTestFlag(BOOL bEnable);
	BOOL GetNetworkTestFlag();

	void SetEchoTestFlag(BOOL bEnable);
	BOOL GetEchoTestFlag();

	void SetSpeakerphoneTestFlag(BOOL bEnable);
	BOOL GetSpeakerphoneTestFlag();

	void SetMicrophoneTestFlag(BOOL bEnable);
	BOOL GetMicrophoneTestFlag();

	void SetVideoTestFlag(BOOL bEnable);
	BOOL GetVideoTestFlag();

	BOOL SetLogFilePath(LPCTSTR lpLogPath = NULL);

	bool JoinChannel(LPCTSTR lpChannelKey, LPCTSTR lpChannelName, UINT nUID = 0);
	BOOL LeaveCahnnel();
	std::wstring GetChanelName();
	std::string GetCallID();
	std::string GetVendorKey() { return m_strVendorKey; };

	void SetSelfUID(UINT nUID) { m_nSelfUID = nUID; };
	UINT GetSelfUID() { return m_nSelfUID; };

	BOOL EnableVideo(BOOL bEnable = TRUE);
	BOOL IsVideoEnabled();

	BOOL EnableScreenCapture(HWND hWnd, BOOL bEnable = TRUE);
	BOOL IsScreenCaptureEnabled();

	BOOL MuteLocalAudio(BOOL bMuted = TRUE);
	BOOL IsLocalAudioMuted();

	BOOL EnableLocalVideo(BOOL bMuted = TRUE);
	BOOL MuteLocalVideo(BOOL bMuted = TRUE);
	BOOL IsLocalVideoMuted();

	BOOL SetLocalVideoMirror(BOOL bEnable = TRUE);

	BOOL EnableAudioRecording(BOOL bEnable, LPCTSTR lpFilePath);

	BOOL EnableNetworkTest(BOOL bEnable);

	BOOL LocalVideoPreview(HWND hVideoWnd, BOOL bPreviewOn = TRUE, BOOL bDualStreamMode = FALSE);
	BOOL RemoteVideoPreview(HWND hVideoWnd, uid_t uid=0);

	BOOL SetLogFilter(LOG_FILTER_TYPE logFilterType, LPCTSTR lpLogPath);

    BOOL SetEncryptionSecret(LPCTSTR lpKey, int nEncryptType = 0);

    BOOL EnableLocalRender(BOOL bEnable);

    int CreateMessageStream();
    BOOL SendChatMessage(int nStreamID, LPCTSTR lpChatMessage);

	int StartRecordingService(std::string s_channel_key);
	int StopRecordingService(std::string s_channel_key);

	void SetChannelProfile(CHANNEL_PROFILE_TYPE profile);
	void SetClientRole(CLIENT_ROLE_TYPE role, std::string permissionKey);
	void EnableDualStreamMode(bool enabled);

	static IRtcEngine *GetEngine();
	
	static std::string GetSDKVersion();

	static BOOL EnableWhiteboardVer(BOOL bEnable);
	static BOOL EnableWhiteboardFeq(BOOL bEnable);

private:
	DWORD	m_dwEngineFlag;
	static  AGEngineManager	*m_lpAgoraObject;
	static	IRtcEngine	    *m_lpAgoraEngine;
	static  IRtcEngineEventHandler* m_lpEngineEventHandler;
	static	std::string	 m_strVendorKey;
	
	UINT		m_nSelfUID;
	std::wstring m_strChannelName;
	BOOL		m_bVideoEnable;

	BOOL		m_bLocalAudioMuted;
	BOOL		m_bLocalVideoMuted;
	BOOL		m_bScreenCapture;

public:
	static AGEngineManager *GetAGEngine(std::string s_vendorKey = "");
	static void CloseAGEngine();
};
