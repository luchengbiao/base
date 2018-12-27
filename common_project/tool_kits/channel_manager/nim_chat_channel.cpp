#include "nim_chat_channel.h"
#include "nim_vchat/manager/vchat_manager.h"
#include "base/util/string_number_conversions.h"
#include "qthread_manager/closure.h"

NimChatChannel::NimChatChannel(bool master) :BaseChatChannel(master)
{
	reconnect_count_ = 0;
}

NimChatChannel::~NimChatChannel()
{

}

bool NimChatChannel::JoinChannel(JoinChatCb cb, uint64_t m_uid, std::string channel_id, std::string channel_key)
{
	join_cb_ = cb;
	channel_id_ = channel_id;
	room_id_ = channel_id;
	if (!room_id_.empty())
	{
		if (master_)
		{
			nim_comp::VChatManager::GetInstance()->CreateRoom(room_id_, "jyxb-pc", nbase::Bind(&NimChatChannel::OnCreateChannelCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
		}
		else
		{
			StartVChat();
		}
	}
	else
	{
		join_cb_(-1, room_id_, 0, "");
	}

	return true;
}

bool NimChatChannel::ReJoinChannel()
{
	return true;
}

void NimChatChannel::LeaveChannel()
{
	nim_comp::VChatManager::GetInstance()->EndChat(u_accid_);
}

void NimChatChannel::InitDevices(int type, std::string speaker_device, std::string mic_device)
{
	nim_comp::VChatManager::GetInstance()->SetVoipMode(nim::kNIMVideoChatModeVideo);
	nim_comp::VChatManager::GetInstance()->SetAudioVolumn(255, true);
	nim_comp::VChatManager::GetInstance()->SetAudioVolumn(255, false);

	InitAudioDevice(speaker_device);
	InitMicroDevice(mic_device);

}

void NimChatChannel::StartRecordService(std::string channel_id)
{

}

void NimChatChannel::StopRecordService()
{

}

void NimChatChannel::OnVolumnChange(int number)
{
	nim_comp::VChatManager::GetInstance()->SetAudioInputAutoVolumn(false);
	nim_comp::VChatManager::GetInstance()->SetAudioVolumn(number, false);
}

void NimChatChannel::OnMicrophoneChange(int number)
{
	nim_comp::VChatManager::GetInstance()->SetAudioInputAutoVolumn(false);
	nim_comp::VChatManager::GetInstance()->SetAudioVolumn(number, true);
}

int NimChatChannel::OnGetVolumn()
{
	int volumn = nim_comp::VChatManager::GetInstance()->GetAudioVolumn(false);
	return volumn;
}

int NimChatChannel::OnGetMicrophoneNum()
{
	int volumn = nim_comp::VChatManager::GetInstance()->GetAudioVolumn(true);
	return volumn;
}

void NimChatChannel::InitAudioDevice(std::string speaker_device)
{
	nim_comp::VChatManager::GetInstance()->EndDevice(nim::kNIMDeviceTypeAudioOut, nim_comp::kDeviceSessionTypeSetting);
	nim_comp::VChatManager::GetInstance()->EndDevice(nim::kNIMDeviceTypeAudioOutChat, nim_comp::kDeviceSessionTypeRts);

	if (speaker_device.empty())
	{
		int no = 0;
		nim_comp::VChatManager::GetInstance()->GetDefaultDevicePath(no, speaker_device, nim::kNIMDeviceTypeAudioOutChat);
	}

	nim_comp::VChatManager::GetInstance()->StartDevice(nim::kNIMDeviceTypeAudioOutChat, speaker_device, nim_comp::kDeviceSessionTypeRts);
}

void NimChatChannel::InitMicroDevice(std::string mic_device)
{
	nim_comp::VChatManager::GetInstance()->EndDevice(nim::kNIMDeviceTypeAudioIn, nim_comp::kDeviceSessionTypeRts);

	if (mic_device.empty())
	{
		int no = 0;
		nim_comp::VChatManager::GetInstance()->GetDefaultDevicePath(no, mic_device, nim::kNIMDeviceTypeAudioIn);
	}

	nim_comp::VChatManager::GetInstance()->StartDevice(nim::kNIMDeviceTypeAudioIn, mic_device, nim_comp::kDeviceSessionTypeRts);
}

bool NimChatChannel::InitTeaVedioDevice(HWND vedio_hwnd)
{
	::ShowWindow(vedio_hwnd, SW_HIDE);
	nim_comp::VChatManager::GetInstance()->EndDevice(nim::kNIMDeviceTypeVideo, nim_comp::kDeviceSessionTypeRts);

	std::string def_audio;
	int no = 0;
	nim_comp::VChatManager::GetInstance()->GetDefaultDevicePath(no, def_audio, nim::kNIMDeviceTypeVideo);
	if (def_audio.empty())
	{
		return false;
	}

	nim_comp::VChatManager::GetInstance()->StartDevice(nim::kNIMDeviceTypeVideo, def_audio, nim_comp::kDeviceSessionTypeRts);
	return true;
}

void NimChatChannel::InitStuVedioDevice(HWND vedio_hwnd, uint32_t m_stu_uid)
{
	::ShowWindow(vedio_hwnd, SW_HIDE);
}

void NimChatChannel::OnCreateChannelCallback(int code, __int64 channel_id, const std::string& json_extension)
{
	if (code == 200)
	{
		StartVChat();
	}
	else
	{
		if (join_cb_)
		{
			join_cb_(code, room_id_, 0, json_extension);
		}
	}
}

void NimChatChannel::StartVChat()
{
	auto cb = ToWeakCallback([=](int code, __int64 channel_id, const std::string& json_extension){
		if (code == 200)
		{
			channel_id_ = nbase::Int64ToString(channel_id);

			if (join_cb_)
			{
				join_cb_(code, room_id_, 0, json_extension);
			}
		}

	});
	
	bool ret = nim_comp::VChatManager::GetInstance()->JoinRoom(nim::kNIMVideoChatModeVideo, room_id_, "", false, u_accid_, cb);

	if (!ret)
	{
		if (reconnect_count_ <= 5)
		{
			++reconnect_count_;
			nim_comp::VChatManager::GetInstance()->EndChat(u_accid_);
			StdClosure task = nbase::Bind(&NimChatChannel::StartVChat, this);
			qtbase::Post2DelayedTask(kThreadUIHelper, task, nbase::TimeDelta::FromSeconds(2));
		}
		else
		{
			if (join_cb_)
			{
				join_cb_(-1, room_id_, 0, "");
			}
		}
	}
}

void NimChatChannel::FreeAudioDevice()
{
	nim_comp::VChatManager::GetInstance()->EndDevice(nim::kNIMDeviceTypeAudioOutChat, nim_comp::kDeviceSessionTypeRts);
}

void NimChatChannel::FreeMicroDevice()
{
	nim_comp::VChatManager::GetInstance()->EndDevice(nim::kNIMDeviceTypeAudioIn, nim_comp::kDeviceSessionTypeRts);
}

void NimChatChannel::FreeVedioDevice(HWND vedio_hwnd, uint32_t m_stu_uid)
{
	nim_comp::VChatManager::GetInstance()->EndDevice(nim::kNIMDeviceTypeVideo, nim_comp::kDeviceSessionTypeRts);
}

void NimChatChannel::FreeTeaVedio()
{
	nim_comp::VChatManager::GetInstance()->EndDevice(nim::kNIMDeviceTypeVideo, nim_comp::kDeviceSessionTypeRts);
}

void NimChatChannel::OpenTeaVedio(HWND vedio_hwnd, std::string str_id)
{
	InitTeaVedioDevice(vedio_hwnd);
}

std::string NimChatChannel::GetRecordingKey()
{
	return "";
}

int NimChatChannel::setClientRole(CLIENT_ROLE_TYPE role, const char* permissionKey)
{
	return 0;
}
