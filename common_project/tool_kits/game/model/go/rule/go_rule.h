#ifndef __GAME_MODEL_GO_RULE_GO_RULE_H__
#define __GAME_MODEL_GO_RULE_GO_RULE_H__
#include <algorithm>
#include <sstream>
#include "log/log.h"
#include "game/model/go/go_trait.h"
#include "game/model/go/rule/go_group.h"

#define PRINT_RULE_LOG 0

GAME_NAMESPACE_BEGIN

/***
*Grid: the grid count of go board
*Black: id or flag of black go stone in board
*White: id or flag of white go stone in board
*Empty: id or flag of empty stone in board
*/
template<int Grid, int Black = 0, int White = 1, int Empty = -1>
class GoRule
{
public:
	typedef GoGroup<Grid, Black, White, Empty> MyGroup;
	typedef GoGroups<Grid, Black, White, Empty> MyGroups;
	typedef Pieces<Grid> MyPieces;

	static const GoRule* Singleton()
	{
		static GoRule go_rule;

		return &go_rule;
	}

	/***
	*get groups whose liberty are 0
	*pieces: all current pieces in board
	*color: piece color to get; -1: get both black and white; Black: get black; White: get white
	*/
	MyGroups DeadGroups(const MyPieces& pieces, const int color = -1) const
	{
		return GroupsWithLibertyRange(pieces, 0, 0, color);
	}
	template<typename GoBoard>
	MyGroups DeadGroups(const GoBoard& go_board, const int color = -1) const
	{
		MyPieces pieces;
		go_board.ChessIds(pieces);
		return DeadGroups(pieces, color);
	}

	/***
	*check whether grid(x, y) is a no-go point for the side of color
	*pieces: all current pieces in board
	*x:
	*y:
	*color: must be Black or White
	*/
	bool IsNoGoPoint(const MyPieces& pieces, const int x, const int y, int color) const
	{
		bool no_go = false;

		do 
		{
			if (pieces[x][y] != Empty) { break; } // existing stone here, not banning point

			if (MyGroup::LibertyAt(pieces, x, y) > 0) { break; } // still has liberty here, not no-go point

			auto& tmp_pieces = const_cast<MyPieces&>(pieces);
			PieceGuard piece_guard(tmp_pieces, x, y);

			tmp_pieces[x][y] = color; // put a piece at (x, y), and PieceGuard will restore the old piece

			if (GroupWithSpecifiedGrid(tmp_pieces, x, y).IsAlive()) { break; } // group containing me still has liberty, not no-go point

			// finally check whether any of opposite groups around has no liberty
			MyGroups groups = DeadGroupsAdjacentToSpecifiedGrid(tmp_pieces, x, y, color == Black ? White : Black);
			if (!groups.empty()) { break; } // i can take opposite group, not no-go point
			
			no_go = true;
		} while (false);

		return no_go;
	}
	template<typename GoBoard>
	bool IsNoGoPoint(const GoBoard& go_board, const int x, const int y, int color) const
	{
		MyPieces pieces;
		go_board.ChessIds(pieces);
		return IsNoGoPoint(pieces, x, y, color);
	}

	/***
	*check whether grid(x, y) is a underlying ko point for the side of color
	*pieces: all current pieces in board
	*x:
	*y:
	*color: must be Black or White
	*/
	bool IsUnderlyingKoPoint(const MyPieces& pieces, const int x, const int y, int color, int* x_ko = nullptr, int* y_ko = nullptr) const
	{
		bool ko = false;

		do 
		{
			if (pieces[x][y] != Empty) { break; } // existing stone here, not ko point

			if (MyGroup::LibertyAt(pieces, x, y) > 0) { break; } // still has liberty here, not ko point

			auto& tmp_pieces = const_cast<MyPieces&>(pieces);
			PieceGuard piece_guard(tmp_pieces, x, y);

			tmp_pieces[x][y] = color; // put a piece at (x, y), and PieceGuard will restore the old piece

			if (GroupWithSpecifiedGrid(tmp_pieces, x, y).IsAlive()) { break; } // group containing me still has liberty, not ko point

			// check whether any of opposite groups around (x, y) has no liberty
			int x_opp = -1, y_opp = -1;
			{
				MyGroups groups_opp = DeadGroupsAdjacentToSpecifiedGrid(tmp_pieces, x, y, color == Black ? White : Black);
				if (groups_opp.size() == 1 && groups_opp.front().IsDead() && groups_opp.front().CountOfGrid() == 1)
				{
					groups_opp.front().FirstGrid(&x_opp, &y_opp);
				}
				else
				{
					break;
				}
			}

			// check whether any of self groups around (x_opp, y_opp) has no liberty
			int x_self = -1, y_self = -1;
			{
				MyGroups groups_self = DeadGroupsAdjacentToSpecifiedGrid(tmp_pieces, x_opp, y_opp, color);
				if (groups_self.size() == 1 && groups_self.front().IsDead() && groups_self.front().CountOfGrid() == 1)
				{
					groups_self.front().FirstGrid(&x_self, &y_self);
				}
				else
				{
					break;
				}
			}

			if (x != x_self || y != y_self) { break; }

			if (x_ko) { *x_ko = x_opp; }

			if (y_ko) { *y_ko = y_opp; }

			ko = true;
		} while (false);

		return ko;
	}
	template<typename GoBoard>
	bool IsKoPoint(const GoBoard& go_board, const int x, const int y, int color) const
	{
		bool ko = false;

		do 
		{
			MyPieces pieces;
			go_board.ChessIds(pieces);

			if (!IsUnderlyingKoPoint(pieces, x, y, color)) { break; }

			auto grids_captured = go_board.LastGridsTaken();

			if (std::distance(std::cbegin(grids_captured), std::cend(grids_captured)) != 1) { break; }

			if (std::cbegin(grids_captured)->X() != x || std::cbegin(grids_captured)->Y() != y) { break; }

			ko = true;
		} while (false);
		
		return ko;
	}

