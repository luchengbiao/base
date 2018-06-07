#pragma once
#include "base_chat_channel.h"
#include "base/util/auto_unregister.h"

//音视频服务
class AgoraChatChannel : public BaseChatChannel
{
public:
	AgoraChatChannel(bool master);
	AgoraChatChannel(){};
	virtual ~AgoraChatChannel();

	virtual bool JoinChannel(JoinChatCb cb, uint64_t m_uid, std::string channel_id, std::string channel_key);
	virtual bool ReJoinChannel();
	virtual void LeaveChannel();
	virtual void InitDevices(int type, std::string speaker_device, std::string mic_device);

	virtual void StartRecordService(std::string channel_id);   //channel_id == s_recording_key_ ; 传recording_key
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

	virtual void FreeVedioDevice(HWND vedio_hwnd, uint32_t m_stu_uid);
	virtual void OpenTeaVedio(HWND vedio_hwnd, std::string str_id = "");
	virtual bool InitTeaVedioDevice(HWND vedio_hwnd);
	virtual void InitStuVedioDevice(HWND vedio_hwnd, uint32_t m_stu_uid);

	virtual NetworkServiceType GetChannelType(){ return SWITCH_NETWORK_AGORA; }
	virtual std::string GetRecordingKey() override;
private:
	bool SpeakerGlobleDeviceCheck(std::string mic_device);
	bool MicroGlobleDeviceCheck(std::string mic_device);

	void OnJoinMediaChannelSuccess(std::string s_channel_id, uint32_t uid, int elapsed);
private:
	JoinChatCb join_cb_;
	std::string camera_str_id_;
	std::string s_recording_key_;

	AgoraCameraManager*		    m_agCamera_;
	AgoraPlayoutManager*		m_agPlayout_;
	AgoraAudInputManager*		m_agAudioin_;
	UnregisterCallback			channel_join_cb_;
};