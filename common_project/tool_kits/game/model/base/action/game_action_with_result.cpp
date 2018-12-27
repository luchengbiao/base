#include "game_action_with_result.h"
#include "game_action_result.h"
#include "game/model/base/board/game_board_base.h"
#include "protocol/command/base/command.h"
#include "log/log.h"

#define PRINT_ERROR_OF_GAME_VARIATION 1

GAME_NAMESPACE_BEGIN

GameActionResult GameActionWithResult::Do(GameBoardBase* game_board)
{
	LOG_NWAR("Do Action: ") << this->ClassName();

	result_ptr_.reset(new GameActionResult(this->DoOnBoard(game_board)));
	this->AssociateWithActionResult(*result_ptr_);

#if (PRINT_ERROR_OF_GAME_VARIATION == 1)
	auto error_var = result_ptr_->GetErrorInfo();
	if (!error_var.empty())
	{
		LOG_NERR("Error Of Action: ") << error_var;
	}
#endif

	return *result_ptr_;
}

void GameActionWithResult::Undo(GameBoardBase* game_board)
{
	if (result_ptr_ && result_ptr_->IsValid())
	{
		this->UndoOnBoard(game_board);
	}
}

void GameActionWithResult::Redo(GameBoardBase* game_board)
{
	if (result_ptr_ && result_ptr_->IsValid())
	{
		this->RedoOnBoard(game_board);
	}
}

proto::CommandType GameActionWithResult::CmdType() const
{
	return command_ptr_ ? command_ptr_->Type() : proto::CommandType::None;
}

void GameActionWithResult::SetCommandPtr(const CommandSharedPtr& command_ptr)
{
	command_ptr_ = command_ptr;
}

CommandSharedPtr GameActionWithResult::GetCommandPtr() const
{
	return command_ptr_;
}

void GameActionWithResult::AssociateWithActionResult(GameActionResult& result)
{
	result.SetActionPtr(shared_from_this());
}

GAME_NAMESPACE_END