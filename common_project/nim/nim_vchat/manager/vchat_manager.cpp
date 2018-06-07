#include "vchat_manager.h"
#include "log_manager\log.h"
#include "json\json.h"
#include "nim_sdk_manager\api\nim_cpp_rts.h"
#include "callback\vchat_callback.h"
#include "nim_sdk_manager\api\nim_cpp_vchat.h"
#include "base\util\string_util.h"

namespace nim_comp
{

VChatManager::VChatManager()
{
	for (int i = nim::kNIMDeviceTypeAudioIn; i <= nim::kNIMDeviceTypeVideo; i++)
	{
		device_session_type_[i] = kDeviceSessionTypeNone;
	}
	chatroom_connect_cb_ = nullptr;
	chatroom_people_cb_ = nullptr;
	chatroom_volume_cb_ = nullptr;
	audio_volume_cb_ = nullptr;
	nim::VChat::SetAudioDataCb(true, &VChatCallback::AudioVolumCb);
}

VChatManager::~VChatManager()
{

}

void VChatManager::OnDeviceStatus(nim::NIMDeviceType type, UINT status, std::string path)
{
	if (device_session_type_[type] == kDeviceSessionTypeNone)
	{
		nim::VChat::RemoveDeviceStatusCb(type);
	}
}
void VChatManager::OnStartDeviceCb(nim::NIMDeviceType type, bool ret)
{
	
}

void VChatManager::OnVChatEvent(nim::NIMVideoChatSessionType type, uint64_t channel_id, int code, const std::string& json)
{
	if (type != nim::kNIMVideoChatSessionTypeInfoNotify && type != nim::kNIMVideoChatSessionTypeVolumeNotify)
	{
		QLOG_APP(L"OnVChatEvent type={0}, channel_id={1}, code={2}, json={3}") << type << channel_id << code << json;
	}
	switch (type)
	{
	case nim::kNIMVideoChatSessionTypeStartRes:{
		QLOG_ERR(L"ChatStart no window");
		EndChat(json);
	}break;
	case nim::kNIMVideoChatSessionTypeInviteNotify:{
		Json::Value valus;
		Json::Reader reader;
		if (reader.parse(json, valus))
		{
			std::string uid = valus[nim::kNIMVChatUid].asString();
			int mode = valus[nim::kNIMVChatType].asInt();
			
			VChatControl(channel_id, nim::kNIMTagControlBusyLine);
			VChatCalleeAck(channel_id, false, "");
		}
	}break;
	case nim::kNIMVideoChatSessionTypeCalleeAckRes:{
		Json::Value valus;
		Json::Reader reader;
		if (reader.parse(json, valus))
		{
			bool accept = valus[nim::kNIMVChatAccept].asInt() > 0;
			if (accept && code != 200)
			{
				
			}
		}
	}break;
	case nim::kNIMVideoChatSessionTypeCalleeAckNotify:{
		Json::Value valus;
		Json::Reader reader;
		if (reader.parse(json, valus))
		{
			bool accept = valus[nim::kNIMVChatAccept].asInt() > 0;
		}
	}break;
	case nim::kNIMVideoChatSessionTypeControlRes:{
		//
	}break;
	case nim::kNIMVideoChatSessionTypeControlNotify:{
		Json::Value valus;
		Json::Reader reader;
		if (reader.parse(json, valus))
		{
			int type = valus[nim::kNIMVChatType].asInt();
		}
	}break;
	case nim::kNIMVideoChatSessionTypeConnect:{
		if (chatroom_connect_cb_)
		{
			chatroom_connect_cb_(code);
		}
	}break;
	case nim::kNIMVideoChatSessionTypePeopleStatus:{
		if (chatroom_people_cb_)
		{
			Json::Value valus;
			Json::Reader reader;
			if (reader.parse(json, valus))
			{
				std::string uid = valus[nim::kNIMVChatUid].asString();
				if (!uid.empty())
				{
					chatroom_people_cb_(uid, code == nim::kNIMVideoChatSessionStatusJoined);
				}
			}
		}
	}break;
	case nim::kNIMVideoChatSessionTypeNetStatus:{
		//
	}break;
	case nim::kNIMVideoChatSessionTypeHangupRes:{
		//
	}break;
	case nim::kNIMVideoChatSessionTypeHangupNotify:{
		//
	}break;
	case nim::kNIMVideoChatSessionTypeSyncAckNotify:{
		Json::Value valus;
		Json::Reader reader;
		if (reader.parse(json, valus))
		{
			bool accept = valus[nim::kNIMVChatAccept].asInt() > 0;
		}
	}break;
	case nim::kNIMVideoChatSessionTypeMp4Notify:{
		Json::Value valus;
		Json::Reader reader;
		if (reader.parse(json, valus))
		{
			
		}
	}break;
	case nim::kNIMVideoChatSessionTypeVolumeNotify:{
		if (chatroom_volume_cb_)
		{
			Json::Value valus;
			Json::Reader reader;
			if (reader.parse(json, valus))
			{
				if (valus.isObject() && valus[nim::kNIMVChatAudioVolume].isObject())
				{
					int32_t self_volume = valus[nim::kNIMVChatAudioVolume][nim::kNIMVChatSelf][nim::kNIMVChatStatus].asInt();
					std::map<std::string, int32_t> other_volume;
					if (valus[nim::kNIMVChatAudioVolume][nim::kNIMVChatReceiver].isArray())
					{
						int count = valus[nim::kNIMVChatAudioVolume][nim::kNIMVChatReceiver].size();
						for (int i = 0; i < count; ++i)
						{
							Json::Value item;
							item = valus[nim::kNIMVChatAudioVolume][nim::kNIMVChatReceiver].get(i, item);
							std::string uid = item[nim::kNIMVChatUid].asString();
							int32_t status = item[nim::kNIMVChatStatus].asInt();
							other_volume[uid] = status;
						}
					}
					chatroom_volume_cb_(self_volume, other_volume);
				}
			}
		}
	}break;
	}
}

void VChatManager::GetDeviceByJson(bool ret, nim::NIMDeviceType type, const char* json)
{
	device_info_list_[type].clear();
	if (ret)
	{
		Json::Value valus;
		Json::Reader reader;
		if (reader.parse(json, valus) && valus.isArray())
		{
			int num = valus.size();
			for (int i=0;i<num;++i)
			{
				Json::Value device;
				device = valus.get(i, device);
				MEDIA_DEVICE_DRIVE_INFO info;
				info.device_path_ = device[nim::kNIMDevicePath].asString();
				info.friendly_name_ = device[nim::kNIMDeviceName].asString();
				device_info_list_[type].push_back(info);
			}
		}
	}
}

bool VChatManager::GetDefaultDevicePath(int &no, std::string &device_path, nim::NIMDeviceType type)
{
	if (type == nim::kNIMDeviceTypeAudioOutChat)
	{
		type = nim::kNIMDeviceTypeAudioOut;
	}
	no = 0;
	device_path.clear();
	GetDeviceInfo(type);
	if(device_info_list_[type].size() != 0)
	{
		if( !def_device_path_[type].empty() )
		{
			int k = 0;
			for(auto i = device_info_list_[type].begin(); i != device_info_list_[type].end(); i++, k++)
			{
				if(i->device_path_ == def_device_path_[type])
				{
					no = k;
					device_path = def_device_path_[type];
					break;
				}
			}
		}
		if( device_path.empty() )
		{
			no = 0;
			device_path = device_info_list_[type].begin()->device_path_;
			def_device_path_[type] = device_path;
		}
	}

	return !device_path.empty();
}

void VChatManager::SetDefaultDevicePath(std::string device_path, nim::NIMDeviceType type)
{
	if (type == nim::kNIMDeviceTypeAudioOutChat)
	{
		type = nim::kNIMDeviceTypeAudioOut;
	}
	def_device_path_[type] = device_path;
}

std::list<MEDIA_DEVICE_DRIVE_INFO> VChatManager::GetDeviceInfo(nim::NIMDeviceType type)
{
	nim::VChat::EnumDeviceDevpath(type, &VChatCallback::EnumDevCb);
	return device_info_list_[type];
}

//音频设置	
void VChatManager::SetAudioVolumn(unsigned char volumn, bool capture)
{
	nim::VChat::SetAudioVolumn(volumn, capture);
}

//自动调节麦克风音量
void VChatManager::SetAudioInputAutoVolumn(bool auto_volumn)
{
	nim::VChat::SetAudioInputAutoVolumn(auto_volumn);
}

void VChatManager::StartDevice(nim::NIMDeviceType type, std::string device_path, DeviceSessionType session_type)
{
	if (device_path.empty())
	{
		int num_no = 0;
		GetDefaultDevicePath(num_no, device_path, type);
	}
	SetDefaultDevicePath(device_path, type);
	int width = 0;
	int height = 0;
	nim::VChat::StartDevice(type, device_path, 50, width, height, &VChatCallback::StartDeviceCb);
	if (device_session_type_[type] == kDeviceSessionTypeNone)
	{
		nim::VChat::AddDeviceStatusCb(type, &VChatCallback::DeviceStatusCb);
	}
	device_session_type_[type] |= session_type;
}

void VChatManager::EndDevice(nim::NIMDeviceType type, DeviceSessionType session_type, bool forced)
{
	device_session_type_[type] &= ~session_type;
	if (device_session_type_[type] == kDeviceSessionTypeNone || forced)
	{
		nim::VChat::EndDevice(type);
	}
}

//通话
bool VChatManager::StartChat(nim::NIMVideoChatMode mode, const std::string& apns_text, const std::string& custom_info, const std::string& uid, const std::string& session_id)
{
	QLOG_APP(L"StartChat mode={0}, uid={1}") << mode << uid;
	Json::FastWriter fs;
	Json::Value value;
	value[nim::kNIMVChatSessionId] = session_id;
	value[nim::kNIMVChatUids].append(uid);
	value[nim::kNIMVChatSound] = "video_chat_tip_receiver.aac";
	value[nim::kNIMVChatNeedBadge] = 0;
	value[nim::kNIMVChatNeedFromNick] = 0;
	if (1)
	{
		value[nim::kNIMVChatVideoQuality] = 0;
		value[nim::kNIMVChatRecord] = 0;
		value[nim::kNIMVChatVideoRecord] = 0;
	}
	std::string json_value = fs.write(value);
	return nim::VChat::Start(mode, mode == nim::kNIMVideoChatModeAudio ? "audio" : "video", "test custom info", json_value);
}

//设置视频类型
bool VChatManager::SetVoipMode(nim::NIMVideoChatMode mode)
{
	QLOG_APP(L"SetVoipMode mode={0}") << mode;
	return nim::VChat::SetTalkingMode(mode, "");
}

//回应邀请
bool VChatManager::VChatCalleeAck(uint64_t channel_id, bool accept, const std::string& session_id)
{
	QLOG_APP(L"VChatCalleeAck channel_id={0}, accept={1}") << channel_id << accept;
	std::string json_value;
	if (accept)
	{
		Json::FastWriter fs;
		Json::Value value;
		value[nim::kNIMVChatVideoQuality] = 0;
		value[nim::kNIMVChatRecord] = 0;
		value[nim::kNIMVChatVideoRecord] = 0;
		value[nim::kNIMVChatSessionId] = session_id;
		json_value = fs.write(value);
	}
	return nim::VChat::CalleeAck(channel_id, accept, json_value);
}

//通话控制
bool VChatManager::VChatControl(uint64_t channel_id, nim::NIMVChatControlType type)
{
	QLOG_APP(L"VChatControl channel_id={0}, type={1}") << channel_id << type;
	return nim::VChat::Control(channel_id, type);
}

void VChatManager::EndChat(const std::string& session_id)
{
	QLOG_APP(L"EndChat {0}") << session_id;
	Json::FastWriter fs;
	Json::Value value;
	value[nim::kNIMVChatSessionId] = session_id;
	std::string json_value = fs.write(value);
	nim::VChat::End(json_value);
}

void VChatManager::SetViewerMode(bool viewer)
{
	nim::VChat::SetViewerMode(viewer);
}

bool VChatManager::GetViewerMode()
{
	return nim::VChat::GetViewerMode();
}

void VChatManager::SetAudioMuted(bool muted)
{
	nim::VChat::SetAudioMuted(muted);
}

bool VChatManager::GetAudioMuteEnabled()
{
	return nim::VChat::GetAudioMuteEnabled();
}

void VChatManager::SetMemberBlacklist(const std::string& uid, bool add, bool audio, nim::VChat::OptCallback cb)
{
	nim::VChat::SetMemberBlacklist(uid, add, audio, "", cb);
}

void VChatManager::CreateRoom(const std::string& room_name, const std::string& custom_info, nim::VChat::Opt2Callback cb)
{
	//int32_t webrtc = 0;
	std::string json_value;
	/*if (webrtc == 1)
	{
	Json::FastWriter fs;
	Json::Value value;
	value[nim::kNIMVChatWebrtc] = webrtc;
	json_value = fs.write(value);
	}*/
	nim::VChat::CreateRoom(room_name, custom_info, json_value, cb);
}

bool VChatManager::JoinRoom(nim::NIMVideoChatMode mode, const std::string& room_name, const std::string& rtmp_url, bool live_link, const std::string& session_id, nim::VChat::Opt2Callback cb)
{
	Json::FastWriter fs;
	Json::Value value;
	value[nim::kNIMVChatSessionId] = session_id;
	value[nim::kNIMVChatRtmpUrl] = rtmp_url;
	value[nim::kNIMVChatBypassRtmp] = live_link ? 1 : 0;
	value[nim::kNIMVChatRtmpRecord] = 0;
	value[nim::kNIMVChatAudioHighRate] = 1;
	std::string split_mode = "";
	if (split_mode.empty())
	{
		value[nim::kNIMVChatSplitMode] = nim::kNIMVChatSplitLatticeTile;
	}
	else
	{
		value[nim::kNIMVChatSplitMode] = atoi(split_mode.c_str());
	}
	std::string json_value = fs.write(value);
	return nim::VChat::JoinRoom(mode, room_name, json_value, cb);
}

void VChatManager::SetChatRoomCb(ConnectCallback connect_cb /*= nullptr*/, PeopleChangeCallback people_cb /*= nullptr*/, VolumeCallback volume_cb /*= nullptr*/)
{
	chatroom_connect_cb_ = connect_cb;
	chatroom_people_cb_ = people_cb;
	chatroom_volume_cb_ = volume_cb;
}

unsigned char VChatManager::GetAudioVolumn(bool capture)
{
	return nim::VChat::GetAudioVolumn(capture);
}

void VChatManager::SetAudioDataCb(AudioVolumCallback audio_cb)
{
	audio_volume_cb_ = audio_cb;
}

void VChatManager::OnAudioVolume(uint64_t time, const char *data, uint32_t size, const char *json_extension, const void *user_data)
{
	if (audio_volume_cb_)
	{
		int volum = PcmDbCount(data, size);
		audio_volume_cb_(volum);
	}
}

int VChatManager::PcmDbCount(const char* ptr, size_t size)
{
	int ndb = 0;

	unsigned int i;
	long v = 0;
	for (i = 0; i < size; i += 2)
	{
		short int value = 0;
		value |= *(ptr + i);
		value |= ((*(ptr + i + 1)) << 8);
		v += abs(value);
	}

	v = v / (size / 2);

	ndb = log10(1 + v) * 20;

	return ndb;
}

}