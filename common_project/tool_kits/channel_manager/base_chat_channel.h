#pragma once
#include "base\callback\callback.h"
#include "channel_manager_global.h"
#include "agora_manager/AgoraDeviceManager.h"
#include <wtypes.h>

//“Ù ”∆µ∑˛ŒÒ
class BaseChatChannel : public nbase::SupportWeakCallback
{
public:
	BaseChatChannel(bool master);
	BaseChatChannel(){};
	virtual ~BaseChatChannel();

	virtual bool JoinChannel(JoinChatCb cb, uint64_t m_uid, std::string channel_id, std::string channel_key) = 0;
	virtual bool ReJoinChannel() = 0;
	virtual void LeaveChannel() = 0;
	virtual void InitDevices(int type,std::string speaker_device, std::string mic_device) = 0;

	virtual void StartRecordService(std::string channel_id) = 0;
	virtual void StopRecordService() = 0;

	virtual void OnVolumnChange(int number) = 0;
	virtual void OnMicrophoneChange(int number) = 0;
	virtual int OnGetVolumn() = 0;
	virtual int OnGetMicrophoneNum() = 0;

	virtual void FreeAudioDevice() = 0;
	virtual void FreeMicroDevice() = 0;
	virtual void FreeTeaVedio() = 0;

	virtual void InitAudioDevice(std::string speaker_device) = 0;
	virtual void InitMicroDevice(std::string mic_device) = 0;

	virtual void FreeVedioDevice(HWND vedio_hwnd, uint32_t m_stu_uid) = 0;
	virtual void OpenTeaVedio(HWND vedio_hwnd, std::string str_id = "") = 0;
	virtual bool InitTeaVedioDevice(HWND vedio_hwnd) = 0;
	virtual void InitStuVedioDevice(HWND vedio_hwnd, uint32_t m_stu_uid) = 0;

	virtual int setClientRole(CLIENT_ROLE_TYPE role, const char* permissionKey) = 0;

	virtual std::string GetRecordingKey() = 0;

	virtual NetworkServiceType GetChannelType() = 0;
	std::string GetChannelId(){ return channel_id_; }
	void SetUserAccid(std::string user_accid){ u_accid_ = user_accid; }
public:
	bool master_;
	std::string channel_id_;
	std::string u_accid_;
};