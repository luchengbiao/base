#include "agora_device_cb_manager.h"

AgDeviceCbManager::AgDeviceCbManager()
{

}

AgDeviceCbManager::~AgDeviceCbManager()
{

}

UnregisterCallback AgDeviceCbManager::RegAudioVolumeCb(const AgoraAudioVolumeCallback& callback)
{
	AgoraAudioVolumeCallback* new_callback = new AgoraAudioVolumeCallback(callback);
	int cb_id = (int)new_callback;
	audio_volume_cb_list_[cb_id].reset(new_callback);

	auto cb = ToWeakCallback([this, cb_id]() {
		audio_volume_cb_list_.erase(cb_id);
	});
	return cb;
}

void AgDeviceCbManager::OnAudioVolumeIndication(int totalVolume)
{
	for (auto& it : audio_volume_cb_list_)
	{
		(*(it.second))(totalVolume);
	}
}
