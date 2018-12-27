#ifndef __GAME_PUBLIC_ENUMS_CLIENT_TYPE_H__
#define __GAME_PUBLIC_ENUMS_CLIENT_TYPE_H__
#include "game/macros.h"

GAME_NAMESPACE_BEGIN

enum class ClientType
{
	None	= -1,
	Student	= 0,
	Teacher	= 1,
};

GAME_NAMESPACE_END

#endif