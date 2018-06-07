#include "agora_vchat_manager.h"
#include "..\agora_engine_sdk\manager\engine_manager.h"

AGVchatManager::AGVchatManager()
{
	app_id_ = "";
}

AGVchatManager::~AGVchatManager()
{

}

UnregisterCallback AGVchatManager::RegJoinSuccessCb(const AgoraJoinSuccessCallback& callback)
{
	AgoraJoinSuccessCallback* new_callback = new AgoraJoinSuccessCallback(callback);
	int cb_id = (int)new_callback;
	join_success_cb_list_[cb_id].reset(new_callback);

	auto cb = ToWeakCallback([this, cb_id]() {
		join_success_cb_list_.erase(cb_id);
	});
	return cb;
}

UnregisterCallback AGVchatManager::RegReJoinSuccessCb(const AgoraReJoinSuccessCallback& callback)
{
	AgoraReJoinSuccessCallback* new_callback = new AgoraReJoinSuccessCallback(callback);
	int cb_id = (int)new_callback;
	rejoin_success_cb_list_[cb_id].reset(new_callback);

	auto cb = ToWeakCallback([this, cb_id]() {
		rejoin_success_cb_list_.erase(cb_id);
	});
	return cb;
}

UnregisterCallback AGVchatManager::RegChannelErrorCb(const AgoraChannelErrorCallback& callback)
{
	AgoraChannelErrorCallback* new_callback = new AgoraChannelErrorCallback(callback);
	int cb_id = (int)new_callback;
	channel_error_cb_list_[cb_id].reset(new_callback);

	auto cb = ToWeakCallback([this, cb_id]() {
		channel_error_cb_list_.erase(cb_id);
	});
	return cb;
}

UnregisterCallback AGVchatManager::RegUserJoinCb(const AgoraUserJoinCallback& callback)
{
	AgoraUserJoinCallback* new_callback = new AgoraUserJoinCallback(callback);
	int cb_id = (int)new_callback;
	user_join_cb_list_[cb_id].reset(new_callback);

	auto cb = ToWeakCallback([this, cb_id]() {
		user_join_cb_list_.erase(cb_id);
	});
	return cb;
}

UnregisterCallback AGVchatManager::RegUserOfflineCb(const AgoraUserOfflineCallback& callback)
{
	AgoraUserOfflineCallback* new_callback = new AgoraUserOfflineCallback(callback);
	int cb_id = (int)new_callback;
	user_offline_cb_list_[cb_id].reset(new_callback);

	auto cb = ToWeakCallback([this, cb_id]() {
		user_offline_cb_list_.erase(cb_id);
	});
	return cb;
}

UnregisterCallback AGVchatManager::RegConnectLostCb(const AgoraConnectLostCallback& callback)
{
	AgoraConnectLostCallback* new_callback = new AgoraConnectLostCallback(callback);
	int cb_id = (int)new_callback;
	connect_lost_cb_list_[cb_id].reset(new_callback);

	auto cb = ToWeakCallback([this, cb_id]() {
		connect_lost_cb_list_.erase(cb_id);
	});
	return cb;
}

UnregisterCallback AGVchatManager::RegLeaveChannelCb(const AgoraLeaveChannelCallback& callback)
{
	AgoraLeaveChannelCallback* new_callback = new AgoraLeaveChannelCallback(callback);
	int cb_id = (int)new_callback;
	leave_channel_cb_list_[cb_id].reset(new_callback);

	auto cb = ToWeakCallback([this, cb_id]() {
		leave_channel_cb_list_.erase(cb_id);
	});
	return cb;
}

UnregisterCallback AGVchatManager::RegAudioQualityCb(const AgoraAudioQualityCallback& callback)
{
	AgoraAudioQualityCallback* new_callback = new AgoraAudioQualityCallback(callback);
	int cb_id = (int)new_callback;
	audio_quality_cb_list_[cb_id].reset(new_callback);

	auto cb = ToWeakCallback([this, cb_id]() {
		audio_quality_cb_list_.erase(cb_id);
	});
	return cb;
}

UnregisterCallback AGVchatManager::RegNetworkQualityCb(const AgoraNetworkQualityCallback& callback)
{
	AgoraNetworkQualityCallback* new_callback = new AgoraNetworkQualityCallback(callback);
	int cb_id = (int)new_callback;
	network_quality_cb_list_[cb_id].reset(new_callback);

	auto cb = ToWeakCallback([this, cb_id]() {
		network_quality_cb_list_.erase(cb_id);
	});
	return cb;
}

