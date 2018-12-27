#ifndef __GAME_MODEL_BASE_GRID_GRID_INDEX__
#define __GAME_MODEL_BASE_GRID_GRID_INDEX__
#include <functional> // for std::hash
#include "game/macros.h"

GAME_NAMESPACE_BEGIN

class GridIndex
{
public:
	static const int InvalidIndex = -1;

	static const GridIndex InvalidGrid;

public:
	GridIndex() = default;

	GridIndex(int x, int y) 
	: x_(x)
	, y_(y)
	{}

	inline void SetX(int x) { x_ = x; }
	inline void SetY(int y) { y_ = y; }
	inline void SetXY(int x, int y) { x_ = x; y_ = y; }

	inline int X() const { return x_; }
	inline int Y() const { return y_; }

	inline bool IsValid() const { return x_ >= 0 && y_ >= 0; }
	inline void Invalid() { x_ = y_ = InvalidIndex; }

	inline bool IsInSquare(int square) const { return x_ >= 0 && x_ < square && y_ >= 0 && y_ < square; }

	explicit operator bool() const { return IsValid(); }

private:
	int x_{ InvalidIndex };
	int y_{ InvalidIndex };

	friend inline bool operator==(const GridIndex&, const GridIndex&);
	friend inline bool operator!=(const GridIndex&, const GridIndex&);
	friend inline bool operator<(const GridIndex&, const GridIndex&);
	friend inline const GridIndex operator+(const GridIndex&, const GridIndex&);
	friend inline const GridIndex operator-(const GridIndex&, const GridIndex&);
	friend struct std::hash<GridIndex>;
	friend struct std::equal_to<GridIndex>;
};

inline bool operator==(const GridIndex& lh, const GridIndex& rh)
{
	return lh.x_ == rh.x_ && lh.y_ == rh.y_;
}

inline bool operator!=(const GridIndex& lh, const GridIndex& rh)
{
	return !(lh == rh);
}

inline bool operator<(const GridIndex& lh, const GridIndex& rh)
{
	if (lh.x_ < rh.x_) { return true; }
	else if (lh.x_ == rh.x_)
	{
		return (lh.y_ < rh.y_);
	}
	return false;
}

inline const GridIndex operator+(const GridIndex& lh, const GridIndex& rh)
{
	return GridIndex(lh.x_ + rh.x_, lh.y_ + rh.y_);
}

inline const GridIndex operator-(const GridIndex& lh, const GridIndex& rh)
{
	return GridIndex(lh.x_ - rh.x_, lh.y_ - rh.y_);
}

GAME_NAMESPACE_END

// full specialization std::hash and std::equal_to with GridIndex 
// for the usage of std::unordered_set<GridIndex, std::hash<GridIndex>, std::equal_to<GridIndex>> and 
// std::unordered_map<GridIndex, Value, std::hash<GridIndex>, std::equal_to<GridIndex>> etc.
namespace std
{ 
	template<> 
	struct hash<game::GridIndex> 
	{ 
		typedef game::GridIndex argument_type;
		typedef size_t result_type;

		result_type operator()(const argument_type& rh) const
		{ 
			return hash<int>()(rh.x_) ^ (hash<int>()(rh.y_) << 1);
		} 
	}; 

	template<> 
	struct equal_to<game::GridIndex> 
	{ 
		typedef game::GridIndex first_argument_type;
		typedef game::GridIndex second_argument_type;
		typedef bool result_type;

		result_type operator()(const first_argument_type& lh, const second_argument_type& rh) const
		{ 
			return lh.x_ == rh.x_ && lh.y_ == rh.y_;
		} 
	};
}

#endif