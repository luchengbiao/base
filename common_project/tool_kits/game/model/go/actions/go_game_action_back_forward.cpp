#include "go_game_action_back_forward.h"
#include "game/model/go/go_game_board.h"

GAME_NAMESPACE_BEGIN

IMPLEMENT_GO_GAME_ACTION_RUNTIME_INFO(GoGameActionType::BackForward, GoActionBackForward)

GameActionResult GoActionBackForward::DoOnBoard(GoGameBoard* game_board)
{
	GameActionResult result;

	do
	{
		int steps = this->Steps(); // maybe -1, in the case, steps-- will be always true

		if (this->IsBack())
		{
			while (steps-- && game_board->Undo(false));
		}
		else if (this->IsForward())
		{
			while (steps-- && game_board->Redo(false));
		}

		result.MakeValid();
	} while (false);

	if (result.IsValid())
	{
		this->AssociateWithActionResult(result);
	}

	return result;
}

void GoActionBackForward::UndoOnBoard(GoGameBoard*)
{}

void GoActionBackForward::RedoOnBoard(GoGameBoard*)
{}

GAME_NAMESPACE_END