#ifndef __GAME_MODEL_COMMAND_HANDLER_COMMAND_CONFLICT_LEVEL_H__
#define __GAME_MODEL_COMMAND_HANDLER_COMMAND_CONFLICT_LEVEL_H__
#include "game/macros.h"
#include "protocol/command/command_types.h"

GAME_NAMESPACE_BEGIN

enum class CmdConflictLevel
{
	None										= 0,

	NoConflict_NoLocalCmdCached					= 1,

	NoConflict_SameWithTheFirstLocalCmdCached	= 2,

	// commands conflict among: TeacherMoveChess, StudentMoveChess, ClearGameBoard, TeacherBackwardOrForward, TeacherAutoTakingPieces... 
	// excluding SwitchedToAnotherGame
	InsideOfCurrentGame							= 3, 

	// TeacherChangeGameBoard, TeacherNewGameBoard, TeacherNewGameBoardEx, TeacherNewGameBoardEx, TeacherSwitchToHistoryBoard, TeacherImportChessManual
	SwitchedToAnotherGame						= 4, 
};

inline CmdConflictLevel ConflictLevelOfCmdType(const proto::CommandType cmd_type, const CmdConflictLevel default_level)
{
	CmdConflictLevel level = default_level;

	switch (cmd_type)
	{
	case proto::CommandType::TeacherChangeGameBoard:
	case proto::CommandType::TeacherNewGameBoard:
	case proto::CommandType::TeacherNewGameBoardEx:
	case proto::CommandType::TeacherSwitchToHistoryBoard:
	case proto::CommandType::TeacherImportChessManual:
		level = CmdConflictLevel::SwitchedToAnotherGame;
		break;

	case proto::CommandType::TeacherMoveChess:
	case proto::CommandType::StudentMoveChess:
	case proto::CommandType::ClearGameBoard:
	case proto::CommandType::TeacherBackForward:
	case proto::CommandType::TeacherUpdateAutoTakingPieces:
		level = CmdConflictLevel::InsideOfCurrentGame;
		break;

	default:
		break;
	}

	return level;
}

GAME_NAMESPACE_END

#endif