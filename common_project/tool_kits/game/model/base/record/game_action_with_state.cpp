#include "game_action_with_state.h"
#include <algorithm>

GAME_NAMESPACE_BEGIN

GameActionWithStates::GameActionWithStates(const GameActionWithStates& rhs)
: container_(rhs.container_)
, valid_(rhs.valid_)
{}

GameActionWithStatesUniquePtr GameActionWithStates::Clone() const
{
	return GameActionWithStatesUniquePtr(new GameActionWithStates(*this));
}

void GameActionWithStates::CopyFrom(const GameActionWithStates& rhs)
{
	if (&rhs != this)
	{
		D_ASSERT(!in_traversal_, L"container_ cannot be modified when being iterated");

		container_ = rhs.container_; // NOTICE: if in traversal of container_, this assigning will cause collapsion
		valid_ = rhs.valid_;
	}
}

void GameActionWithStates::Cache(const GameActionPtr& action)
{
	container_.emplace_back(action);

	if (action->PropertyOfMakingBoardValid())
	{
		valid_ = true; // valid once, valid forever

		this->RemoveActionsUndone();
	}
}

void GameActionWithStates::RemoveActionsUndone()
{
	container_.erase(std::remove_if(container_.begin(), container_.end(),
		[](const GameActionWithState& action){ return action.State() == GameActionState::Undone; }),
		container_.end());
}

void GameActionWithStates::Reset()
{
	D_ASSERT(!in_traversal_, L"container_ cannot be modified when being iterated");

	container_.clear();
	valid_ = false;
}

unsigned int  GameActionWithStates::StepsToBack() const
{
	return std::count_if(container_.crbegin(), container_.crend(), [](const GameActionWithState& action){ 
		return action.State() == GameActionState::Done && action.Action()->PropertyOfBeingCountedAsOneStepWhenBackForward();
	});
}

unsigned int  GameActionWithStates::StepsToForward() const
{
	return std::count_if(container_.cbegin(), container_.cend(), [](const GameActionWithState& action){
		return action.State() == GameActionState::Undone && action.Action()->PropertyOfBeingCountedAsOneStepWhenBackForward();
	});
}

GAME_NAMESPACE_END