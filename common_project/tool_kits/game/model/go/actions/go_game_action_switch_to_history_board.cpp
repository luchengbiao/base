#include "go_game_action_switch_to_history_board.h"
#include "game/model/go/go_game_board.h"

GAME_NAMESPACE_BEGIN

IMPLEMENT_GO_GAME_ACTION_RUNTIME_INFO(GoGameActionType::SwitchToHistoryBoard, GoActionSwitchToHistoryBoard)

GameActionResult GoActionSwitchToHistoryBoard::DoOnBoard(GoGameBoard* game_board)
{
	GameActionResult result;

	game_board->SwitchToGame(command_ptr_->IntAt(0));

	result.MakeValid();

	this->AssociateWithActionResult(result);

	return result;
}

void GoActionSwitchToHistoryBoard::UndoOnBoard(GoGameBoard*)
{}

void GoActionSwitchToHistoryBoard::RedoOnBoard(GoGameBoard*)
{}

GAME_NAMESPACE_END