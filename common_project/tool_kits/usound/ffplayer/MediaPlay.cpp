#include "MediaPlay.h"
#include <string>
#include "base\win32\platform_string_util.h"
#include "base\file\file_util.h"


FFMediaPlay::FFMediaPlay()
	:m_hplayer(NULL),
	m_hWnd(NULL),
	m_bPlaying(false),
	m_nRenderType(VDEV_RENDER_TYPE_GDI),
	m_nAudioVolume(0),
	m_llDuration(0),
	m_nVideoWidth(0),
	m_nVideoHeight(0),
	m_nIndex(0),
	m_rect({0,0,0,0})
{
}


FFMediaPlay::~FFMediaPlay()
{
}

void FFMediaPlay::SetHwnd(HWND hWnd)
{
	m_hWnd = hWnd;
}

void FFMediaPlay::SetDisplayArea(RECT rect)
{
	m_rect = rect;
	if (m_hplayer)
	{
		player_setrect(m_hplayer, 0, m_rect.left, m_rect.top, m_rect.right - m_rect.left, m_rect.bottom - m_rect.top);
		player_setrect(m_hplayer, 1, m_rect.left, m_rect.top, m_rect.right - m_rect.left, m_rect.bottom - m_rect.top);
	}
}

bool FFMediaPlay::Play(std::wstring strFileName)
{
	if(!::IsWindow(m_hWnd))
	{
		return false;
	}
	Stop();

	std::string file_path = "";
	nbase::win32::UnicodeToMBCS(strFileName, file_path);
	m_hplayer = player_open(const_cast<char*>(file_path.c_str()), m_hWnd);
	player_getparam(m_hplayer, PARAM_VIDEO_WIDTH, &m_nVideoWidth);
	player_getparam(m_hplayer, PARAM_VIDEO_HEIGHT, &m_nVideoHeight);
	player_getparam(m_hplayer, PARAM_MEDIA_DURATION, &m_llDuration);
	player_setparam(m_hplayer, PARAM_VDEV_RENDER_TYPE, &m_nRenderType);
	player_setparam(m_hplayer, PARAM_AUDIO_VOLUME, &m_nAudioVolume);
	player_play(m_hplayer);
	
	m_bPlaying = true;

	return true;
}

bool FFMediaPlay::Play(int nIndex)
{
	if(nIndex >= m_vecFileNames.size() || nIndex < 0)
	{
		return false;
	}

	m_nIndex = nIndex;
	return Play(m_vecFileNames[nIndex]);
}

void FFMediaPlay::Stop()
{
	if (m_hplayer)
	{
		player_close(m_hplayer);
		m_hplayer = NULL;
	}
	m_bPlaying = false;
	m_llDuration = 0;
	m_nVideoWidth = 0;
	m_nVideoHeight = 0;
}

void FFMediaPlay::Pause()
{
	if (m_hplayer && m_bPlaying)
	{
		player_pause(m_hplayer);
		m_bPlaying = false;
	}
}

void FFMediaPlay::Play()
{
	if (m_hplayer)
	{
		// continue to play
		player_play(m_hplayer);
		m_bPlaying = true;
	}
}

bool FFMediaPlay::Next()
{
	return Play(m_nIndex + 1);
}

bool FFMediaPlay::Previous()
{
	return Play(m_nIndex - 1);
}

void FFMediaPlay::Seek(LONGLONG ms)
{
	if (m_hplayer)
	{
		ms = ms > m_llDuration ? m_llDuration : ms;
		if (ms < 0) {
			ms = 0;
		}
		player_seek(m_hplayer, ms);
	}
}

bool FFMediaPlay::IsPlaying()
{
	return m_bPlaying;
}

bool FFMediaPlay::IsOpenFile()
{
	return m_hplayer ? true : false;
}

std::wstring FFMediaPlay::GetPlayingFileName()
{
	std::wstring file_name;
	if (m_nIndex >= 0 && m_nIndex < m_vecFileNames.size())
	{
		std::wstring file_path = m_vecFileNames[m_nIndex];
		nbase::FilePathApartFileName(file_path, file_name);
	}
	return file_name;
}

int FFMediaPlay::GetPlayListCount()
{
	return m_vecFileNames.size();
}

int FFMediaPlay::GetCurIndex()
{
	return m_nIndex;
}

void FFMediaPlay::AddToPlayList(std::vector<std::wstring>& vecFileNames)
{
	m_vecFileNames.insert(m_vecFileNames.end(), vecFileNames.begin(), vecFileNames.end());
}

void FFMediaPlay::SetRenderType(int nRenderType)
{
	m_nRenderType = nRenderType;
	if (m_hplayer)
	{
		player_setparam(m_hplayer, PARAM_VDEV_RENDER_TYPE, &m_nRenderType);
	}
}

void FFMediaPlay::SetAudioVolume(int nAudioVolume)
{
	m_nAudioVolume = nAudioVolume;
	if (m_hplayer)
	{
		player_setparam(m_hplayer, PARAM_AUDIO_VOLUME, &m_nAudioVolume);
	}
}

int FFMediaPlay::GetAudioVolume()
{
	return m_nAudioVolume;
}

LONGLONG FFMediaPlay::GetVideoDuration()
{
	return m_llDuration;
}

LONGLONG FFMediaPlay::GetCurPosition()
{
	LONGLONG position = 0;
	if (m_hplayer)
		player_getparam(m_hplayer, PARAM_MEDIA_POSITION, &position);
	return position;
}

int FFMediaPlay::GetVideoWidth()
{
	return m_nVideoWidth;
}

int FFMediaPlay::GetVideoHeight()
{
	return m_nVideoHeight;
}

