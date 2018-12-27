#include "chess_game_action_move_outof_board.h"
#include "chess_action_error.h"
#include "game/model/chess/chess_game_board.h"
#include "game/model/chess/chess_trait.h"

GAME_NAMESPACE_BEGIN

IMPLEMENT_CHESS_GAME_ACTION_RUNTIME_INFO(ChessGameActionType::MoveOutOfBoard, ChessActionMoveOutOfBoard)

GameActionResult ChessActionMoveOutOfBoard::DoOnBoard(ChessGameBoard* game_board)
{
	GameActionResult result;

	do
	{
		if (game_board->IsRuleBased())
		{
			result.SetErrorInfo(chess_actoin_error::CannotMovePieceOutofBoardActivelyWithRule);
			break;
		}

		auto ope_chess_id = this->ChessId();
		auto grid_from = this->GridFrom();

		ChessInfoAdapter chess_info_from(game_board->ChessAt(grid_from));

		if (!chess_info_from)
		{
			result.SetErrorInfo(chess_actoin_error::ParamErrorOnFromGrid);
			break;
		}

		if (chess_info_from.IsEmpty())
		{
			result.SetErrorInfo(chess_actoin_error::NotExistingChessOnFromGrid);
			break;
		}

		if (chess_info_from.GetId() != ope_chess_id)
		{
			result.SetErrorInfo(chess_actoin_error::ChessIdMismatchingOnFromGrid);
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

void ChessActionMoveOutOfBoard::UndoOnBoard(ChessGameBoard* game_board)
{
	game_board->UndoChessesViaActionResult(*result_ptr_);
}

void ChessActionMoveOutOfBoard::RedoOnBoard(ChessGameBoard* game_board)
{
	game_board->RedoChessesViaActionResult(*result_ptr_);
}

GAME_NAMESPACE_END