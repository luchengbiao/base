#include "go_game_action_clear_board.h"
#include "game/model/go/go_game_board.h"
#include "go_action_error.h"

GAME_NAMESPACE_BEGIN

IMPLEMENT_GO_GAME_ACTION_RUNTIME_INFO(GoGameActionType::ClearBoard, GoActionClearBoard)

GameActionResult GoActionClearBoard::DoOnBoard(GoGameBoard* game_board)
{
	GameActionResult result;

	do
	{
		if (game_board->IsRuleBased())
		{
			result.SetErrorInfo(go_actoin_error::CannotClearBoardWithRule);
			break;
		}

		result.SetExtraInfo(std::make_shared<ClearBoardExtra>(game_board->current_game_->Snapshot()));

		game_board->ClearBoard();

		result.MakeValid();
	} while (false);

	if (result.IsValid())
	{
		this->AssociateWithActionResult(result);
	}

	return result;
}

void GoActionClearBoard::UndoOnBoard(GoGameBoard* game_board)
{
	if (!game_board->IsRuleBased())
	{
		auto clear_board_extra = std::dynamic_pointer_cast<ClearBoardExtra>(result_ptr_->GetExtraInfo());
		if (clear_board_extra)
		{
			auto snapshot = clear_board_extra->Snapshot();
			if (snapshot)
			{
				game_board->current_game_->CopyFromSnapshot(*snapshot);
			}
		}
	}
}

void GoActionClearBoard::RedoOnBoard(GoGameBoard* game_board)
{
	if (!game_board->IsRuleBased())
	{
		game_board->ClearBoard();
	}
}

GAME_NAMESPACE_END