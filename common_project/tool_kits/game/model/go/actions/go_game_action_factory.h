#ifndef __GAME_MODEL_GO_ACTIONS_GO_GAME_ACTION_FACTORY_H__
#define __GAME_MODEL_GO_ACTIONS_GO_GAME_ACTION_FACTORY_H__
#include <memory>
#include "game/model/base/action/game_action_facotry.h"
#include "game/model/go/actions/go_game_action.h"

GAME_NAMESPACE_BEGIN

class GoGameActionFactory : public GameActionFactory
{
public:
	using GoGameActionPtr = std::shared_ptr<GoGameAction>;

public:
	GoGameActionFactory();

	virtual	GameActionPtr	CreateActionWithCommand(const CommandSharedPtr& cmd_ptr) override;

	GoGameActionPtr			CreateGameActionWithType(GoGameActionType) const;
};

GAME_NAMESPACE_END

#endif