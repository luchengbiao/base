#ifndef __GAME_MODEL_BASE_BOARD_GAME_BOARD_H__
#define __GAME_MODEL_BASE_BOARD_GAME_BOARD_H__
#include <assert.h>
#include "game_board_base.h"
#include "game/model/base/action/game_action_result.h"

GAME_NAMESPACE_BEGIN

template<int XGrid, int YGrid = XGrid>
class GameBoard : public GameBoardBase
{
public:
	static const int MyXGrid = XGrid;
	static const int MyYGrid = YGrid;

public:
	GameBoard(GameBoardType board_type, const GameRecordsPtr& all_games_to_be_cached) : GameBoardBase(board_type, MyXGrid, MyYGrid, all_games_to_be_cached)
	{}
};

GAME_NAMESPACE_END

#endif