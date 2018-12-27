#ifndef __GAME_PUBLIC_UNSIGNED_INCLUDER_UNSIGNED_INCLUDER_H__
#define __GAME_PUBLIC_UNSIGNED_INCLUDER_UNSIGNED_INCLUDER_H__
#include "game/macros.h"

GAME_NAMESPACE_BEGIN

template<typename T, unsigned int MaxCount>
class UnsignedIncluder
{
public:
	template<typename InitList>
	UnsignedIncluder(const InitList& init_list)
	{
		memset(included_, 0x00, sizeof(included_));

		for (const auto& item : init_list)
		{
			if (static_cast<unsigned int>(item) < MaxCount)
			{
				included_[static_cast<unsigned int>(item)] = 1;
			}
		}
	}

	inline bool Includes(const T value) const
	{
		return (static_cast<unsigned int>(value) < MaxCount && included_[static_cast<unsigned int>(value)]);
	}

private:
	char included_[MaxCount];
};

GAME_NAMESPACE_END

#endif