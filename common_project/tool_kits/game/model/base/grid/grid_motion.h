#ifndef __GAME_MODEL_BASE_GRID_GRID_MOTION_H__
#define __GAME_MODEL_BASE_GRID_GRID_MOTION_H__
#include "game/model/base/grid/grid_index.h"

GAME_NAMESPACE_BEGIN

class GridMotion
{
public:
	GridMotion(const GridIndex& from = GridIndex::InvalidGrid, const GridIndex& to = GridIndex::InvalidGrid)
		: from_(from)
		, to_(to)
		, first(from_)
		, second(to_)
	{}

	inline const GridIndex& From() const { return from_; }
	inline const GridIndex& To() const { return to_; }

	inline void SetFrom(const GridIndex& from) { from_ = from; }
	inline void SetTo(const GridIndex& to) { to_ = to; }
	inline void SetFromTo(const GridIndex& from, const GridIndex& to) { from_ = from; to_ = to; }

	inline void Invalid() { from_ = to_ = GridIndex::InvalidGrid; }

	inline bool IsValid() const { return from_ != to_; }
	inline explicit operator bool() const { return IsValid(); }

	GridMotion& operator=(const GridMotion& rhs)
	{
		if (&rhs != this)
		{
			from_ = rhs.from_;
			to_ = rhs.to_;
		}

		return (*this);
	}

private:
	GridIndex from_;
	GridIndex to_;

public:
	// std::pair<GridIndex, GridIndex> compatibility
	GridIndex& first;
	GridIndex& second;
};

GAME_NAMESPACE_END

#endif