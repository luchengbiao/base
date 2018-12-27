#ifndef __GAME_MODEL_CHESS_ACTIONS_CHESS_GAME_ACTION_NEW_BOARD_EX_H__
#define __GAME_MODEL_CHESS_ACTIONS_CHESS_GAME_ACTION_NEW_BOARD_EX_H__
#include "game/model/chess/actions/chess_game_action.h"

GAME_NAMESPACE_BEGIN

class ChessActionNewBoardEx : public ChessGameAction
{
	DECLARE_CHESS_GAME_ACTION_RUNTIME_INFO(ChessActionNewBoardEx)

	virtual bool PropertyOfNeedingToBeCachedInOneGame() const override { return false; }
};

GAME_NAMESPACE_END

#endif