#ifndef __GAME_MODEL_CHESS_ACTIONS_CHESS_GAME_ACTION_BACK_FORWARD_H__
#define __GAME_MODEL_CHESS_ACTIONS_CHESS_GAME_ACTION_BACK_FORWARD_H__
#include "game/model/base/action/game_action_back_forward_base.h"
#include "game/model/chess/actions/chess_game_action.h"

GAME_NAMESPACE_BEGIN

class ChessActionBackForward : public GameActionBackForwardBase<ChessGameAction>
{
	DECLARE_CHESS_GAME_ACTION_RUNTIME_INFO(ChessActionBackForward)
};

GAME_NAMESPACE_END

#endif