#include "audio_callback.h"
#include <assert.h>
#include <string>
#include "log\log.h"
#include "qthread_manager\closure.h"
#include "nim_sdk_manager\api\nim_audio_cpp.h"
#include "manager\audio_manager.h"

namespace nim_comp
{
void UIPlayAudioCallback( int code, std::string sid, std::string cid )
{
	assert( !sid.empty() );
	assert( !cid.empty() );
	if (sid.empty() || cid.empty()) {
		return;
	}

	if (code != nim_audio::kSuccess)
	{
		AudioManager::GetInstance()->ClearPlayId();
	}

	
}

void AudioCallback::OnPlayAudioCallback( int code, const char* file_path, const char* sid, const char* cid )
{
	LOG_MSG(L"OnPlayAudioCallback: sid={0} cid={1} code={2}") << sid << cid << code;
	qtbase::Post2Task(kThreadUIHelper, nbase::Bind(&UIPlayAudioCallback, code, std::string(sid), std::string(cid)));
}

void UIStopAudioCallback( int code, const std::string &sid, const std::string &cid )
{
	if(sid.empty() || cid.empty())
		return;

	//如果这个停止的callback是当前正在播放的
	if (sid == AudioManager::GetInstance()->GetPlaySid() && cid == AudioManager::GetInstance()->GetPlayCid())
	{
		AudioManager::GetInstance()->ClearPlayId();
	}

}

void AudioCallback::OnStopAudioCallback( int code, const char* file_path, const char* sid, const char* cid )
{
	LOG_MSG(L"OnStopAudioCallback: sid={0} cid={1} code={2}") <<sid <<cid <<code;
	qtbase::Post2Task(kThreadUIHelper, nbase::Bind(&UIStopAudioCallback, code, std::string(sid), std::string(cid)));
}

void AudioCallback::OnStartCaptureCallback(int code)
{
	LOG_MSG(L"OnStartCaptureCallback: code={0}") << code;
	qtbase::Post2Task(kThreadUIHelper, nbase::Bind(&AudioManager::OnStartCaptureCallback, AudioManager::GetInstance(), code));
}

void AudioCallback::OnStopCaptureCallback(int rescode, const char* sid, const char* cid, 
											const char* file_path, const char* file_ext, long file_size, int audio_duration)
{
	LOG_MSG(L"OnStopCaptureCallback: sid={0} cid={1} code={2} file={3}") << sid << cid << rescode << file_path;
	qtbase::Post2Task(kThreadUIHelper, nbase::Bind(&AudioManager::OnStopCaptureCallback, AudioManager::GetInstance(), 
		rescode, std::string(sid), std::string(cid), std::string(file_path), std::string(file_ext), file_size, audio_duration));
}

void AudioCallback::OnCancelCaptureCallback(int code)
{
	LOG_MSG(L"OnCancelCaptureCallback: code={0}") << code;
	qtbase::Post2Task(kThreadUIHelper, nbase::Bind(&AudioManager::OnCancelCaptureCallback, AudioManager::GetInstance(), code));
}

void AudioCallback::OnEnumCaptureDeviceCallback(int rescode, const wchar_t* device_list)
{
	LOG_INFO(L"OnEnumCaptureDeviceCallback: code={0}") << rescode;
	qtbase::Post2Task(kThreadUIHelper, nbase::Bind(&AudioManager::OnEnumCaptureDeviceCallback, AudioManager::GetInstance(), rescode, device_list));
}

}