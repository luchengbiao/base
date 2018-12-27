#ifndef __GAME_PUBLIC_ENUMS_GAME_MODE_H__
#define __GAME_PUBLIC_ENUMS_GAME_MODE_H__
#include "game/macros.h"

GAME_NAMESPACE_BEGIN

enum class GameMode
{
	None		= -1,
	Teaching	= 0,
	VS			= 1,
};

GAME_NAMESPACE_END

#endif