	/***
	*get the group that contains the specified grid
	*pieces: all current pieces in board
	*x:
	*y:
	*/
	MyGroup GroupWithSpecifiedGrid(const MyPieces& pieces, const int x, const int y) const
	{
		MyGroup group(pieces[x][y]);
		group.TryToAddRecursively(pieces, x, y);
		group.CalcLiberty(pieces);

#if (PRINT_RULE_LOG == 1)
		std::stringstream ss;
		group.Output(ss);
		LOG_NINFO("\n{0}") << ss.str();
#endif

		return group;
	}
	template<typename GoBoard>
	MyGroup GroupWithSpecifiedGrid(const GoBoard& go_board, const int x, const int y) const
	{
		MyPieces pieces;
		go_board.ChessIds(pieces);
		return GroupWithSpecifiedGrid(pieces, x, y, color);
	}

	/***
	*get the groups that are adjacent to the specified grid, 
	*maybe including the specified grid if existing piece at there and color is same or -1
	*pieces: all current pieces in board
	*x:
	*y:
	*color: group color to get; -1: get both black and white groups; Black: get black groups; White: get white groups
	*/
	MyGroups GroupsAdjacentToSpecifiedGrid(const MyPieces& pieces, const int x, const int y, const int color = -1) const
	{
		MyGroups groups;

		const int delta[4][2] = { { -1, 0 }, { +1, 0 }, { 0, +1 }, { 0, -1 } }; // left, right, up, down
		int x_tmp, y_tmp;
		for (int i = 0; i < 4; ++i)
		{
			x_tmp = x + delta[i][0];
			y_tmp = y + delta[i][1];

			if (MyGroup::IsGridValid(x_tmp, y_tmp) &&
				pieces[x_tmp][y_tmp] != Empty &&
				(pieces[x_tmp][y_tmp] == color || color == -1) &&
				!groups.HasGrid(x_tmp, y_tmp))
			{
				groups.emplace_back(GroupWithSpecifiedGrid(pieces, x_tmp, y_tmp));
			}
		}

		return groups;
	}
	template<typename GoBoard>
	MyGroups GroupsAdjacentToSpecifiedGrid(const GoBoard& go_board, const int x, const int y, const int color = -1) const
	{
		MyPieces pieces;
		go_board.ChessIds(pieces);
		return GroupsAdjacentToSpecifiedGrid(pieces, x, y, color);
	}

	/***
	*get the dead groups that are adjacent to the specified grid,
	*maybe including the specified grid if existing piece at there and color is same or -1
	*pieces: all current pieces in board
	*x:
	*y:
	*color: group color to get; -1: get both black and white groups; Black: get black groups; White: get white groups
	*/
	MyGroups DeadGroupsAdjacentToSpecifiedGrid(const MyPieces& pieces, const int x, const int y, const int color = -1) const
	{
		MyGroups all_groups = GroupsAdjacentToSpecifiedGrid(pieces, x, y, color);

		MyGroups dead_groups;
		for (const auto& group : all_groups)
		{
			if (group.IsDead())
			{
				dead_groups.emplace_back(group);
			}
		}

		return dead_groups;
	}
	template<typename GoBoard>
	MyGroups DeadGroupsAdjacentToSpecifiedGrid(const GoBoard& go_board, const int x, const int y, const int color = -1) const
	{
		MyPieces pieces;
		go_board.ChessIds(pieces);
		return DeadGroupsAdjacentToSpecifiedGrid(pieces, x, y, color);
	}

	/***
	*get groups whose liberty are in range of [lower, upper]
	*pieces: all current pieces in board
	*lower: the lower bound of liberty
	*upper: the upper bound of liberty
	*color: piece color to get; -1: get both black and white; Black: get black; White: get white
	*/
	MyGroups GroupsWithLibertyRange(const MyPieces& pieces, const int lower, const int upper, const int color = -1) const
	{
		MyGroups all_groups;

		for (int x = 0; x < Grid; ++x)
		{
			for (int y = 0; y < Grid; ++y)
			{
				if (pieces[x][y] != Empty && (pieces[x][y] == color || color == -1))
				{
					all_groups.Add(pieces[x][y], x, y);
				}
			}
		}

		all_groups.CalcLiberty(pieces);

#if (PRINT_RULE_LOG == 1)
		std::stringstream ss;
		all_groups.Output(ss);
		LOG_NINFO("\n{0}") << ss.str();
#endif

		MyGroups target_groups;
		for (const auto& group : all_groups)
		{
			const int liberty = group.Liberty();
			if (liberty >= lower && liberty <= upper)
			{
				target_groups.push_back(group);
			}
		}

		return target_groups;
	}
	template<typename GoBoard>
	MyGroup GroupsWithLibertyRange(const GoBoard& go_board, const int lower, const int upper, const int color = -1) const
	{
		MyPieces pieces;
		go_board.ChessIds(pieces);
		return GroupsWithLibertyRange(pieces, x, y, color);
	}

private:
	GoRule()
	{
		MyGroup::Init();
	}

	GoRule(const GoRule&) = delete;
	GoRule& operator=(const GoRule&) = delete;

	struct PieceGuard
	{
		MyPieces& pieces_;
		const int x_;
		const int y_;
		const int id_;
		PieceGuard(MyPieces& pieces, const int x, const int y)
			: pieces_(pieces), x_(x), y_(y), id_(pieces[x][y])
		{}

		~PieceGuard()
		{
			pieces_[x_][y_] = id_;
		}
	};
};

GAME_NAMESPACE_END

#endif