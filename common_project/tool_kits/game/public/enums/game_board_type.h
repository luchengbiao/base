#ifndef __GAME_PUBLIC_ENUMS_GAME_BOARD_TYPE_H__
#define __GAME_PUBLIC_ENUMS_GAME_BOARD_TYPE_H__
#include "game/macros.h"

GAME_NAMESPACE_BEGIN

enum class GameBoardType
{
	None	= -1,
	Go9		= 0,
	Go13	= 1,
	Go19	= 2,
	Chess	= 3,
};

inline bool IsValidBoardType(const GameBoardType board_type)
{
	return board_type == GameBoardType::Go9 ||
		   board_type == GameBoardType::Go13 || 
		   board_type == GameBoardType::Go19 ||
		   board_type == GameBoardType::Chess;
}

GAME_NAMESPACE_END

#endif