#include "chess_game_action.h"
#include "common/macros/assert.h"
#include "game/model/base/board/game_board_base.h"
#include "game/model/chess/chess_trait.h"
#include "game/model/chess/chess_game_board.h"

GAME_NAMESPACE_BEGIN

ChessGameActionType ChessGameAction::Type() const
{
	return this->GetClassRuntimeInfo().Type();
}

std::string ChessGameAction::ClassName() const
{
	return this->GetClassRuntimeInfo().ClassName();
}

GameActionResult ChessGameAction::DoOnBoard(GameBoardBase* board_base)
{
	auto chess_board = dynamic_cast<ChessGameBoard*>(board_base); // should be ChessGameBoard

	D_ASSERT(chess_board, L"The actual type of board_base should be ChessGameBoard and not be null");

	return this->DoOnBoard(chess_board);
}

void ChessGameAction::UndoOnBoard(GameBoardBase* board_base)
{
	auto chess_board = dynamic_cast<ChessGameBoard*>(board_base); // should be ChessGameBoard

	D_ASSERT(chess_board, L"The actual type of board_base should be ChessGameBoard and not be null");

	this->UndoOnBoard(chess_board);
}

void ChessGameAction::RedoOnBoard(GameBoardBase* board_base)
{
	auto chess_board = dynamic_cast<ChessGameBoard*>(board_base); // should be ChessGameBoard

	D_ASSERT(chess_board, L"The actual type of board_base should be ChessGameBoard and not be null");

	this->RedoOnBoard(chess_board);
}

GAME_NAMESPACE_END