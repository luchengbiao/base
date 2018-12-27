#ifndef __GAME_PUBLIC_ENUMS_PLAYER_SIDE_H__
#define __GAME_PUBLIC_ENUMS_PLAYER_SIDE_H__
#include "game/macros.h"

GAME_NAMESPACE_BEGIN

enum class PlayerSide
{
	None	= -1,
	Black	= 0,
	White	= 1,
};

GAME_NAMESPACE_END

#endif