#include "game/macros.h"

GAME_NAMESPACE_BEGIN

namespace go_actoin_error
{
	const char*  ParamErrorOnFromGrid = "param error on from-grid";
	const char*  ParamErrorOnToGrid = "param error on to-grid";
	const char*  ExistingChessOnToGrid = "existing chess on to-grid";
	const char*  NotExistingChessOnFromGrid = "not existing chess on from-grid";
	const char*  CannotTakeChessOfOwnColorOnToGrid = "cannot take chess of own color on to-grid";
	const char*  ChessIdMismatchingOnFromGrid = "chess id mismatching on from-grid";
	const char*  CannotMovePieceOutofBoardActivelyWithRule = "cannot move piece out of board actively with rule";
	const char*  CannotMovePieceInsideofBoardActivelyWithRule = "cannot move piece inside of board actively with rule";
	const char*  CannotClearBoardWithRule = "cannot clear board with rule";
	const char*  IsNotTheRightSideToMoveWithRule = "is not the right side to move with rule";
	const char*  ToGridIsANoGoPoint = "to-grid is a no-go point";
	const char*  ToGridIsKoPoint = "to-grid is a ko point";
	const char*  ParamErrorInEntityOfNewBoardEx = "param error in entity of NewBoardEx";
	const char*  CannotUpdateAutoTakingPiecesWithRule = "cannot update auto taking pieces with rule";
	const char*  CmdGroupIsNullOrEmtpyInManualImported = "cmd group is null or empty in the manual imported";
	const char*  NotExistingNewBoardCmdInManualImported = "not existing NewBoard/NewBoardEx in the manual imported";
}

GAME_NAMESPACE_END