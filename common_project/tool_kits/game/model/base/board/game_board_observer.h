#ifndef __GAME_MODEL_BASE_BOARD_GAME_BOARD_OBSERVER_H__
#define __GAME_MODEL_BASE_BOARD_GAME_BOARD_OBSERVER_H__
#include "game/macros.h"

GAME_NAMESPACE_BEGIN

class GameActionResult;

class GameBoardObserver
{
public:
	virtual ~GameBoardObserver() = default;

	virtual void OnGameActionDone(const GameActionResult&) = 0;
	virtual void OnGameActionUndone(const GameActionResult&) = 0;
	virtual void OnGameActionRedone(const GameActionResult&) = 0;
};

GAME_NAMESPACE_END

#endif