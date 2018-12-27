#include "chess_game_action_import_manual.h"
#include "chess_action_error.h"
#include "game/model/chess/chess_game_board.h"
#include "game/model/base/proto_entities_view/import_chess_manual_view.h"
#include "game/public/chess_manaul_helper/chess_manual_helper.h"
#include "protocol/command/base/command_group.h"
#include "protocol/command/commands/cmd_teacher_import_chess_manual.h"
#include "protocol/command/entities/import_chess_manual.h"

GAME_NAMESPACE_BEGIN

IMPLEMENT_CHESS_GAME_ACTION_RUNTIME_INFO(ChessGameActionType::ImportChessManual, ChessActionImportManual)

GameActionResult ChessActionImportManual::DoOnBoard(ChessGameBoard* game_board)
{
	GameActionResult result;

	do
	{
		auto cmd_import_chess = std::static_pointer_cast<proto::CmdTeacherImportChessManual>(command_ptr_);
		auto cmd_entity = cmd_import_chess->EntityOfCommand();

		const auto game_record = CHESS_MANAUL_HELPER->GameRecordByUrl(cmd_entity->file_url_);
		if (game_record)
		{
			game_board->CreateNewGameWithClone(game_record);
		}
		else
		{
			// Make sure the chess manual file is downloaded and the command group in CmdTeacherImportChessManual is generated ahead

			auto cmd_groud = cmd_import_chess->GetCommandGroup();
			if (!cmd_groud || cmd_groud->GetCommandCount() == 0)
			{
				result.SetErrorInfo(chess_actoin_error::CmdGroupIsNullOrEmtpyInManualImported);
				break;
			}

			auto cmd_new_board = cmd_import_chess->FindNewBoardCommand();
			if (!cmd_new_board)
			{
				result.SetErrorInfo(chess_actoin_error::NotExistingNewBoardCmdInManualImported);
				break;
			}

			game_board->TryToSaveCurrentGame(); // important

			cmd_groud->Foreach([=](const CommandSharedPtr& cmd_ptr){
				game_board->DoCommand(cmd_ptr, false);
			});

			CHESS_MANAUL_HELPER->SaveGameRecord(cmd_entity->file_url_, game_board->current_game_);
		}

		if (game_board->IsRuleBased())
		{
			ImportChessManualView view(*cmd_entity);

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

void ChessActionImportManual::UndoOnBoard(ChessGameBoard*)
{}

void ChessActionImportManual::RedoOnBoard(ChessGameBoard*)
{}

GAME_NAMESPACE_END