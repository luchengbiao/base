#include "game_action_result.h"
#include "game_action_with_result.h"
#include "protocol/command/base/command.h"

GAME_NAMESPACE_BEGIN

GameActionResult::GameActionResult(GameActionResult&& rh)
: game_action_ptr_(rh.game_action_ptr_)
, chess_variations_(std::move(rh.chess_variations_))
, valid_(rh.valid_)
, error_info_(std::move(rh.error_info_))
, extra_info_(std::move(rh.extra_info_))
{
	rh.game_action_ptr_.reset();  // weak_ptr has no move assign operator
	rh.valid_ = false;
}

GameActionResult& GameActionResult::operator=(GameActionResult&& rh)
{
	if (&rh != this)
	{
		game_action_ptr_ = rh.game_action_ptr_;
		chess_variations_ = std::move(rh.chess_variations_);
		valid_ = rh.valid_;
		error_info_ = std::move(rh.error_info_);
		extra_info_ = std::move(rh.extra_info_);

		rh.game_action_ptr_.reset();  // weak_ptr has no move assign operator
		rh.valid_ = false;
	}

	return (*this);
}

void GameActionResult::SetActionPtr(GameActionWithResultWeakPtr weak_ptr)
{
	game_action_ptr_ = weak_ptr;
}

GameActionResult::GameActionWithResultPtr GameActionResult::GetActionPtr() const
{
	return game_action_ptr_.lock();
}

bool GameActionResult::BoardIsChangedByAction() const
{
	auto action_ptr = GetActionPtr();

	return action_ptr ? action_ptr->PropertyOfChangingBoard() : false;
}

CommandSharedPtr GameActionResult::GetCommandPtr() const
{
	CommandSharedPtr cmd_ptr;

	const auto action = this->GetActionPtr();
	if (action)
	{
		cmd_ptr = action->GetCommandPtr();
	}

	return cmd_ptr;
}

proto::CommandType GameActionResult::GetCommandTypeOfAction() const
{
	auto cmd_type = proto::CommandType::None;

	do
	{
		const auto action = this->GetActionPtr();
		if (!action) { break; }

		cmd_type = action->CmdType();

	} while (false);

	return cmd_type;
}

void GameActionResult::AddChessVariation(const ChessVariation& chess_var) 
{
	chess_variations_.emplace_back(chess_var); 
}

GAME_NAMESPACE_END