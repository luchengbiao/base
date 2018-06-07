#pragma once
#include <vector>
#include <string>
#include <wtypes.h>
#include "ffplay\ffplayer\ffplayer.h"

class FFMediaPlay
{
public:
	FFMediaPlay();
	~FFMediaPlay();
	// ������ʾ��Ƶ�Ĵ��ھ��
	void SetHwnd(HWND hWnd);
	// ������Ʒ��ʾ����
	void SetDisplayArea(RECT rect);    //play֮��������Ч
	// ����
	bool Play(std::wstring strFileName);
	bool Play(int nIndex);
	// ֹͣ
	void Stop();
	// ��ͣ
	void Pause();
	// ��������
	void Play();
	// ������һ���ļ�
	bool Next();
	// ������һ���ļ�
	bool Previous();
	// seek
	void Seek(LONGLONG ms);
	// �Ƿ����ڲ���
	bool IsPlaying();
	// �Ƿ�����ļ�
	bool IsOpenFile();
	// ��ȡ���ڲ��ŵ��ļ���
	std::wstring GetPlayingFileName();
	// ��ȡ�����б����ļ����� �ظ�Ҳ��
	int GetPlayListCount();
	// ��ȡ�����б��е�ǰ����
	int GetCurIndex();
	// ����ļ��������б�
	void AddToPlayList(std::vector<std::wstring> &vecFileNames);
	// ������Ƶ��Ⱦģʽ
	void SetRenderType(int nRenderType = VDEV_RENDER_TYPE_D3D);
	// ��������
	//�������ò�����������ͬ��ϵͳ������ffplayer �ڲ�����һ�� - 30dB �� + 12dB ������������Ƶ�Ԫ
	//������Χ��[-182, 73]�� - 182 ��Ӧ - 30dB��73 ��Ӧ + 12dB
	//����ֵ  ��0 ��Ӧ 0dB ���棬 - 255 ��Ӧ������ + 255 ��Ӧ�������
	void SetAudioVolume(int nAudioVolume = 0);
	// ��ȡ����ֵ
	int GetAudioVolume();
	// �����Ƶ����
	LONGLONG GetVideoDuration();
	// ��õ�ǰ���Ž���
	LONGLONG GetCurPosition();
	// ��ȡ��Ƶ��
	int GetVideoWidth();
	// ��ȡ��Ƶ��
	int GetVideoHeight();

private:
	// �����б�
	std::vector<std::wstring> m_vecFileNames;
	int m_nIndex;
	void *m_hplayer;
	HWND m_hWnd;
	bool m_bPlaying;
	RECT m_rect;
	int m_nRenderType;
	int m_nAudioVolume;
	LONGLONG m_llDuration;
	int m_nVideoWidth;
	int m_nVideoHeight;
};

