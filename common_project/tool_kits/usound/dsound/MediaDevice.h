
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

void GetSystemVolume(float &dVolumeValue) //��ȡ��������ֵ
;

void SetSystemVolume(DWORD dVolumeValue) //������������ֵ
;

void GetMicrophoneValue(DWORD &dMicorphoneValue) //��ȡ��˷�����
;

void SetMicrophoneValue(DWORD dMicorphoneValue) //������˷�����
;

bool GetMicroBoostData(float &fMin, float &fMax, float &fStep) //��ȡ��˷���ǿ�ȼ�����Сֵ�����ֵ������
;

bool GetMicroBoostLevel(float &dMicroBoostLevel) //��ȡ��˷���ǿ�ȼ�
;
void SetMicroBoostLevel(float dMicroBoostLevel) //������˷���ǿ�ȼ�
;