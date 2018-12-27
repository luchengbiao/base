#ifndef __GAME_MODEL_CHESS_CHESS_ENUM__
#define __GAME_MODEL_CHESS_CHESS_ENUM__
#include "game/macros.h"

GAME_NAMESPACE_BEGIN

enum class ChessColor
{
	None	= -1,
	White	= 0,
	Black	= 1,
};

enum class ChessType
{
	None	= -1,
	Pawn	= 0,		//兵/
	Rook	= 1,		//车/
	Knight	= 2,		//马/
	Bishop	= 3,		//象/
	Queen	= 4,		//王后/
	King	= 5,		//王/
};

GAME_NAMESPACE_END

#endif