#ifndef __GAME_MODEL_CHESS_ACTIONS_CHESS_ACTION_ERROR_H__
#define __GAME_MODEL_CHESS_ACTIONS_CHESS_ACTION_ERROR_H__
#include "game/macros.h"

GAME_NAMESPACE_BEGIN

namespace chess_actoin_error
{
	extern const char*  ParamErrorOnFromGrid;
	extern const char*  ParamErrorOnToGrid;
	extern const char*  ExistingChessOnToGrid;
	extern const char*  NotExistingChessOnFromGrid;
	extern const char*  CannotTakeChessOfOwnColorOnToGridWithoutRule;
	extern const char*  ChessIdMismatchingOnFromGrid;
	extern const char*  ChessIdMismatchingOnFromGridAndNotPromotionNeither;
	extern const char*  IllegalMoveWithRule;
	extern const char*  CannotMovePieceIntoBoardActivelyWithRule;
	extern const char*  CannotMovePieceOutofBoardActivelyWithRule;
	extern const char*  CannotClearBoardWithRule;
	extern const char*  ParamErrorInEntityOfNewBoardEx;
	extern const char*  CmdGroupIsNullOrEmtpyInManualImported;
	extern const char*  NotExistingNewBoardCmdInManualImported;
}

GAME_NAMESPACE_END

#endif