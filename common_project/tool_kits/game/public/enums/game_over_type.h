#ifndef __GAME_PUBLIC_ENUMS_GAME_OVER_TYPE_H__
#define __GAME_PUBLIC_ENUMS_GAME_OVER_TYPE_H__
#include "game/macros.h"
#include "stockfish/adapter/game_over_type.h"

GAME_NAMESPACE_BEGIN

enum class GameOverType
{
	None,
	WhiteWin,
	BlackWin,
	Draw,
};

GAME_NAMESPACE_END

#endif