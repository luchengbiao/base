﻿#include "ring.h"
#include <assert.h>
#include "log\log.h"
#include "nim_base\util.h"
#include "..\common\tool\common_tool.h"

static const std::wstring kNewMessage		= L"new_message.wav";
static const std::wstring kVoipConnecting	= L"voip_connecting.wav";
static const std::wstring kVoipNoResponse	= L"voip_no_response.wav";
static const std::wstring kVoipPeerBusy		= L"voip_peer_busy.wav";
static const std::wstring kVoipPeerReject	= L"voip_peer_reject.wav";
static const std::wstring kVoipRing			= L"voip_ring.wav";
static const std::wstring kCourseOver		= L"course_over.wav";

static const std::wstring kChessEat		= L"chess_eat.wav";
static const std::wstring kChessMove	= L"chess_move.wav";
static const std::wstring kChessNewDown = L"chess_new_down.wav";
static const std::wstring kGoMove		= L"go_move.wav";
static const std::wstring kGoDeadStone  = L"go_dead_stone.wav";

#pragma comment(lib, "VFW32.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib, "dsound.lib")
#pragma comment(lib,"dxguid.lib")

namespace nim_comp
{
	Ring::Ring()
	{
		hwnd_ = NULL;
	}

	Ring::~Ring()
	{
		Destroy();
	}

	bool Ring::Init(HWND hwnd)
	{
		hwnd_ = MCIWndCreateW(hwnd, NULL, WS_POPUP | MCIWNDF_NOERRORDLG | MCIWNDF_NOTIFYMODE, NULL);
		if (hwnd_ == NULL)
		{
			LOG_ERR(L"MCIWndCreateW error {0}") << GetLastError();
			return false;
		}
		return true;
	}

	void Ring::Play(RingType type, bool replay)
	{
		assert(hwnd_);
		if (hwnd_ == NULL)
			return;

		Stop();
		replay_ = replay;

		std::wstring dir = commontool::GetAppPath().append(L"res\\audio\\");

		switch (type)
		{
		case RING_NEW_MESSAGE:
			dir.append(kNewMessage);
			break;
		case RING_VOIP_CONNTETING:
			dir.append(kVoipConnecting);
			break;
		case RING_VOIP_NORESPONSE:
			dir.append(kVoipNoResponse);
			break;
		case RING_VOIP_PEERBUSY:
			dir.append(kVoipPeerBusy);
			break;
		case RING_VOIP_PEERREJECT:
			dir.append(kVoipPeerReject);
			break;
		case RING_VOIP_RING:
			dir.append(kVoipRing);
			break;
		case RING_COURSE_OVER:
			dir.append(kCourseOver);
			break;
		case RING_CHESS_EAT:
			dir.append(kChessEat);
			break;
		case RING_CHESS_MOVE:
			dir.append(kChessMove);
			break;
		case RING_CHESS_NEW_DOWN:
			dir.append(kChessNewDown);
			break;
		case RING_GO_MOVE:
			dir.append(kGoMove);
			break;
		case RING_GO_DEAD_STONE:
			dir.append(kGoDeadStone);
			break;
		}

		DWORD ret = MCIWndOpen(hwnd_, dir.c_str(), 0);
		if (ret == 0)
		{
			MCIWndPlay(hwnd_);
		}
		else
		{
			LOG_ERR(L"MCIWndOpen error {0}") << ret;
		}
	}

	void Ring::Replay()
	{
		if (hwnd_ != NULL && MCIWndCanPlay(hwnd_))
		{
			MCIWndSeek(hwnd_, 0);
			MCIWndPlay(hwnd_);
		}
	}


	void Ring::Stop()
	{
		replay_ = false;
		if (hwnd_)
		{
			MCIWndStop(hwnd_);
			MCIWndClose(hwnd_);
		}
	}

	void Ring::Destroy()
	{
		if (hwnd_)
		{
			Stop();
			MCIWndDestroy(hwnd_);
		}
	}

	LRESULT Ring::Notify(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (uMsg == MCIWNDM_NOTIFYMODE)
		{
			if (lParam == MCI_MODE_STOP && replay_)
			{
				Replay();
			}
		}
		return 0;
	}
}