#ifndef __GAME_MODEL_BASE_BOARD_GAME_BOARD_BASE_FWD_H__
#define __GAME_MODEL_BASE_BOARD_GAME_BOARD_BASE_FWD_H__
#include <memory>
#include "game/macros.h"

GAME_NAMESPACE_BEGIN

class GameBoardBase;

typedef std::shared_ptr<GameBoardBase> GameBoardPtr;

GAME_NAMESPACE_END

#endif