#ifndef __GAME_PUBLIC_RESOURCE_LOADER_H__
#define __GAME_PUBLIC_RESOURCE_LOADER_H__
#include "game/macros.h"

GAME_NAMESPACE_BEGIN

class ResourceManager
{
public:
	static void LoadResource();

private:
	ResourceManager() = delete;
};

GAME_NAMESPACE_END

#endif