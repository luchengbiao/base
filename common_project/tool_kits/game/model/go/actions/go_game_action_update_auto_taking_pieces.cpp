#include "go_game_action_update_auto_taking_pieces.h"
#include "common/macros/assert.h"
#include "game/model/base/action/game_action_result_extra.h"
#include "game/model/go/go_game_board.h"
#include "go_action_error.h"

GAME_NAMESPACE_BEGIN

class UpdateAutoTakingPiecesExtra : public ActionResultExtra
{
public:
	UpdateAutoTakingPiecesExtra(bool last_auto_taking_pieces)
		: last_auto_taking_pieces_(last_auto_taking_pieces)
	{}

	inline bool	LastAutoTakingPieces() const { return last_auto_taking_pieces_; }

private:
	bool last_auto_taking_pieces_{ false };
};

IMPLEMENT_GO_GAME_ACTION_RUNTIME_INFO(GoGameActionType::UpdateAutoTakingPieces, GoActionUpdateAutoTakingPieces)

GameActionResult GoActionUpdateAutoTakingPieces::DoOnBoard(GoGameBoard* game_board)
{
	GameActionResult result;

	do
	{
		if (game_board->IsRuleBased())
		{
			result.SetErrorInfo(go_actoin_error::CannotUpdateAutoTakingPiecesWithRule);
			break;
		}

		auto current_record = game_board->CurrentRecord();
		D_ASSERT(current_record, "current_record should not be null");

		result.SetExtraInfo(std::make_shared<UpdateAutoTakingPiecesExtra>(current_record->IsAutoTakingPieces()));

		auto cmd_ptr = this->GetCommandPtr();
		D_ASSERT(cmd_ptr, "cmd_ptr should not be null");

		current_record->SetAutoTakingPieces(cmd_ptr->IntAt(0) == 1);

		result.MakeValid();
	} while (false);

	if (result.IsValid())
	{
		this->AssociateWithActionResult(result);
	}

	return result;
}

void GoActionUpdateAutoTakingPieces::UndoOnBoard(GoGameBoard* game_board)
{
	if (!game_board->IsRuleBased())
	{
		auto update_auto_taking_extra = std::dynamic_pointer_cast<UpdateAutoTakingPiecesExtra>(result_ptr_->GetExtraInfo());
		if (update_auto_taking_extra)
		{
			game_board->CurrentRecord()->SetAutoTakingPieces(update_auto_taking_extra->LastAutoTakingPieces());
		}
	}
}

void GoActionUpdateAutoTakingPieces::RedoOnBoard(GoGameBoard* game_board)
{
	if (!game_board->IsRuleBased())
	{
		game_board->CurrentRecord()->SetAutoTakingPieces(GetCommandPtr()->IntAt(0) == 1);
	}
}

GAME_NAMESPACE_END