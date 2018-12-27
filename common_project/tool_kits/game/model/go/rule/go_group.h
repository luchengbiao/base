#ifndef __GAME_MODEL_GO_RULE_GO_GROUP_H__
#define __GAME_MODEL_GO_RULE_GO_GROUP_H__
#include <assert.h>
#include <map>
#include <set>
#include <type_traits>
#include <vector>
#include "game/macros.h"

GAME_NAMESPACE_BEGIN

template<int Grid, typename IdT = int>
using Pieces = IdT[Grid][Grid];

#define U  0 // up
#define L  1 // left
#define D  2 // down
#define R  3 // right

/***
*Grid: the grid count of go board
*Black: id or flag of black go stone in board
*White: id or flag of white go stone in board
*Empty: id or flag of empty stone in board
*/
template<int Grid, int Black = 0, int White = 1, int Empty = -1>
class GoGroup
{
public:
	enum  { TotalGrid = Grid * Grid };
	static int Indices[TotalGrid][4]; // <index, <up, left, down, right>>

	typedef Pieces<Grid> MyPieces;

public:
	GoGroup(const int color)
		: color_(color)
	{
		memset(grids_, 0, sizeof(grids_));
	}

	inline int Color() const { return color_; }

	inline bool IsEmpty() const
	{
		return first_valid_index_ == last_valid_index_;
	}

	bool TestToAdd(const int x, const int y)
	{
		bool canAdd = false;

		if (IsEmpty())
		{
			canAdd = true;
		}
		else
		{
			canAdd = this->HasGridAdjacentTo(x, y);
		}

		if (canAdd)
		{
			Add(x, y);
		}

		return canAdd;
	}

	bool HasGridAdjacentTo(const int x, const int y)
	{
		assert(IsGridValid(x, y));

		int idx = IndexOfGrid(x, y);

		for (int i = 0; i < 4; ++i)
		{
			if (GoGroup::Indices[idx][i] != -1 && grids_[GoGroup::Indices[idx][i]])
			{
				return true;
			}
		}

		return false;
	}

	inline void Add(const int x, const int y)
	{
		this->Add(IndexOfGrid(x, y));
	}

	void TryToAddRecursively(const MyPieces& pieces, const int x, const int y)
	{
		assert(IsGridValid(x, y));

		int idx = IndexOfGrid(x, y);
		if (grids_[idx] || pieces[x][y] != color_) // return if already added or color is not same
		{
			return;
		}

		this->Add(idx);

		int left = x - 1;
		int right = x + 1;
		int up = y - 1;
		int down = y + 1;

		if (left >= 0 && pieces[left][y] == color_)
		{
			this->TryToAddRecursively(pieces, left, y);
		}

		if (right < Grid && pieces[right][y] == color_)
		{
			this->TryToAddRecursively(pieces, right, y);
		}

		if (up >= 0 && pieces[x][up] == color_)
		{
			this->TryToAddRecursively(pieces, x, up);
		}

		if (down < Grid && pieces[x][down] == color_)
		{
			this->TryToAddRecursively(pieces, x, down);
		}
	}

	inline bool HasGrid(const int x, const int y) const
	{ 
		return IsGridValid(x, y) && grids_[IndexOfGrid(x, y)];
	}

	inline bool IsAlive() const
	{
		return liberty_ > 0;
	}

	inline bool IsDead() const
	{
		return liberty_ == 0;
	}

	void Joint(const GoGroup& right)
	{
		if (&right != this && !right.IsEmpty())
		{
			if (IsEmpty())
			{
				*this = right;
			}
			else
			{
				for (int i = right.first_valid_index_; i < right.last_valid_index_; ++i)
				{
					if (right.grids_[i])
					{
						grids_[i] = right.grids_[i];
					}
				}

				if (right.first_valid_index_ < first_valid_index_)
				{
					first_valid_index_ = right.first_valid_index_;
				}

				if (right.last_valid_index_ > last_valid_index_)
				{
					last_valid_index_ = right.last_valid_index_;
				}
			}
		}
	}

	inline int Liberty() const
	{ 
		return liberty_;
	}

	void CalcLiberty(const MyPieces& pieces)
	{
		liberty_ = 0;

		if (!IsEmpty())
		{
			char libterties[TotalGrid];
			memset(libterties, 0, sizeof(libterties));

			for (int i = first_valid_index_, j = 0; i < last_valid_index_; ++i)
			{
				if (!grids_[i]) { continue; }

				const int x = XOfIndex(i);
				const int y = YOfIndex(i);

				const int grids[4][2] = { { x - 1, y }, { x + 1, y }, { x, y - 1 }, { x, y + 1 } };
				for (int i = 0; i < 4; ++i)
				{
					if (IsGridValid(grids[i][0], grids[i][1]) && pieces[grids[i][0]][grids[i][1]] == Empty)
					{
						libterties[IndexOfGrid(grids[i][0], grids[i][1])] += 1;
					}
				}
			}

			for (auto liberty : libterties)
			{
				if (liberty) { liberty_ += 1; }
			}
		}
	}

	int CountOfGrid() const
	{
		int count = 0;

		for (int i = first_valid_index_; i < last_valid_index_; ++i)
		{
			if (grids_[i])
			{
				count += 1;
			}
		}

		return count;
	}

	bool FirstGrid(int* x, int* y)
	{
		if (first_valid_index_ != -1)
		{
			if (x) { *x = XOfIndex(first_valid_index_); }
			if (y) { *y = YOfIndex(first_valid_index_); }

			return true;
		}

		return false;
	}

