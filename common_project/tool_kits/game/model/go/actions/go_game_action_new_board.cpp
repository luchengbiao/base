#include "go_game_action_new_board.h"
#include "game/model/go/go_game_board.h"

GAME_NAMESPACE_BEGIN

IMPLEMENT_GO_GAME_ACTION_RUNTIME_INFO(GoGameActionType::NewBoard, GoActionNewBoard)

GameActionResult GoActionNewBoard::DoOnBoard(GoGameBoard* game_board)
{
	GameActionResult result;

	game_board->CreateNewBoard(false, false);

	result.MakeValid();

	this->AssociateWithActionResult(result);

	return result;
}

void GoActionNewBoard::UndoOnBoard(GoGameBoard*)
{}

void GoActionNewBoard::RedoOnBoard(GoGameBoard*)
{}

GAME_NAMESPACE_END