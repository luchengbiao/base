#include "go_game_action_move_into_board.h"
#include "game/model/go/go_game_board.h"
#include "game/model/go/go_trait.h"
#include "game/model/go/rule/go_rule.h"
#include "go_action_error.h"
#include "log/log.h"

GAME_NAMESPACE_BEGIN

IMPLEMENT_GO_GAME_ACTION_RUNTIME_INFO(GoGameActionType::MoveIntoBoard, GoActionMoveIntoBoard)

template<int Grid>
static bool GridIsNoGoPoint(const GoGameBoard* go_board, const GridIndex& grid, const ChessId chess_side)
{
	return GoRule<Grid>::Singleton()->IsNoGoPoint(*go_board, grid.X(), grid.Y(), chess_side);
}

static bool GridIsNoGoPoint(const GoGameBoard* go_board, const GridIndex& grid, const ChessId chess_side)
{
	switch (go_board->ChessRows())
	{
	case 9:
		return GridIsNoGoPoint<9>(go_board, grid, chess_side);

	case 13:
		return GridIsNoGoPoint<13>(go_board, grid, chess_side);
		
	case 19:
		return GridIsNoGoPoint<19>(go_board, grid, chess_side);
		
	default:
		break;
	}

	return false;
}

template<int Grid>
static bool GridIsKoPoint(const GoGameBoard* go_board, const GridIndex& grid, const ChessId chess_side)
{
	return GoRule<Grid>::Singleton()->IsKoPoint(*go_board, grid.X(), grid.Y(), chess_side);
}

static bool GridIsKoPoint(const GoGameBoard* go_board, const GridIndex& grid, const ChessId chess_side)
{
	switch (go_board->ChessRows())
	{
	case 9:
		return GridIsKoPoint<9>(go_board, grid, chess_side);

	case 13:
		return GridIsKoPoint<13>(go_board, grid, chess_side);

	case 19:
		return GridIsKoPoint<19>(go_board, grid, chess_side);

	default:
		break;
	}

	return false;
}

template<int Grid>
static std::vector<GridIndex> GridsOfDeadGroups(const GoGameBoard* go_board, const ChessId chess_side)
{
	std::vector<GridIndex> grids;

	auto groups = GoRule<Grid>::Singleton()->DeadGroups(*go_board, chess_side);
	if (!groups.empty())
	{
		for (const auto& group : groups)
		{
			group.ForeachGrid([&grids](const int x, const int y){
				grids.emplace_back(x, y);
			});
		}
	}

	return grids;
}

static std::vector<GridIndex> GridsOfDeadGroups(const GoGameBoard* go_board, const ChessId chess_side)
{
	switch (go_board->ChessRows())
	{
	case 9:
		return GridsOfDeadGroups<9>(go_board, chess_side);

	case 13:
		return GridsOfDeadGroups<13>(go_board, chess_side);

	case 19:
		return GridsOfDeadGroups<19>(go_board, chess_side);

	default:
		break;
	}

	return std::vector<GridIndex>();
}

template<int Grid>
static std::vector<GridIndex> GridsOfDeadGroupsAdjacentTo(const GoGameBoard* go_board, const GridIndex& grid, const ChessId chess_side)
{
	std::vector<GridIndex> grids;

	auto groups = GoRule<Grid>::Singleton()->DeadGroupsAdjacentToSpecifiedGrid(*go_board, grid.X(), grid.Y(), chess_side);
	if (!groups.empty())
	{
		for (const auto& group : groups)
		{
			group.ForeachGrid([&grids](const int x, const int y){
				grids.emplace_back(x, y);
			});
		}
	}

	return grids;
}

