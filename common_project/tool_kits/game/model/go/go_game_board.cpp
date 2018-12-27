#include "go_game_board.h"
#include "actions/go_game_action_move_into_board.h"
#include "game/model/base/action/game_action.h"
#include "game/model/base/proto_entities_view/new_game_board_ex_view.h"
#include "game/model/go/go_trait.h"

GAME_NAMESPACE_BEGIN

std::unique_ptr<GoGameBoard> GoGameBoard::Create(int grid, const GameRecordsPtr& all_games_to_be_cached)
{
	auto game_board = new (std::nothrow) GoGameBoard(grid, all_games_to_be_cached);
	if (game_board)
	{
		game_board->Init();

		return std::unique_ptr<GoGameBoard>(game_board);
	}

	return std::unique_ptr<GoGameBoard>();
}

GoGameBoard::GoGameBoard(int grid, const GameRecordsPtr& all_games_to_be_cached)
: GameBoardBase(GoTrait::GoBoardInfoByGrid(grid).board_type_, grid, grid, all_games_to_be_cached)
{
	this->CreateNewGameWithCustomRecord(std::make_shared<GoRecord>(grid, false, false));
}

void GoGameBoard::Init()
{
	this->SetBoardToOpening();
}

void GoGameBoard::SetBoardToOpening()
{
	this->EmptyAllChesses();
}

bool GoGameBoard::IsRuleBased() const
{
	return CurrentRecord()->IsRuleBased();
}

void GoGameBoard::SetRuleBased(bool rule_based)
{
	auto current_record = CurrentRecord();

	if (rule_based && !current_record->IsRuleBased() && CurrentActions()->IsEmpty())  // only support false-true change and no action on board yet
	{
		current_record->SetRuleBased(true);
	}
}

bool GoGameBoard::IsAutoTakingPieces() const
{
	return CurrentRecord()->IsAutoTakingPieces();
}

void GoGameBoard::SetAutoTakingPieces(bool auto_taking_pieces)
{
	CurrentRecord()->SetAutoTakingPieces(auto_taking_pieces);
}

void GoGameBoard::IncreaseHistoryOfChessMovedIn(const GameActionResult& result)
{
	CurrentRecord()->IncreaseHistoryOfChessMovedIn(result);
}

void GoGameBoard::DecreaseHistoryOfChessMovedIn(const GameActionResult& result)
{
	CurrentRecord()->DecreaseHistoryOfChessMovedIn(result);
}

ChessId GoGameBoard::LastChessIdMovedIn() const
{
	return CurrentRecord()->LastChessIdMovedIn();
}

GoGameBoard::GridIndexVector GoGameBoard::LastGridsTaken(ChessId* chess_id_taken) const
{
	return CurrentRecord()->LastGridsTaken(chess_id_taken);
}

ChessId  GoGameBoard::NextChessIdToMoveInWithRule() const
{
	return CurrentRecord()->NextChessIdToMoveInWithRule();
}

int GoGameBoard::IndexOfChessAt(const GridIndex& grid) const
{
	return CurrentRecord()->IndexOfChessAt(grid);
}

int GoGameBoard::MaxIndexOfChess() const
{
	return CurrentRecord()->MaxIndexOfChess();
}

void GoGameBoard::CreateNewBoard(bool rule_based, bool auto_taking_pieces)
{
	this->CreateNewGameWithCustomRecord(std::make_shared<GoRecord>(rows_, rule_based, auto_taking_pieces));

	this->SetBoardToOpening();
}

bool GoGameBoard::CreateNewBoard(const proto::NewGameBoardEx& entity)
{
	bool ret = true;

	auto view = NewGameBoardExView(entity);

	D_ASSERT(view.BoardType() == board_type_, L"The board type of NewGameBoardEx should be same with mine");

	if (view.IsCreatingInitialBoard())
	{
		this->CreateNewBoard(view.IsRuleBased(), false);
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

void GoGameBoard::ClearBoard()
{
	this->EmptyAllChesses();

	CurrentRecord()->ClearHistoryOfChessMovedIn();
}

GAME_NAMESPACE_END