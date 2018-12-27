#ifndef __GAME_MODEL_BASE_ACTION_GAME_ACTION_FACTORY_H__
#define __GAME_MODEL_BASE_ACTION_GAME_ACTION_FACTORY_H__
#include <memory>
#include "protocol/command/base/command_fwd.h"
#include "game/macros.h"

GAME_NAMESPACE_BEGIN

class GameAction;

class GameActionFactory
{
public:
	using GameActionPtr = std::shared_ptr<GameAction>;

public:
	virtual ~GameActionFactory() = default;

	virtual	GameActionPtr	CreateActionWithCommand(const CommandSharedPtr& cmd_ptr) = 0;
};

GAME_NAMESPACE_END

#endif