#ifndef __GAME_MODEL_GO_ACTIONS_GO_GAME_ACTION_NEW_BOARD_H__
#define __GAME_MODEL_GO_ACTIONS_GO_GAME_ACTION_NEW_BOARD_H__
#include "game/model/go/actions/go_game_action.h"

GAME_NAMESPACE_BEGIN

class GoActionNewBoard : public GoGameAction
{
	DECLARE_GO_GAME_ACTION_RUNTIME_INFO(GoActionNewBoard)

	virtual bool PropertyOfNeedingToBeCachedInOneGame() const override { return false; }
};

GAME_NAMESPACE_END

#endif