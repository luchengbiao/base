#ifndef __GAME_MODEL_CHESS_ACTIONS_CHESS_GAME_ACTION_NEW_BOARD_H__
#define __GAME_MODEL_CHESS_ACTIONS_CHESS_GAME_ACTION_NEW_BOARD_H__
#include "game/model/chess/actions/chess_game_action.h"

GAME_NAMESPACE_BEGIN

class ChessActionNewBoard : public ChessGameAction
{
	DECLARE_CHESS_GAME_ACTION_RUNTIME_INFO(ChessActionNewBoard)

	virtual bool PropertyOfNeedingToBeCachedInOneGame() const override { return false; }
};

GAME_NAMESPACE_END

#endif