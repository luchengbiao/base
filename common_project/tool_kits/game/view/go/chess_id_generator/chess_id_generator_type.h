#ifndef __GAME_VIEW_GO_CHESS_ID_GENERATOR_CHESS_ID_GENERATOR_TYPE_H__
#define __GAME_VIEW_GO_CHESS_ID_GENERATOR_CHESS_ID_GENERATOR_TYPE_H__
#include "game/macros.h"

GAME_NAMESPACE_BEGIN

enum class ChessIdGeneratorType
{
	None,
	Alternate,
	OnlyBlack,
	OnlyWhite,
	Default = Alternate,
};

GAME_NAMESPACE_END

#endif