#include "chess_game_board.h"
#include "actions/chess_game_action_move_insideof_board.h"
#include "common/macros/assert.h"
#include "game/model/chess/chess_board_info_adapter_for_rule.h"
#include "game/model/chess/chess_trait.h"
#include "game/model/chess/simple_path_rule/chess_path_rule.h"
#include "game/model/chess/stockfish_converter.h"
#include "game/model/base/action/game_action.h"
#include "game/model/base/proto_entities_view/new_game_board_ex_view.h"
#include "stockfish/adapter/position_adapter.h"

GAME_NAMESPACE_BEGIN

std::unique_ptr<ChessGameBoard> ChessGameBoard::Create(const GameRecordsPtr& all_games_to_be_cached)
{
	auto game_board = new (std::nothrow) ChessGameBoard(all_games_to_be_cached);
	if (game_board)
	{
		game_board->Init();

		return std::unique_ptr<ChessGameBoard>(game_board);
	}

	return std::unique_ptr<ChessGameBoard>();
}

ChessGameBoard::ChessGameBoard(const GameRecordsPtr& all_games_to_be_cached)
: GameBoard(GameBoardType::Chess, all_games_to_be_cached)
{
	this->CreateNewGameWithCustomRecord(std::make_shared<ChessRecord>(false, false));
}

void ChessGameBoard::Init()
{
	this->SetBoardToOpening();
}

void ChessGameBoard::SetBoardToOpening()
{
	this->EmptyAllChesses();

	ChessType chess_types[MyXGrid] =
	{
		ChessType::Rook,
		ChessType::Knight,
		ChessType::Bishop,
		ChessType::Queen,
		ChessType::King,
		ChessType::Bishop,
		ChessType::Knight,
		ChessType::Rook,
	};

	for (int i = 0; i < MyXGrid; ++i)
	{
		ChessAt(i, 0)->SetId(ChessTrait::IdOfBlackType(chess_types[i]));
		ChessAt(i, 1)->SetId(ChessTrait::IdOfBlackPawn);

		ChessAt(i, MyYGrid - 1)->SetId(ChessTrait::IdOfWhiteType(chess_types[i]));
		ChessAt(i, MyYGrid - 2)->SetId(ChessTrait::IdOfWhitePawn);
	}
}

bool ChessGameBoard::IsRuleBased() const 
{ 
	return CurrentRecord()->IsRuleBased();
}

void ChessGameBoard::SetRuleBased(bool rule_based) 
{
	auto current_record = CurrentRecord();

	if (rule_based && !current_record->IsRuleBased() && CurrentActions()->IsEmpty())  // only support false-true change and no action on board yet
	{
		current_record->SetRuleBased(true);
	}
}

void ChessGameBoard::CreateNewBoard(bool rule_based, bool first_hand_is_white_side)
{
	this->CreateNewGameWithCustomRecord(std::make_shared<ChessRecord>(rule_based, first_hand_is_white_side));

	this->SetBoardToOpening();
}

bool ChessGameBoard::CreateNewBoard(const proto::NewGameBoardEx& entity)
{
	bool ret = true;

	NewGameBoardExView view(entity);
	
	D_ASSERT(view.BoardType() == board_type_, L"The board type of NewGameBoardEx should be same with mine");

	if (view.IsCreatingInitialBoard())
	{
		this->CreateNewBoard(view.IsRuleBased(), view.FirstHandIsWhiteSide());
	}
	else if (view.IsCreatingWithCurrentBoard())
	{
		this->CreateNewGameWithCurrentGame();
	}
	else if (view.IsCreatingWithHistoryBoard())
	{
		ret = this->CreateNewGameWithHistoryGame(view.IndexOfHistoryBoard());
	}

	return ret;
}

void ChessGameBoard::ClearBoard()
{
	this->EmptyAllChesses();
}

std::map<GridIndex, bool>  ChessGameBoard::GridsReachableFrom(const GridIndex& grid_from) const
{
	return IsRuleBased() ? GridsReachableFromWithRule(grid_from) : GridsReachableFromWithoutRule(grid_from);
}

bool ChessGameBoard::IsMoveLegalWithRule(const GridIndex& grid_from, const GridIndex& grid_to)
{
	bool legal = false;

	if (IsRuleBased())
	{
		auto rule_board = this->CurrentRuleBoard();
		D_ASSERT(rule_board, L"rule_board should not be null");

		legal = rule_board->IsMoveLegal(stk_cvter::GridToSquare(grid_from), stk_cvter::GridToSquare(grid_to));
	}

	return legal;
}

GridMotion ChessGameBoard::LastGridMotionOfCurrentGame(ChessId* chess_id_moved) const
{
	GridMotion motion;

	const auto& actions = CurrentActions();
	actions->RForeachActionWithBreaker([&](const GameActionWithState& action){
		if (action.IsDone() && action.Action()->PropertyOfMakingBoardValid())
		{
			auto action_with_result = std::static_pointer_cast<GameActionWithResult>(action.Action());
			if (action_with_result->ActionResult() && action_with_result->ActionResult()->IsValid())
			{
				for (const auto& chess_var : action_with_result->ActionResult()->ChessVariations())
				{
					if (chess_var.IsMovedActively())
					{
						motion = chess_var.GetGridMotion();
						if (chess_id_moved) { *chess_id_moved = chess_var.GetChessIdMoved(); }

						break;
					}
				}

				return true;
			}
		}

		return false;
	});

	return motion;
}

bool ChessGameBoard::IsGameOverWithRule(GameOverType* type)
{
	return CurrentRecord()->IsGameOver(type);
}

bool ChessGameBoard::IsCheckingWithRule() const
{
	return CurrentRecord()->IsChecking();
}

std::map<GridIndex, bool>  ChessGameBoard::GridsReachableFromWithoutRule(const GridIndex& grid_from) const
{
	BoardInfoAdapter board_info(*this);
	return chess_path_rule::GridsReachableFrom(board_info, grid_from);
}

std::map<GridIndex, bool>  ChessGameBoard::GridsReachableFromWithRule(const GridIndex& grid_from) const
{
	auto rule_board = this->CurrentRuleBoard();
	D_ASSERT(rule_board, L"rule_board should not be null");

	std::map<GridIndex, bool> grids;
	auto moves = rule_board->CurrentLegalMoves();

	if (!moves.empty())
	{
		auto square_from = stk_cvter::GridToSquare(grid_from);

		for (const auto& move : moves)
		{
			if (move.square_from_ == square_from)
			{
				if (move.is_short_castling_ || move.is_long_castling_)
				{
					grids.emplace(stk_cvter::SquareToGrid(Square(move.square_from_ > move.square_to_ ? (move.square_from_ - 2) : (move.square_from_ + 2))), true);
				}
				else
				{
					grids.emplace(stk_cvter::SquareToGrid(move.square_to_), move.is_captured_);
				}
			}
		}
	}

	return grids;
}

GAME_NAMESPACE_END