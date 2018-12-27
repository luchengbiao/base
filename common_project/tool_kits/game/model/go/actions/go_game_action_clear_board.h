#ifndef __GAME_MODEL_GO_ACTIONS_GO_GAME_ACTION_CLEAR_BOARD_H__
#define __GAME_MODEL_GO_ACTIONS_GO_GAME_ACTION_CLEAR_BOARD_H__
#include "game/model/base/action/game_action_clear_board_base.h"
#include "game/model/go/actions/go_game_action.h"

GAME_NAMESPACE_BEGIN

class GoActionClearBoard : public GameActionClearBoardBase<GoGameAction>
{
	DECLARE_GO_GAME_ACTION_RUNTIME_INFO(GoActionClearBoard)
};

GAME_NAMESPACE_END

#endif