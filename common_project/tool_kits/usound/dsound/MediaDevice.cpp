#include <atlbase.h>
#include <WTypes.h>
#include <shellapi.h>
#include "MixerWrap.h"
#include "VolController.h"
#include "CMixer.h"
#include "MediaDevice.h"

int GetOSVer()
{
	OSVERSIONINFO   osver;
	osver.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osver);
	if (osver.dwPlatformId == 2)
	{
		if (osver.dwMajorVersion == 5 && osver.dwMinorVersion == 1)   //xp
		{
			return WIN_XP;
		}
		if (osver.dwMajorVersion == 5 && osver.dwMinorVersion == 2)  //2003
		{
			return WIN_2003;
		}
		if (osver.dwMajorVersion == 6 && (osver.dwMinorVersion == 0 || osver.dwMinorVersion == 1))  //vista or 2008
		{
			return WIN_VISTA;
		}
		if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 1)  //win7
		{
			return WIN_7;
		}
		if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 2)  //win8
		{
			return WIN_8;
		}
	}
	return 0;
}

void InitMediaDevice()
{
	if (1 || GetOSVer() == WIN_XP)
	{
		CVolController::InitializeMixer();
	}
}

void GetSystemVolume(float &dVolumeValue) //获取主音量的值
{
	if (GetOSVer() == WIN_XP)
	{
		LONG max = CVolController::GetVolumeRangMax();
		LONG lValue = CVolController::GetVolumeValue(MainChannel);
		dVolumeValue = lValue * 100 / max;
	}
	else if (GetOSVer() == WIN_7 || GetOSVer() == WIN_8 || GetOSVer() == WIN_VISTA)
	{
		MixerWrap::GetSpeakerValue_W7(dVolumeValue);
	}

}

void SetSystemVolume(DWORD dVolumeValue) //设置主音量的值
{
	if (GetOSVer() == WIN_XP)
	{
		LONG min = CVolController::GetVolumeRangMin();
		LONG max = CVolController::GetVolumeRangMax();
		CVolController::SetVolumeValue((max - min)*dVolumeValue / 100);
	}
	else if (GetOSVer() == WIN_7 || GetOSVer() == WIN_8 || GetOSVer() == WIN_VISTA)
	{
		MixerWrap::SetSpeakerValue_W7(dVolumeValue);
	}
}

void GetMicrophoneValue(DWORD &dMicorphoneValue) //获取麦克风音量
{
	if (GetOSVer() == WIN_XP)
	{
		CVolController::GetMicrPhoneVolume(dMicorphoneValue);
	}
	else if (GetOSVer() == WIN_7 || GetOSVer() == WIN_8 || GetOSVer() == WIN_VISTA)
	{
		MixerWrap::GetMicphone_W7(dMicorphoneValue);
	}
}

void SetMicrophoneValue(DWORD dMicorphoneValue) //设置麦克风音量
{
	if (GetOSVer() == WIN_XP)
	{
		CVolController::SetMicrPhoneVolume(dMicorphoneValue);
	}
	else if (GetOSVer() == WIN_7 || GetOSVer() == WIN_8 || GetOSVer() == WIN_VISTA)
	{
		MixerWrap::MicphoneMut_W7(false, dMicorphoneValue);
	}
}

bool GetMicroBoostData(float &fMin, float &fMax, float &fStep) //获取麦克风增强等级的最小值，最大值和增量
{
	if (GetOSVer() == WIN_7 || GetOSVer() == WIN_8 || GetOSVer() == WIN_VISTA)
	{
		MixerWrap::GetMicphoneLevelData(fMin, fMax, fStep);
		return true;
	}
	return false;
}

bool GetMicroBoostLevel(float &dMicroBoostLevel) //获取麦克风增强等级
{
	//return CVolController::GetMicrophoneLevel();
	if (GetOSVer() == WIN_7 || GetOSVer() == WIN_8 || GetOSVer() == WIN_VISTA)
	{
		MixerWrap::GetMicphoneLevel(dMicroBoostLevel);
		return true;
	}
	return false;
}

void SetMicroBoostLevel(float dMicroBoostLevel) //设置麦克风增强等级
{
	if (GetOSVer() == WIN_7 || GetOSVer() == WIN_8 || GetOSVer() == WIN_VISTA)
	{
		MixerWrap::MicphoneBoost_W7(dMicroBoostLevel);
	}
}