#ifndef __GAME_MODEL_CHESS_SIMPLE_PATH_RULE_MOVE_TYPE_H__
#define __GAME_MODEL_CHESS_SIMPLE_PATH_RULE_MOVE_TYPE_H__
#include "game/macros.h"

GAME_NAMESPACE_BEGIN

enum class ChessMoveType
{
	None				= -1,
	Normal				= 0,	// 普通移子/
	Capture				= 1,	// 吃子 x/
	LongCastling		= 2,	// 长易位, 后翼易位 O-O-O/
	ShortCastling		= 3,	// 短易位, 王翼易位 O-O/
	Enpassant			= 4,	// 吃过路兵/
	Promotion			= 5,	// 升变 =/
	CaptureAndPromotion	= 6,	// 吃子&升变 x=/
};

GAME_NAMESPACE_END

#endif