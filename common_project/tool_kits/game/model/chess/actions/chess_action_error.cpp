#include "game/macros.h"

GAME_NAMESPACE_BEGIN

namespace chess_actoin_error
{
	const char*  ParamErrorOnFromGrid = "param error on from-grid";
	const char*  ParamErrorOnToGrid = "param error on to-grid";
	const char*  ExistingChessOnToGrid = "existing chess on to-grid";
	const char*  NotExistingChessOnFromGrid = "not existing chess on from-grid";
	const char*  CannotTakeChessOfOwnColorOnToGridWithoutRule = "cannot take chess of own color on to-grid without rule";
	const char*  ChessIdMismatchingOnFromGrid = "chess id mismatching on from-grid";
	const char*  ChessIdMismatchingOnFromGridAndNotPromotionNeither = "chess id mismatching on from-grid and not promotion neither";
	const char*  IllegalMoveWithRule = "illegal move with rule";
	const char*  CannotMovePieceIntoBoardActivelyWithRule = "cannot move piece into board actively with rule";
	const char*  CannotMovePieceOutofBoardActivelyWithRule = "cannot move piece out of board actively with rule";
	const char*  CannotClearBoardWithRule = "cannot clear board with rule";
	const char*  ParamErrorInEntityOfNewBoardEx = "param error in entity of NewBoardEx";
	const char*  CmdGroupIsNullOrEmtpyInManualImported = "cmd group is null or empty in the manual imported";
	const char*  NotExistingNewBoardCmdInManualImported = "not existing NewBoard/NewBoardEx in the manual imported";
}

GAME_NAMESPACE_END