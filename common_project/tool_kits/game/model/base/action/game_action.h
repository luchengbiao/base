#ifndef __GAME_MODEL_BASE_ACTION_GAME_ACTION_H__
#define __GAME_MODEL_BASE_ACTION_GAME_ACTION_H__
#include "base/memory/objectcounter.h"
#include "game/macros.h"
#include "protocol/command/command_types.h"

GAME_NAMESPACE_BEGIN

class GameBoardBase;
class GameActionResult;

class GameAction
#ifdef GAME_TEST_MEMORY_LEAK
	: public nbase::ObjectCounter<GameAction>
#endif
{
public:
	virtual ~GameAction() = default;

	virtual	GameActionResult	Do(GameBoardBase*) = 0;
	virtual	void				Undo(GameBoardBase*) = 0;
	virtual	void				Redo(GameBoardBase*) = 0;

	virtual	proto::CommandType	CmdType() const = 0;

	// Ability to change chesses in a board.
	virtual bool				PropertyOfChangingBoard() const = 0;

	// There is no need for cache some actions to be cached in a game record, such as NewBoard and BackForward etc.
	virtual bool				PropertyOfNeedingToBeCachedInOneGame() const = 0;

	// Now only moving-chess, clearing-board and update-auto-taking-pieces can make a board valid.
	virtual bool				PropertyOfMakingBoardValid() const = 0;

	// Subset of PropertyOfMakingBoardValid, Now only moving-chess and clearing-board can be counted as one step when back-forward.
	virtual bool				PropertyOfBeingCountedAsOneStepWhenBackForward() const = 0;
};

GAME_NAMESPACE_END

#endif