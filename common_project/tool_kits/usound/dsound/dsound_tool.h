#pragma once
#include <string>
#include <vector>
#include <atlbase.h>
#include <WTypes.h>
#include <shellapi.h>
#include "base/memory/singleton.h"
#include <mmsystem.h>
#include <dsound.h> 

#define SAFE_RELEASE(p)			{ if (NULL != p) { (p)->Release(); (p) = NULL; } }

struct DsoundDevice
{
	LPGUID guid_;
	std::wstring	device_name_;
};

class DSoundTool
{
public:
	SINGLETON_DEFINE(DSoundTool);
	DSoundTool(){}
	~DSoundTool()
	{
		SAFE_RELEASE(m_pDSBuffer8);
		SAFE_RELEASE(m_pDS);
	}

public:
	void InitSpeaker();
	//bool EnumDevices(vector<wstring>& vec, bool b_capture = true);
	std::vector<DsoundDevice>& GetSpeakers();
	void PrintMicProps(HWND mhwnd);

	void SetSystemAudio();	//设置系统声音： 主音量、麦克风音量、麦克风增强

	bool StartRecord();
	bool StopPlaying();
	bool PlayWav(HWND hwnd, std::wstring device_name_);
	bool PlayPCM2(HWND hwnd, const void* pcm_data, int data_len);
	void SetSpeakerVolume(double value);
private:
	static BOOL CALLBACK enum_dev_proc(LPGUID lpGUID, LPCTSTR lpszDesc,
		LPCTSTR lpszDrvName, LPVOID lpContext);
	//枚举设备  b_capture表示麦克风
	BOOL EnumDevices(std::vector<DsoundDevice>& vec, bool b_capture = true);
	DsoundDevice* GetSpeakerByName(std::wstring device_name);

	

	std::vector<DsoundDevice> vec_device;
	IDirectSound8 *m_pDS;
	IDirectSoundBuffer8 *m_pDSBuffer8; //used to manage sound buffers. 
};