#ifndef __GAME_MODEL_CHESS_ACTIONS_CHESS_GAME_ACTION_CLEAR_BOARD_H__
#define __GAME_MODEL_CHESS_ACTIONS_CHESS_GAME_ACTION_CLEAR_BOARD_H__
#include "game/model/base/action/game_action_clear_board_base.h"
#include "game/model/chess/actions/chess_game_action.h"

GAME_NAMESPACE_BEGIN

class ChessActionClearBoard : public GameActionClearBoardBase<ChessGameAction>
{
	DECLARE_CHESS_GAME_ACTION_RUNTIME_INFO(ChessActionClearBoard)
};

GAME_NAMESPACE_END

#endif