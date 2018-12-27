#ifndef __GAME_MODEL_GO_ACTIONS_GO_GAME_ACTION_UPDATE_AUTO_TAKING_PIECES_H__
#define __GAME_MODEL_GO_ACTIONS_GO_GAME_ACTION_UPDATE_AUTO_TAKING_PIECES_H__
#include "game/model/go/actions/go_game_action.h"

GAME_NAMESPACE_BEGIN

class GoActionUpdateAutoTakingPieces: public GoGameAction
{
	DECLARE_GO_GAME_ACTION_RUNTIME_INFO(GoActionUpdateAutoTakingPieces)

	virtual bool	PropertyOfChangingBoard() const override { return false; }
	virtual bool	PropertyOfMakingBoardValid() const override { return true; }
	virtual bool	PropertyOfBeingCountedAsOneStepWhenBackForward() const override { return false; }
};

GAME_NAMESPACE_END

#endif