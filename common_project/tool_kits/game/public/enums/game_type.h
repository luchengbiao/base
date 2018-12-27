#ifndef __GAME_PUBLIC_ENUMS_GAME_TYPE_H__
#define __GAME_PUBLIC_ENUMS_GAME_TYPE_H__
#include "game/macros.h"

GAME_NAMESPACE_BEGIN

enum class GameType
{
	None	= -1,
	Go		= 0,
	Chess	= 1,
};

GAME_NAMESPACE_END

#endif