	template<typename Callable>
	void ForeachGrid(Callable&& callback) const
	{
		for (int i = first_valid_index_; i < last_valid_index_; ++i)
		{
			if (grids_[i])
			{
				callback(XOfIndex(i), YOfIndex(i));
			}
		}
	}

	template<typename OStream>
	OStream& Output(OStream& stream) const
	{
		if (!IsEmpty())
		{
			stream << "color(" << color_ << ") hp(" << liberty_ << ") -- Grids: ";
			for (int i = first_valid_index_, j = 1; i < last_valid_index_; ++i)
			{
				if (grids_[i])
				{
					stream << j++ << "(" << XOfIndex(i) << ", " << YOfIndex(i) << ") ";
				}
			}
		}

		return stream;
	}

	static void Init()
	{
		auto& indices = GoGroup::Indices;
		
		int x, y;

		for (int i = 0; i < TotalGrid; ++i)
		{
			x = XOfIndex(i);
			y = YOfIndex(i);

			indices[i][U] = (y > 0) ? (i - Grid) : -1;
			indices[i][L] = (x > 0) ? (i - 1) : -1;
			indices[i][D] = (y < (Grid - 1)) ? (i + Grid) : -1;
			indices[i][R] = (x < (Grid - 1)) ? (i + 1) : -1;
		}
	}

	inline static int LibertyAt(const MyPieces& pieces, const int x, const int y)
	{
		int liberty = 4;

		const int grids[4][2] = { { x - 1, y }, { x + 1, y }, { x, y - 1 }, { x, y + 1 } };

		for (int i = 0; i < 4; ++i)
		{
			if (!IsGridValid(grids[i][0], grids[i][1]) || pieces[grids[i][0]][grids[i][1]] != Empty) // border or captured
			{
				liberty -= 1;
			}
		}

		return liberty;
	}

	inline static bool IsGridValid(const int x, const int y)
	{
		return x >= 0 && x < Grid && y >= 0 && y < Grid;
	}

	inline static int IndexOfGrid(const int x, const int y)
	{
		return y * Grid + x;
	}

	inline static int XOfIndex(int index)
	{
		return index % Grid;
	}

	inline static int YOfIndex(int index)
	{
		return index / Grid;
	}

private:
	void Add(const int index)
	{
		grids_[index] = 1;

		if (first_valid_index_ == -1 || index < first_valid_index_)
		{
			first_valid_index_ = index;
		}

		if (last_valid_index_ == -1 || index >= last_valid_index_)
		{
			last_valid_index_ = index + 1;
		}
	}


private:
	int		color_;
	char	grids_[TotalGrid]; //1: has grid in the grid indexed, 0: no grid in the grid indexed

	int		liberty_{ 0 };

	// [first_valid_index_, last_valid_index_)
	int		first_valid_index_{ -1 };
	int		last_valid_index_{ -1 };
};

template<int Grid, int Black, int White, int Empty>
int GoGroup<Grid, Black, White, Empty>::Indices[TotalGrid][4] = { 0 };

template<int Grid, int Black, int White, int Empty>
class GoGroups : public std::vector<GoGroup<Grid, Black, White, Empty>>
{
	typedef GoGroup<Grid, Black, White, Empty> MyGroup;
	typedef std::vector<MyGroup> MyBase;
	typedef Pieces<Grid> MyPieces;

public:
	GoGroups() = default;
	GoGroups(const GoGroups&) = default;
	GoGroups(GoGroups&& right)
		: MyBase(std::move(right))
	{}

	GoGroups& operator=(const GoGroups&) = default;
	GoGroups& operator=(GoGroups&& right)
	{
		if (&right != this)
		{
			MyBase::operator =(std::move(right));
		}

		return (*this)
	}

	void Add(const int color, const int x, const int y)
	{
		std::vector<int> vec_added;

		if (!empty())
		{
			int index = -1;
			for (auto& group : *this)
			{
				index += 1;
				if (group.Color() == color && group.TestToAdd(x, y))
				{
					vec_added.push_back(index);
				}
			}
		}

		if (vec_added.empty())
		{
			MyGroup group(color);
			group.Add(x, y);

			this->push_back(std::move(group));
		}
		else if (vec_added.size() > 1)
		{
			MyGroup& group = at(vec_added[0]);
			for (int i = 1; i < vec_added.size(); ++i)
			{
				group.Joint(at(vec_added[i]));
			}

			for (int i = 1; i < vec_added.size(); ++i)
			{
				this->erase(cbegin() + vec_added[i] + (i - 1)); // count of elements erased is (i - 1)
			}
		}
	}

	void CalcLiberty(const MyPieces& pieces)
	{
		if (!empty())
		{
			for (auto& group : *this)
			{
				group.CalcLiberty(pieces);
			}
		}
	}

	bool HasGrid(const int x, const int y) const
	{
		if (!empty())
		{
			for (const auto& group : *this)
			{
				if (group.HasGrid(x, y))
				{
					return true;
				}
			}
		}

		return false;
	}

	template<typename OStream>
	OStream& Output(OStream& stream) const
	{
		stream << "GroupCount(" << size() << ")\n";

		if (!empty())
		{
			int i = 1;
			for (const auto& group : *this)
			{
				stream << i++ << ": ";
				group.Output(stream) << "\n";
			}
		}

		return stream;
	}
};

#undef U
#undef L
#undef D
#undef R


GAME_NAMESPACE_END

#endif