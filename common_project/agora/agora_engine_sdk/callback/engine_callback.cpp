#include "engine_callback.h"
#include <string>
#include "qthread_manager\closure.h"
#include "..\agora_vchat\callback\agora_vchat_callback.h"
#include "..\agora_device\callback\agora_device_callback.h"

AGEngineEventHandler::AGEngineEventHandler(void)
{

}

AGEngineEventHandler::~AGEngineEventHandler(void)
{

}

void AGEngineEventHandler::onJoinChannelSuccess(const char* channel, uid_t uid, int elapsed)
{
	std::string s_channel_id = std::string(channel);
	auto task = [=]()
	{
		AGVchatCallback::OnJoinChannelSuccess(s_channel_id, uid, elapsed);
	};
	qtbase::Post2Task(kThreadUIHelper, task);
}

void AGEngineEventHandler::onRejoinChannelSuccess(const char* channel, uid_t uid, int elapsed)
{
	std::string s_channel_id = std::string(channel);
	auto task = [=]()
	{
		AGVchatCallback::OnRejoinChannelSuccess(s_channel_id, uid, elapsed);
	};
	qtbase::Post2Task(kThreadUIHelper, task);
}

void AGEngineEventHandler::onWarning(int warn, const char* msg)
{
	
}

void AGEngineEventHandler::onError(int err, const char* msg)
{
	std::string msgStr = "";
	if (msg)
	{
		msgStr = std::string(msg);
	}
	auto task = [=]()
	{
		AGVchatCallback::OnError(err, msgStr);
	};
	qtbase::Post2Task(kThreadUIHelper, task);
	

}

void AGEngineEventHandler::onAudioQuality(uid_t uid, int quality, unsigned short delay, unsigned short lost)
{
	auto task = [=]()
	{
		AGVchatCallback::onAudioQuality(uid, quality, delay, lost);
	};
	qtbase::Post2Task(kThreadUIHelper, task);
}

void AGEngineEventHandler::onAudioVolumeIndication(const AudioVolumeInfo* speakers, unsigned int speakerNumber, int totalVolume)
{
	auto task = [=]()
	{
		AGDeviceCallback::OnAudioVolumeIndication(totalVolume);
	};
	qtbase::Post2Task(kThreadUIHelper, task);
}

void AGEngineEventHandler::onLeaveChannel(const RtcStats& stat)
{
	auto task = [=]()
	{
		AGVchatCallback::OnLeaveChannel();
	};
	qtbase::Post2Task(kThreadUIHelper, task);
}

void AGEngineEventHandler::onRtcStats(const RtcStats& stat)
{

}

void AGEngineEventHandler::onMediaEngineEvent(int evt)
{
}

void AGEngineEventHandler::onAudioDeviceStateChanged(const char* deviceId, int deviceType, int deviceState)
{

}

void AGEngineEventHandler::onVideoDeviceStateChanged(const char* deviceId, int deviceType, int deviceState)
{

}

void AGEngineEventHandler::onLastmileQuality(int quality)
{


}

void AGEngineEventHandler::onFirstLocalVideoFrame(int width, int height, int elapsed)
{

}

void AGEngineEventHandler::onFirstRemoteVideoDecoded(uid_t uid, int width, int height, int elapsed)
{

}

void AGEngineEventHandler::onFirstRemoteVideoFrame(uid_t uid, int width, int height, int elapsed)
{

}

void AGEngineEventHandler::onUserJoined(uid_t uid, int elapsed)
{
	auto task = [=]()
	{

	};
	qtbase::Post2Task(kThreadUIHelper, task);
}

void AGEngineEventHandler::onUserOffline(uid_t uid, USER_OFFLINE_REASON_TYPE reason)
{
	if (reason != USER_OFFLINE_QUIT)
	{
		auto task = [=]()
		{
			AGVchatCallback::OnUserOffline(uid);
		};
		qtbase::Post2Task(kThreadUIHelper, task);
	}
}

void AGEngineEventHandler::onUserMuteAudio(uid_t uid, bool muted)
{

}

void AGEngineEventHandler::onUserMuteVideo(uid_t uid, bool muted)
{

}

void AGEngineEventHandler::onStreamMessage(uid_t uid, int streamId, const char* data, size_t length)
{

}

void AGEngineEventHandler::onApiCallExecuted(const char* api, int error)
{
	
}

void AGEngineEventHandler::onLocalVideoStats(const LocalVideoStats& stats)
{

}

void AGEngineEventHandler::onRemoteVideoStats(const RemoteVideoStats& stats)
{

}

void AGEngineEventHandler::onCameraReady()
{

}

void AGEngineEventHandler::onVideoStopped()
{

}

void AGEngineEventHandler::onConnectionLost()
{
	auto task = [=]()
	{
		AGVchatCallback::OnConnectionLost();
	};
	qtbase::Post2Task(kThreadUIHelper, task);
}

void AGEngineEventHandler::onConnectionInterrupted()
{
	
}

void AGEngineEventHandler::onNetworkQuality(uid_t uid, int txQuality, int rxQuality)
{
	auto task = [=]()
	{
		AGVchatCallback::onNetworkQuality(uid, txQuality, rxQuality);
	};
	qtbase::Post2Task(kThreadUIHelper, task);
}

void AGEngineEventHandler::onUserEnableVideo(uid_t uid, bool enabled)
{

}

void AGEngineEventHandler::onStartRecordingService(int error)
{

}

void AGEngineEventHandler::onStopRecordingService(int error)
{

}

void AGEngineEventHandler::onRefreshRecordingServiceStatus(int status)
{

}