static std::vector<GridIndex> GridsOfDeadGroupsAdjacentTo(const GoGameBoard* go_board, const GridIndex& grid, const ChessId chess_side)
{
	switch (go_board->ChessRows())
	{
	case 9:
		return GridsOfDeadGroupsAdjacentTo<9>(go_board, grid, chess_side);

	case 13:
		return GridsOfDeadGroupsAdjacentTo<13>(go_board, grid, chess_side);

	case 19:
		return GridsOfDeadGroupsAdjacentTo<19>(go_board, grid, chess_side);

	default:
		break;
	}

	return std::vector<GridIndex>();
}

GameActionResult GoActionMoveIntoBoard::DoOnBoard(GoGameBoard* game_board)
{
	GameActionResult result;

	do 
	{
		auto ope_chess_id = this->ChessId();
		auto grid_to = this->GridTo();

		ChessInfoAdapter chess_info_to(game_board->ChessAt(grid_to));

		if (!chess_info_to)
		{
			result.SetErrorInfo(go_actoin_error::ParamErrorOnToGrid);
			break;
		}

		if (chess_info_to.IsNotEmpty())
		{
			result.SetErrorInfo(go_actoin_error::ExistingChessOnToGrid);
			break;
		}

		if (game_board->IsRuleBased())
		{
			if (game_board->NextChessIdToMoveInWithRule() != ope_chess_id)
			{
				result.SetErrorInfo(go_actoin_error::IsNotTheRightSideToMoveWithRule);
				break;
			}

			bool banning = GridIsNoGoPoint(game_board, grid_to, ope_chess_id);
			DBG_WRAPPER(LOG_NINFO("({0}, {1})") << grid_to.X() << grid_to.Y() 
				<< (banning ? std::string(" is a no-go point") : std::string(" is not a no-go point")) 
				<< (ope_chess_id ? std::string(" for white side") : std::string(" for black side")));

			if (banning)
			{
				result.SetErrorInfo(go_actoin_error::ToGridIsANoGoPoint);
				break;
			}

			bool ko = GridIsKoPoint(game_board, grid_to, ope_chess_id);
			DBG_WRAPPER(LOG_NINFO("({0}, {1})") << grid_to.X() << grid_to.Y()
				<< (ko ? std::string(" is a ko point") : std::string(" is not a ko point"))
				<< (ope_chess_id ? std::string(" for white side") : std::string(" for black side")));

			if (ko)
			{
				result.SetErrorInfo(go_actoin_error::ToGridIsKoPoint);
				break;
			}
		}

		{
			chess_info_to.SetId(ope_chess_id);

			result.AddChessVariation({ GridIndex::InvalidGrid, grid_to, chess_info_to, ChessVariation::Reason::MovedIntoBoardActively });

			if (game_board->IsRuleBased() || game_board->IsAutoTakingPieces())
			{
				//auto grids = GridsOfDeadGroups(game_board, GoTrait::IdOfOppositePiece(ope_chess_id));
				auto grids = GridsOfDeadGroupsAdjacentTo(game_board, grid_to, GoTrait::IdOfOppositePiece(ope_chess_id)); // more efficient
				for (const auto& grid : grids)
				{
					ChessInfoAdapter chess_info_captured(game_board->ChessAt(grid));
					chess_info_captured.SetToEmpty();

					result.AddChessVariation({ GridIndex::InvalidGrid, grid, chess_info_captured, ChessVariation::Reason::CapturedPassively });
				}
			}
		}

		result.MakeValid();
	} while (false);

	if (result.IsValid())
	{
		this->AssociateWithActionResult(result);
		game_board->IncreaseHistoryOfChessMovedIn(result);
	}

	return result;
}

void GoActionMoveIntoBoard::UndoOnBoard(GoGameBoard* game_board)
{
	game_board->DecreaseHistoryOfChessMovedIn(*result_ptr_);

	game_board->UndoChessesViaActionResult(*result_ptr_);
}

void GoActionMoveIntoBoard::RedoOnBoard(GoGameBoard* game_board)
{
	game_board->IncreaseHistoryOfChessMovedIn(*result_ptr_);

	game_board->RedoChessesViaActionResult(*result_ptr_);
}

GAME_NAMESPACE_END