#pragma once
#include "base_chat_channel.h"

//“Ù ”∆µ∑˛ŒÒ
class NimChatChannel : public BaseChatChannel
{
public:
	NimChatChannel(bool master);
	virtual ~NimChatChannel();

	virtual bool JoinChannel(JoinChatCb cb, uint64_t m_uid, std::string channel_id, std::string channel_key);
	virtual bool ReJoinChannel();
	virtual void LeaveChannel();
	virtual void InitDevices(int type,std::string speaker_device, std::string mic_device);
	virtual int setClientRole(CLIENT_ROLE_TYPE role, const char* permissionKey);
	virtual void StartRecordService(std::string channel_id);
	virtual void StopRecordService();

	virtual void OnVolumnChange(int number);
	virtual void OnMicrophoneChange(int number);
	virtual int OnGetVolumn();
	virtual int OnGetMicrophoneNum();

	virtual void FreeAudioDevice();
	virtual void FreeMicroDevice();
	virtual void FreeTeaVedio();
	virtual void InitAudioDevice(std::string speaker_device);
	virtual void InitMicroDevice(std::string mic_device);

	virtual NetworkServiceType GetChannelType(){ return SWITCH_NETWORK_NETEASE; }

	virtual void FreeVedioDevice(HWND vedio_hwnd, uint32_t m_stu_uid);
	virtual void OpenTeaVedio(HWND vedio_hwnd, std::string str_id = "");
	virtual bool InitTeaVedioDevice(HWND vedio_hwnd);
	virtual void InitStuVedioDevice(HWND vedio_hwnd, uint32_t m_stu_uid);
	virtual std::string GetRecordingKey() override;
private:
	void StartVChat();
	void OnCreateChannelCallback(int code, __int64 channel_id, const std::string& json_extension);

private:
	JoinChatCb join_cb_;
	std::string room_id_;
	int reconnect_count_;
};