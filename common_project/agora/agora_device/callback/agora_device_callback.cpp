#include "agora_device_callback.h"
#include "manager\agora_device_cb_manager.h"

void AGDeviceCallback::OnAudioVolumeIndication(int totalVolume)
{
	AgDeviceCbManager::GetInstance()->OnAudioVolumeIndication(totalVolume);
}

