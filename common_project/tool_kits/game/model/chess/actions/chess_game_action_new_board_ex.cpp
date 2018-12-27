#include "chess_game_action_new_board_ex.h"
#include "chess_action_error.h"
#include "game/model/base/proto_entities_view/new_game_board_ex_view.h"
#include "game/model/chess/chess_game_board.h"
#include "protocol/command/commands/cmd_teacher_new_game_board_ex.h"
#include "protocol/command/entities/new_game_board_ex.h"

GAME_NAMESPACE_BEGIN

IMPLEMENT_CHESS_GAME_ACTION_RUNTIME_INFO(ChessGameActionType::NewBoardEx, ChessActionNewBoardEx)

GameActionResult ChessActionNewBoardEx::DoOnBoard(ChessGameBoard* game_board)
{
	GameActionResult result;

	do 
	{
		auto cmd_ptr = std::static_pointer_cast<proto::CmdTeacherNewGameBoardEx>(command_ptr_);
		auto cmd_entity = cmd_ptr->EntityOfCommand();

		if (!game_board->CreateNewBoard(*cmd_entity))
		{
			result.SetErrorInfo(chess_actoin_error::ParamErrorInEntityOfNewBoardEx);
			break;
		}

		if (game_board->IsRuleBased())
		{
			NewGameBoardExView view(*cmd_entity);

			ClientType black_client{ ClientType::None };
			ClientType white_client{ ClientType::None };
			view.VSClient(black_client, white_client);

			auto black_player = VSPlayerInfo(black_client, cmd_entity->black_name_, cmd_entity->black_url_);
			auto white_player = VSPlayerInfo(white_client, cmd_entity->white_name_, cmd_entity->white_url_);

			game_board->CurrentRecord()->SetCurrentVSPlayerInfo(black_player, white_player);
		}

		result.MakeValid();
	} while (false);

	if (result.IsValid())
	{
		this->AssociateWithActionResult(result);
	}

	return result;
}

void ChessActionNewBoardEx::UndoOnBoard(ChessGameBoard*)
{}

void ChessActionNewBoardEx::RedoOnBoard(ChessGameBoard*)
{}

GAME_NAMESPACE_END