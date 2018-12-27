#ifndef __GAME_MODEL_CHESS_ACTIONS_CHESS_GAME_ACTION_TYPES_H__
#define __GAME_MODEL_CHESS_ACTIONS_CHESS_GAME_ACTION_TYPES_H__
#include "game/macros.h"

GAME_NAMESPACE_BEGIN

enum class ChessGameActionType
{
	MoveIntoBoard,
	MoveInsideOfBoard,
	MoveOutOfBoard,
	NewBoard,
	NewBoardEx,
	ClearBoard,
	BackForward,
	SwitchToHistoryBoard,
	ImportChessManual,
};

GAME_NAMESPACE_END

#endif