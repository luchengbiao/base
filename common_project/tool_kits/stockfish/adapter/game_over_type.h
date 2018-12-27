#ifndef STOCKFISH_ADAPTER_GAME_OVER_TYPE_H_INCLUDED
#define STOCKFISH_ADAPTER_GAME_OVER_TYPE_H_INCLUDED
#include "stockfish/macros.h"

STK_NAMESPACE_BEGIN

enum class GameOverType
{
	None,
	WhiteWin,
	BlackWin,
	Draw,
};

STK_NAMESPACE_END

#endif