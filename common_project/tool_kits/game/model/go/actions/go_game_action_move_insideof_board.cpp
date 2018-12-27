#include "go_game_action_move_insideof_board.h"
#include "game/model/go/go_game_board.h"
#include "game/model/go/go_trait.h"
#include "go_action_error.h"

GAME_NAMESPACE_BEGIN

IMPLEMENT_GO_GAME_ACTION_RUNTIME_INFO(GoGameActionType::MoveInsideOfBoard, GoActionMoveInsideOfBoard)

GameActionResult GoActionMoveInsideOfBoard::DoOnBoard(GoGameBoard* game_board)
{
	GameActionResult result;

	do 
	{
		if (game_board->IsRuleBased())
		{
			result.SetErrorInfo(go_actoin_error::CannotMovePieceInsideofBoardActivelyWithRule);
			break;
		}

		auto ope_chess_id = this->ChessId();
		auto grid_from = this->GridFrom();
		auto grid_to = this->GridTo();

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

		{
			chess_info_from.SetToEmpty();
			chess_info_to.SetId(ope_chess_id);

			result.AddChessVariation({ grid_to, grid_from, chess_info_from, ChessVariation::Reason::MovedInsideOfBoardActively });
			result.AddChessVariation({ GridIndex::InvalidGrid, grid_to, chess_info_to, ChessVariation::Reason::CapturedPassively });
		}

		result.MakeValid();
	} while (false);

	if (result.IsValid())
	{
		this->AssociateWithActionResult(result);
	}

	return result;
}

void GoActionMoveInsideOfBoard::UndoOnBoard(GoGameBoard* game_board)
{
	game_board->UndoChessesViaActionResult(*result_ptr_);
}

void GoActionMoveInsideOfBoard::RedoOnBoard(GoGameBoard* game_board)
{
	game_board->RedoChessesViaActionResult(*result_ptr_);
}

GAME_NAMESPACE_END