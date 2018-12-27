#ifndef __GAME_MODEL_CHESS_ACTIONS_CHESS_GAME_ACTION_MOVE_INTO_BOARD_H__
#define __GAME_MODEL_CHESS_ACTIONS_CHESS_GAME_ACTION_MOVE_INTO_BOARD_H__
#include "game/model/base/action/game_action_move_base.h"
#include "game/model/chess/actions/chess_game_action.h"

GAME_NAMESPACE_BEGIN

class ChessActionMoveIntoBoard : public GameActionMoveBase<ChessGameAction>
{
	DECLARE_CHESS_GAME_ACTION_RUNTIME_INFO(ChessActionMoveIntoBoard)
};

GAME_NAMESPACE_END

#endif