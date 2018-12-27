#include "chess_game_action_switch_to_history_board.h"
#include "game/model/chess/chess_game_board.h"

GAME_NAMESPACE_BEGIN

IMPLEMENT_CHESS_GAME_ACTION_RUNTIME_INFO(ChessGameActionType::SwitchToHistoryBoard, ChessActionSwitchToHistoryBoard)

GameActionResult ChessActionSwitchToHistoryBoard::DoOnBoard(ChessGameBoard* game_board)
{
	GameActionResult result;

	game_board->SwitchToGame(command_ptr_->IntAt(0));

	result.MakeValid();

	this->AssociateWithActionResult(result);

	return result;
}

void ChessActionSwitchToHistoryBoard::UndoOnBoard(ChessGameBoard*)
{}

void ChessActionSwitchToHistoryBoard::RedoOnBoard(ChessGameBoard*)
{}

GAME_NAMESPACE_END