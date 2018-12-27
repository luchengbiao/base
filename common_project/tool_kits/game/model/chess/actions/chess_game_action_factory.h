#ifndef __GAME_MODEL_CHESS_ACTIONS_CHESS_GAME_ACTION_FACTORY_H__
#define __GAME_MODEL_CHESS_ACTIONS_CHESS_GAME_ACTION_FACTORY_H__
#include <memory>
#include "game/model/base/action/game_action_facotry.h"
#include "game/model/chess/actions/chess_game_action.h"

GAME_NAMESPACE_BEGIN

class ChessGameActionFactory : public GameActionFactory
{
public:
	using ChessGameActionPtr = std::shared_ptr<ChessGameAction>;

public:
	ChessGameActionFactory();

	virtual	GameActionPtr	CreateActionWithCommand(const CommandSharedPtr& cmd_ptr) override;

	ChessGameActionPtr		CreateGameActionWithType(ChessGameActionType) const;
};

GAME_NAMESPACE_END

#endif