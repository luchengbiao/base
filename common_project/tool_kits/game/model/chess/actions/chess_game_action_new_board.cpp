#include "chess_game_action_new_board.h"
#include "game/model/chess/chess_game_board.h"

GAME_NAMESPACE_BEGIN

IMPLEMENT_CHESS_GAME_ACTION_RUNTIME_INFO(ChessGameActionType::NewBoard, ChessActionNewBoard)

GameActionResult ChessActionNewBoard::DoOnBoard(ChessGameBoard* game_board)
{
	GameActionResult result;

	game_board->CreateNewBoard(false);

	result.MakeValid();

	this->AssociateWithActionResult(result);

	return result;
}

void ChessActionNewBoard::UndoOnBoard(ChessGameBoard*)
{}

void ChessActionNewBoard::RedoOnBoard(ChessGameBoard*)
{}

GAME_NAMESPACE_END