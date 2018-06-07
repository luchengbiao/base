#pragma once
#include "base\util\auto_unregister.h"
#include "base\callback\callback.h"
#include "base\memory\singleton.h"
#include <stdint.h>
#include <string>
#include <functional>
#include <map>
#include <memory>
#include <wtypes.h>
#include "agora\IAgoraRtcEngine.h"

typedef std::function<void(std::string channel_id, uint32_t uid, int elapsed)>	AgoraJoinSuccessCallback;
typedef std::function<void(std::string channel_id, uint32_t uid, int elapsed)>	AgoraReJoinSuccessCallback;
typedef std::function<void(int err, std::string msg)>	AgoraChannelErrorCallback;
typedef std::function<void(uint32_t uid, int elapsed)>	AgoraUserJoinCallback;
typedef std::function<void(uint32_t uid)>	AgoraUserOfflineCallback;
typedef std::function<void()>	AgoraConnectLostCallback;
typedef std::function<void()>	AgoraLeaveChannelCallback;
typedef std::function<void(uint32_t, int, uint16_t, uint16_t)>	AgoraAudioQualityCallback;
typedef std::function<void(uint32_t, int, int)>	AgoraNetworkQualityCallback;

class AGVchatManager : public nbase::SupportWeakCallback
{
public:
	SINGLETON_DEFINE(AGVchatManager);
	AGVchatManager();
	~AGVchatManager();
	
	// reg cb
	UnregisterCallback RegJoinSuccessCb(const AgoraJoinSuccessCallback& callback);
	UnregisterCallback RegReJoinSuccessCb(const AgoraReJoinSuccessCallback& callback);
	UnregisterCallback RegChannelErrorCb(const AgoraChannelErrorCallback& callback);
	UnregisterCallback RegUserJoinCb(const AgoraUserJoinCallback& callback);
	UnregisterCallback RegUserOfflineCb(const AgoraUserOfflineCallback& callback);
	UnregisterCallback RegConnectLostCb(const AgoraConnectLostCallback& callback);
	UnregisterCallback RegLeaveChannelCb(const AgoraLeaveChannelCallback& callback);
	UnregisterCallback RegAudioQualityCb(const AgoraAudioQualityCallback& callback);
	UnregisterCallback RegNetworkQualityCb(const AgoraNetworkQualityCallback& callback);

	// cb
	void OnJoinChannelSuccess(std::string channel_id, uint32_t uid, int elapsed);
	void OnRejoinChannelSuccess(std::string channel_id, uint32_t uid, int elapsed);
	void OnError(int err, std::string msg);
	void OnLeaveChannel();
	void OnUserJoined(uint32_t uid, int elapsed);
	void OnUserOffline(uint32_t uid);
	void OnConnectionLost();
	void onAudioQuality(uint32_t uid, int quality, uint16_t delay, uint16_t lost); // 频道内声音质量回调
	void onNetworkQuality(uint32_t uid, int txQuality, int rxQuality);			   // 频道内网络质量回调	

	//
	bool JoinChannel(LPCTSTR lpChannelKey, LPCTSTR lpChannelName, UINT nUID = 0);
	BOOL LeaveCahnnel();
	BOOL SetLogFilePath(LPCTSTR lpLogPath = NULL);
	
	BOOL MuteLocalAudio(BOOL bMuted = TRUE);
	BOOL IsLocalAudioMuted();

	BOOL EnableLocalVideo(BOOL bMuted = TRUE);
	BOOL MuteLocalVideo(BOOL bMuted = TRUE);
	BOOL IsLocalVideoMuted();

	BOOL EnableVideo(BOOL bEnable = TRUE);
	void SetChannelProfile(agora::rtc::CHANNEL_PROFILE_TYPE profile);
	void SetClientRole(agora::rtc::CLIENT_ROLE_TYPE role, std::string permissionKey);

	int StartRecordingService(std::string s_channel_key);
	int StopRecordingService(std::string s_channel_key);

	void SetAppId(std::string app_id);
	std::string GetAppId();

private:
	std::string app_id_;
	std::map<int, std::unique_ptr<AgoraJoinSuccessCallback>> join_success_cb_list_;
	std::map<int, std::unique_ptr<AgoraReJoinSuccessCallback>> rejoin_success_cb_list_;
	std::map<int, std::unique_ptr<AgoraChannelErrorCallback>> channel_error_cb_list_;
	std::map<int, std::unique_ptr<AgoraUserJoinCallback>> user_join_cb_list_;
	std::map<int, std::unique_ptr<AgoraUserOfflineCallback>> user_offline_cb_list_;
	std::map<int, std::unique_ptr<AgoraConnectLostCallback>> connect_lost_cb_list_;
	std::map<int, std::unique_ptr<AgoraLeaveChannelCallback>> leave_channel_cb_list_;
	std::map<int, std::unique_ptr<AgoraAudioQualityCallback>> audio_quality_cb_list_;
	std::map<int, std::unique_ptr<AgoraNetworkQualityCallback>> network_quality_cb_list_;
};
