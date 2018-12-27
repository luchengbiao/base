#ifndef __GAME_MODEL_GO_ACTIONS_GO_GAME_ACTION_NEW_BOARD_EX_H__
#define __GAME_MODEL_GO_ACTIONS_GO_GAME_ACTION_NEW_BOARD_EX_H__
#include "game/model/go/actions/go_game_action.h"

GAME_NAMESPACE_BEGIN

class GoActionNewBoardEx : public GoGameAction
{
	DECLARE_GO_GAME_ACTION_RUNTIME_INFO(GoActionNewBoardEx)

	virtual bool PropertyOfNeedingToBeCachedInOneGame() const override { return false; }
};

GAME_NAMESPACE_END

#endif