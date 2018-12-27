#include "agora_chat_channel.h"
#include "agora_vchat/agora_vchat_manager.h"
#include "base/util/string_util.h"
#include "log\log.h"

AgoraChatChannel::AgoraChatChannel(bool master) :BaseChatChannel(master)
{
	m_agCamera_ = nullptr;
	m_agPlayout_ = nullptr;
	m_agAudioin_ = nullptr;
}

AgoraChatChannel::~AgoraChatChannel()
{

}

bool AgoraChatChannel::JoinChannel(JoinChatCb cb, uint64_t m_uid, std::string channel_id, std::string channel_key)
{
	join_cb_ = cb;
	channel_id_ = channel_id;
	auto join_fail_cb = nbase::Bind(&AgoraChatChannel::OnJoinMediaChannelSuccess, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	channel_join_cb_ = AGVchatManager::GetInstance()->RegJoinSuccessCb(join_fail_cb);

	AGVchatManager::GetInstance()->LeaveCahnnel();
	std::wstring channel_key_w = nbase::UTF8ToUTF16(channel_key);
	std::wstring channel_id_w = nbase::UTF8ToUTF16(channel_id);
	AGVchatManager::GetInstance()->JoinChannel(channel_key_w.c_str(), channel_id_w.c_str(), m_uid);

	return true;
}

bool AgoraChatChannel::ReJoinChannel()
{
	return true;
}

void AgoraChatChannel::LeaveChannel()
{
	if (channel_join_cb_)
	{
		channel_join_cb_();
	}
	AGVchatManager::GetInstance()->LeaveCahnnel();
}

void AgoraChatChannel::InitDevices(int type, std::string speaker_device, std::string mic_device)
{
	DeviceManagerParam* param = AgoraDeviceManager::GetInstance()->CreateDeviceManager();
	if (param)
	{
		m_agCamera_ = param->m_agCamera_;
		m_agPlayout_ = param->m_agPlayout_;
		m_agAudioin_ = param->m_agAudioin_;

		InitAudioDevice(speaker_device);
		InitMicroDevice(mic_device);
	}
}

void AgoraChatChannel::StartRecordService(std::string channel_id)
{
	s_recording_key_ = channel_id;
	AGVchatManager::GetInstance()->StartRecordingService(s_recording_key_);
}

void AgoraChatChannel::StopRecordService()
{
	AGVchatManager::GetInstance()->StopRecordingService(s_recording_key_);
}

void AgoraChatChannel::OnVolumnChange(int number)
{
	if (!m_agPlayout_)
	{
		return;
	}
	m_agPlayout_->SetVolume(number);
}

void AgoraChatChannel::OnMicrophoneChange(int number)
{
	m_agAudioin_->SetVolume(number);
}

int AgoraChatChannel::OnGetVolumn()
{
	return m_agPlayout_->GetVolume();
}

int AgoraChatChannel::OnGetMicrophoneNum()
{
	return m_agAudioin_->GetVolume();
}

void AgoraChatChannel::InitAudioDevice(std::string speaker_device)
{
	if (m_agPlayout_->GetDeviceCount() > 0)
	{
		if (SpeakerGlobleDeviceCheck(speaker_device))
		{
			std::wstring wstr_speaker = nbase::UTF8ToUTF16(speaker_device);
			m_agPlayout_->SetCurDevice(wstr_speaker.c_str());
		}
	}
}

void AgoraChatChannel::InitMicroDevice(std::string mic_device)
{
	//只有设备的时候才会去重新选择  不然不选择全局保存的
	if (m_agAudioin_->GetDeviceCount() > 0)
	{
		if (MicroGlobleDeviceCheck(mic_device))
		{
			std::wstring wstr_mic = nbase::UTF8ToUTF16(mic_device);
			m_agAudioin_->SetCurDevice(wstr_mic.c_str());
		}
	}
}

bool AgoraChatChannel::InitTeaVedioDevice(HWND vedio_hwnd)
{
	::ShowWindow(vedio_hwnd, SW_SHOW);
	if (m_agCamera_->GetDeviceCount() > 0)
	{
		camera_str_id_ = m_agCamera_->GetCurDeviceID();
		std::wstring wstrDeviceId = nbase::UTF8ToUTF16(camera_str_id_);
		m_agCamera_->SetCurDevice(wstrDeviceId.c_str());

		m_agCamera_->TestCameraDevice(vedio_hwnd, TRUE);
		return true;
	}

	return false;
}

void AgoraChatChannel::InitStuVedioDevice(HWND vedio_hwnd, uint32_t m_stu_uid)
{
	::ShowWindow(vedio_hwnd, SW_SHOW);
	m_agCamera_->TestRemoteCameraDevice(vedio_hwnd, m_stu_uid, TRUE);
}

void AgoraChatChannel::FreeAudioDevice()
{
	if (m_agPlayout_)
	{
		m_agPlayout_->Close();
	}
	
}

void AgoraChatChannel::FreeMicroDevice()
{
	if (m_agAudioin_)
	{
		m_agAudioin_->Close();
	}
	
}

void AgoraChatChannel::FreeVedioDevice(HWND vedio_hwnd, uint32_t m_stu_uid)
{
	if (m_agCamera_)
	{
		m_agCamera_->TestCameraDevice(NULL, FALSE);
		m_agCamera_->TestRemoteCameraDevice(vedio_hwnd, m_stu_uid, FALSE);

		m_agCamera_->Close();
	}
}

bool AgoraChatChannel::SpeakerGlobleDeviceCheck(std::string mic_device)
{
	if (m_agPlayout_->GetDeviceCount() <= 0)
	{
		return false;
	}

	//选择选取的设备
	if (mic_device.empty())
	{
		return false;
	}

	std::string mic_cur_device = m_agPlayout_->GetCurDeviceID();
	if (mic_device == mic_cur_device)
	{
		return false;
	}

	//检测是否在检测到的设备中
	std::string strDeviceName = "";
	std::string strDeviceID = "";

	for (UINT nIndex = 0; nIndex < m_agPlayout_->GetDeviceCount(); nIndex++){
		m_agPlayout_->GetDevice(nIndex, strDeviceName, strDeviceID);
		if (mic_device == strDeviceID)
		{
			return true;
		}
	}
	return false;
}

bool AgoraChatChannel::MicroGlobleDeviceCheck(std::string mic_device)
{
	if (m_agAudioin_->GetDeviceCount() <= 0)
	{
		return false;
	}

	//选择选取的设备
	if (mic_device.empty())
	{
		return false;
	}

	std::string mic_cur_device = m_agAudioin_->GetCurDeviceID();
	if (mic_device == mic_cur_device)
	{
		return false;
	}

	//检测是否在检测到的设备中
	std::string strDeviceName = "";
	std::string strDeviceID = "";

	for (UINT nIndex = 0; nIndex < m_agAudioin_->GetDeviceCount(); nIndex++){
		m_agAudioin_->GetDevice(nIndex, strDeviceName, strDeviceID);
		if (mic_device == strDeviceID)
		{
			return true;
		}
	}
	return false;
}

void AgoraChatChannel::FreeTeaVedio()
{
	m_agCamera_->TestCameraDevice(NULL, FALSE);
}

void AgoraChatChannel::OpenTeaVedio(HWND vedio_hwnd, std::string str_id)
{
	if (!str_id.empty())
	{
		AGVchatManager::GetInstance()->EnableLocalVideo(TRUE);
		std::wstring wstrDeviceId = nbase::UTF8ToUTF16(str_id);
		m_agCamera_->SetCurDevice(wstrDeviceId.c_str());
	}
	m_agCamera_->TestCameraDevice(vedio_hwnd, TRUE);
}

std::string AgoraChatChannel::GetRecordingKey()
{
	return s_recording_key_;
}

void AgoraChatChannel::OnJoinMediaChannelSuccess(std::string s_channel_id, uint32_t uid, int elapsed)
{
	if (join_cb_)
	{
		LOG_O2O(L"AgoraChatChannel::OnJoinMediaChannelSuccess ChannelId: {0},current userId {1}") << s_channel_id << uid;
		join_cb_(200, s_channel_id, uid, ""); 
	}
}

int AgoraChatChannel::setClientRole(CLIENT_ROLE_TYPE role, const char* permissionKey)
{
	return AgoraDeviceManager::GetInstance()->setClientRole(role, permissionKey);
}

