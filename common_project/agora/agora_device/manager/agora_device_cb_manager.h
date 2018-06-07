#pragma once
#include <map>
#include <memory>
#include <functional>
#include "base\callback\callback.h"
#include "base\util\auto_unregister.h"
#include "base\memory\singleton.h"

typedef std::function<void(int volume)>	AgoraAudioVolumeCallback;	/**< 声网麦克风的音量回调 */

class AgDeviceCbManager : public nbase::SupportWeakCallback
{
public:
	SINGLETON_DEFINE(AgDeviceCbManager);
	AgDeviceCbManager();
	~AgDeviceCbManager();

	// reg cb
	UnregisterCallback RegAudioVolumeCb(const AgoraAudioVolumeCallback& callback);

	// cb
	void OnAudioVolumeIndication(int totalVolume);

private:
	std::map<int, std::unique_ptr<AgoraAudioVolumeCallback>> audio_volume_cb_list_;
};