void AGVchatManager::OnJoinChannelSuccess(std::string channel_id, uint32_t uid, int elapsed)
{
	for (auto& it : join_success_cb_list_)
	{
		(*(it.second))(channel_id, uid, elapsed);
	}
}

void AGVchatManager::OnRejoinChannelSuccess(std::string channel_id, uint32_t uid, int elapsed)
{
	for (auto& it : rejoin_success_cb_list_)
	{
		(*(it.second))(channel_id, uid, elapsed);
	}
}

void AGVchatManager::OnError(int err, std::string msg)
{
	for (auto& it : channel_error_cb_list_)
	{
		(*(it.second))(err, msg);
	}
}

void AGVchatManager::OnLeaveChannel()
{
	for (auto& it : leave_channel_cb_list_)
	{
		(*(it.second))();
	}
}

void AGVchatManager::OnUserJoined(uint32_t uid, int elapsed)
{
	for (auto& it : user_join_cb_list_)
	{
		(*(it.second))(uid, elapsed);
	}
}

void AGVchatManager::OnUserOffline(uint32_t uid)
{
	for (auto& it : user_offline_cb_list_)
	{
		(*(it.second))(uid);
	}
}

void AGVchatManager::OnConnectionLost()
{
	for (auto& it : connect_lost_cb_list_)
	{
		(*(it.second))();
	}
}

void AGVchatManager::onAudioQuality(uint32_t uid, int quality, uint16_t delay, uint16_t lost)
{
	for (auto& it : audio_quality_cb_list_)
	{
		(*(it.second))(uid, quality, delay, lost);
	}
}

void AGVchatManager::onNetworkQuality(uint32_t uid, int txQuality, int rxQuality)
{
	for (auto& it : network_quality_cb_list_)
	{
		(*(it.second))(uid, txQuality, rxQuality);
	}
}

bool AGVchatManager::JoinChannel(LPCTSTR lpChannelKey, LPCTSTR lpChannelName, UINT nUID /*= 0*/)
{
	return AGEngineManager::GetAGEngine(app_id_)->JoinChannel(lpChannelKey, lpChannelName, nUID);
}

BOOL AGVchatManager::LeaveCahnnel()
{
	return AGEngineManager::GetAGEngine(app_id_)->LeaveCahnnel();
}

BOOL AGVchatManager::SetLogFilePath(LPCTSTR lpLogPath /*= NULL*/)
{
	return AGEngineManager::GetAGEngine(app_id_)->SetLogFilePath(lpLogPath);
}

BOOL AGVchatManager::MuteLocalAudio(BOOL bMuted /*= TRUE*/)
{
	return AGEngineManager::GetAGEngine(app_id_)->MuteLocalAudio(bMuted);
}

BOOL AGVchatManager::IsLocalAudioMuted()
{
	return AGEngineManager::GetAGEngine(app_id_)->IsLocalAudioMuted();
}

BOOL AGVchatManager::EnableLocalVideo(BOOL bMuted /*= TRUE*/)
{
	return AGEngineManager::GetAGEngine(app_id_)->EnableLocalVideo(bMuted);
}

BOOL AGVchatManager::MuteLocalVideo(BOOL bMuted /*= TRUE*/)
{
	return AGEngineManager::GetAGEngine(app_id_)->MuteLocalVideo(bMuted);
}

BOOL AGVchatManager::IsLocalVideoMuted()
{
	return AGEngineManager::GetAGEngine(app_id_)->IsLocalVideoMuted();
}

int AGVchatManager::StartRecordingService(std::string s_channel_key)
{
	return AGEngineManager::GetAGEngine(app_id_)->StartRecordingService(s_channel_key);
}

int AGVchatManager::StopRecordingService(std::string s_channel_key)
{
	return AGEngineManager::GetAGEngine(app_id_)->StopRecordingService(s_channel_key);
}

void AGVchatManager::SetAppId(std::string app_id)
{
	app_id_ = app_id;
}

std::string AGVchatManager::GetAppId()
{
	return app_id_;
}

BOOL AGVchatManager::EnableVideo(BOOL bEnable /*= TRUE*/)
{
	return AGEngineManager::GetAGEngine(app_id_)->EnableVideo(bEnable);
}

void AGVchatManager::SetChannelProfile(agora::rtc::CHANNEL_PROFILE_TYPE profile)
{
	AGEngineManager::GetAGEngine(app_id_)->SetChannelProfile(profile);
}

void AGVchatManager::SetClientRole(agora::rtc::CLIENT_ROLE_TYPE role, std::string permissionKey)
{
	AGEngineManager::GetAGEngine(app_id_)->SetClientRole(role, permissionKey);
}