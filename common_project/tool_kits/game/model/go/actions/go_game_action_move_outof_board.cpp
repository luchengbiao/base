#include "go_game_action_move_outof_board.h"
#include "game/model/go/go_game_board.h"
#include "game/model/go/go_trait.h"
#include "go_action_error.h"

GAME_NAMESPACE_BEGIN

IMPLEMENT_GO_GAME_ACTION_RUNTIME_INFO(GoGameActionType::MoveOutOfBoard, GoActionMoveOutOfBoard)

GameActionResult GoActionMoveOutOfBoard::DoOnBoard(GoGameBoard* game_board)
{
	GameActionResult result;

	do
	{
		if (game_board->IsRuleBased())
		{
			result.SetErrorInfo(go_actoin_error::CannotMovePieceOutofBoardActivelyWithRule);
			break;
		}

		auto ope_chess_id = this->ChessId();
		auto grid_from = this->GridFrom();

		ChessInfoAdapter chess_info_from(game_board->ChessAt(grid_from));

		if (!chess_info_from)
		{
			result.SetErrorInfo(go_actoin_error::ParamErrorOnFromGrid);
			break;
		}

		if (chess_info_from.IsEmpty())
		{
			result.SetErrorInfo(go_actoin_error::NotExistingChessOnFromGrid);
			break;
		}

		if (chess_info_from.GetId() != ope_chess_id)
		{
			result.SetErrorInfo(go_actoin_error::ChessIdMismatchingOnFromGrid);
			break;
		}

		{
			chess_info_from.SetToEmpty();

			result.AddChessVariation({ GridIndex::InvalidGrid, grid_from, chess_info_from, ChessVariation::Reason::MovedOutOfBoardActively });
		}

		result.MakeValid();
	} while (false);

	if (result.IsValid())
	{
		this->AssociateWithActionResult(result);
	}

	return result;
}

void GoActionMoveOutOfBoard::UndoOnBoard(GoGameBoard* game_board)
{
	game_board->UndoChessesViaActionResult(*result_ptr_);
}

void GoActionMoveOutOfBoard::RedoOnBoard(GoGameBoard* game_board)
{
	game_board->RedoChessesViaActionResult(*result_ptr_);
}

GAME_NAMESPACE_END