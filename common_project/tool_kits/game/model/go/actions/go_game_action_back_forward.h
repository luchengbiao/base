#ifndef __GAME_MODEL_GO_ACTIONS_GO_GAME_ACTION_BACK_FORWARD_H__
#define __GAME_MODEL_GO_ACTIONS_GO_GAME_ACTION_BACK_FORWARD_H__
#include "game/model/base/action/game_action_back_forward_base.h"
#include "game/model/go/actions/go_game_action.h"

GAME_NAMESPACE_BEGIN

class GoActionBackForward : public GameActionBackForwardBase<GoGameAction>
{
	DECLARE_GO_GAME_ACTION_RUNTIME_INFO(GoActionBackForward)
};

GAME_NAMESPACE_END

#endif