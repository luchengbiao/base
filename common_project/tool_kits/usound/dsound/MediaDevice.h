
#include "MixerWrap.h"
#include "VolController.h"
#include "CMixer.h"

#define  WIN_XP    2
#define  WIN_2003  3
#define  WIN_VISTA 4
#define  WIN_7     5
#define  WIN_2008  6
#define  WIN_8     8
int GetOSVer();

void InitMediaDevice()
;

void GetSystemVolume(float &dVolumeValue) //获取主音量的值
;

void SetSystemVolume(DWORD dVolumeValue) //设置主音量的值
;

void GetMicrophoneValue(DWORD &dMicorphoneValue) //获取麦克风音量
;

void SetMicrophoneValue(DWORD dMicorphoneValue) //设置麦克风音量
;

bool GetMicroBoostData(float &fMin, float &fMax, float &fStep) //获取麦克风增强等级的最小值，最大值和增量
;

bool GetMicroBoostLevel(float &dMicroBoostLevel) //获取麦克风增强等级
;
void SetMicroBoostLevel(float dMicroBoostLevel) //设置麦克风增强等级
;