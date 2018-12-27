#ifndef __PROTO_COMMAND_COMMAND_TYPES_H__
#define __PROTO_COMMAND_COMMAND_TYPES_H__
#include "protocol/macros.h"

PROTO_NAMESPACE_BEGIN

enum class CommandType
{
	None								= 0,

	TeacherDrawStart					= 1,
	TeacherDrawMove						= 2,
	TeacherDrawEnd						= 3,
	TeacherChangeColor					= 4,
	TeacherChangeBoardPager				= 5,
	UploadImg							= 6,
	TeacherStartCourse					= 7,
	EndCourse							= 8,
	TeacherChangePrice					= 9,
	StudentChargeBalance				= 10,
	TeacherChangePhotoPage				= 11,
	StudentRecvCourseStart				= 12,
	ExchangeData						= 13,
	TeacherChangeToPpt					= 14,
	StudentRevoke						= 15,
	TeacherRevoke						= 16,
	StudentClean						= 17,
	TeacherClean						= 18,
	AudioStart							= 19,
	TimesSign							= 20,
	TeacherUploadPptFiles				= 21,
	StudentChangeBoardPager				= 22,
	StudentChangePhotoPager				= 23,
	StudentChangePptPager				= 24,
	StudentDrawStart					= 25,
	StudentDrawMove						= 26,
	StudentDrawEnd						= 27,
	StudentAudioStart					= 28,
	TeacherResetPpt						= 29,
	StartObserve						= 30,
	ObserveData							= 31,
	ExchangeUserInfo					= 32,
	TeacherClassUploadPptFiles			= 33,
	TeacherClassChangePptPager			= 34,
	TeacherClassCleanPpt				= 35,
	TeacherClassBeginUpload				= 36,
	TeacherClassEndUpload				= 37,
	TeacherChangeGameBoard				= 40,
	TeacherForbiddenStudentOper			= 41,
	TeacherShowBoardRule				= 42,
	TeacherEraserDrawDown				= 43,
	TeacherEraserDrawMove				= 44,
	TeacherEraserDrawUp					= 45,
	TeacherMoveChess					= 46,
	StudentChangeGameBoard				= 50,
	StudentMoveChess					= 51,
	TeacherNewGameBoard					= 52,
	TeacherMarkManualChess				= 53,
	TeacherCheckManualChess				= 54,
	ReplayAsk							= 55,
	ReplayReceive						= 56,
	CheckHeartbeatFromPeer				= 57,
	ClearGameBoard						= 59,
	
	TeacherStartSyncCurrentChessBoard	= 60,
	TeacherSyncCurrentChessBoard		= 61,
	StudentHangsupSpeak					= 62,
	TeacherAllowForbidStuSpeak			= 63,
	TeacherAddCancelStuChess			= 64,
	TeacherStartPauseStuChess			= 65,
	TeacherExchangeStuChess				= 66,
	TeacherNewGameBoardEx				= 67,
	TeacherBackForward					= 70,
	TeacherImportChessManual			= 71,
	TeacherUpdateMarkOnBoard			= 72,
	TeacherUpdateAutoTakingPieces		= 74,
	TeacherSwitchToHistoryBoard			= 75,

	MaxCountFlag						= 128,

	// user-defined
	CommandCell							= 500,
	CommandGroup						= 501,
	CmdGroupWithTimestamp				= 502,

	StudentDrawGroup					= 503,
	TeacherDrawGroup					= 504,
	TeacherEraserGroup					= 505,
	
	User0								= 1000,
	User1								= 2000,
	User2								= 3000,
	User3								= 4000,
	User4								= 5000,
	User5								= 6000,
	User6								= 7000,
	User7								= 8000,
	User8								= 9000,
	User9								= 10000,
};

PROTO_NAMESPACE_END

#endif