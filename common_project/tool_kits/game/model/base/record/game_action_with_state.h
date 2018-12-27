#ifndef __GAME_MODEL_BASE_RECORD_GAME_ACTION_WITH_STATE_H__
#define __GAME_MODEL_BASE_RECORD_GAME_ACTION_WITH_STATE_H__
#include <algorithm>
#include <deque>
#include <memory>
#include "base/util/value_guard.h"
#include "common/macros/assert.h"
#include "game/model/base/action/game_action.h"

GAME_NAMESPACE_BEGIN

typedef std::shared_ptr<GameAction> GameActionPtr;

enum class GameActionState
{
	Done,
	Undone,
};

// The reason not to put the GameActionState variable inside of GameAction is that many games will REUSE/SHARE a GameAction
// (e.g. when we create a new board with current game or a history game),
// but the GameActionState variable for every GameAction needs to be managed in every each game when undoing and redoing,
// so here we associate GameAction and GameActionState in GameActionWithState.
// And in order to REUSE/SHARE a GameAction in many games, there is a requirement that the GameActionResult cannot be modified after calculated firstly
class GameActionWithState
{
public:
	GameActionWithState(GameActionPtr action)  
		: action_(std::move(action))
	{ D_ASSERT(action_, L"action_ should not be null"); }

	inline const GameActionPtr&	Action() const { return action_; }

	inline GameActionState		State() const { return state_; }
	inline void					SetState(const GameActionState state) { state_ = state; }

	inline bool					IsDone() const { return state_ == GameActionState::Done; }
	inline bool					IsUndone() const { return state_ == GameActionState::Undone; }

private:
	GameActionPtr				action_;
	GameActionState				state_{ GameActionState::Done };
};

class GameActionWithStates;
typedef std::unique_ptr<GameActionWithStates> GameActionWithStatesUniquePtr;

class GameActionWithStates
{
	typedef GameActionWithStatesUniquePtr SelfUniquePtr;
	typedef std::deque<GameActionWithState> MyContainter;

public:
	GameActionWithStates() = default;
	GameActionWithStates(const GameActionWithStates&);

	SelfUniquePtr	Clone() const;

	void			CopyFrom(const GameActionWithStates&);

	void			Cache(const GameActionPtr& action);

	inline bool		IsValid() const { return valid_; }

	unsigned int	StepsToBack() const;
	unsigned int	StepsToForward() const;

	inline int		Count() const { return container_.size(); }
	inline bool		IsEmpty() const { return container_.empty(); }
	inline GameActionWithState& Back() { return container_.back(); }
	inline const GameActionWithState& Back() const { return container_.back(); }

	template<typename Callbale>
	void			ForeachAction(Callbale&& callable);
	template<typename Callbale>
	void			RForeachAction(Callbale&& callable);
	template<typename Callbale>
	void			ForeachActionWithBreaker(Callbale&& callable);
	template<typename Callbale>
	void			RForeachActionWithBreaker(Callbale&& callable);

	void			Reset();

private:
	void			RemoveActionsUndone();

private:
	MyContainter	container_;
	bool			valid_{ false };

	bool			in_traversal_{ false };
};

template<typename Callbale>
void GameActionWithStates::ForeachAction(Callbale&& callable)
{
	if (!container_.empty())
	{
		nbase::ValueGuard<bool> vg(in_traversal_);
		in_traversal_ = true;

		for (auto& action : container_)
		{
			callable(action);
		}
	}
}

template<typename Callbale>
void GameActionWithStates::RForeachAction(Callbale&& callable)
{
	if (!container_.empty())
	{
		nbase::ValueGuard<bool> vg(in_traversal_);
		in_traversal_ = true;

		for (auto it = container_.rbegin(); it != container_.rend(); ++it)
		{
			callable(*it);
		}
	}
}

template<typename Callbale>
void GameActionWithStates::ForeachActionWithBreaker(Callbale&& callable)
{
	if (!container_.empty())
	{
		nbase::ValueGuard<bool> vg(in_traversal_);
		in_traversal_ = true;

		for (auto& action : container_)
		{
			if (callable(action))
			{
				break;
			}
		}
	}
}

template<typename Callbale>
void GameActionWithStates::RForeachActionWithBreaker(Callbale&& callable)
{
	if (!container_.empty())
	{
		nbase::ValueGuard<bool> vg(in_traversal_);
		in_traversal_ = true;

		for (auto it = container_.rbegin(); it != container_.rend(); ++it)
		{
			if (callable(*it))
			{
				break;
			}
		}
	}
}

GAME_NAMESPACE_END

#endif