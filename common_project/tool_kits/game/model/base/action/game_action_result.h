#ifndef __GAME_MODEL_BASE_ACTION_GAME_ACTION_RESULT_H__
#define __GAME_MODEL_BASE_ACTION_GAME_ACTION_RESULT_H__
#include <memory>
#include <string>
#include <vector>
#include "game/model/base/chess/chess_variation.h"
#include "protocol/command/base/command_fwd.h"
#include "protocol/command/command_types.h"

GAME_NAMESPACE_BEGIN

class ActionResultExtra;

class GameActionResult
{
	friend class GameActionWithResult;

public:
	typedef std::shared_ptr<GameActionWithResult>	GameActionWithResultPtr;
	typedef std::weak_ptr<GameActionWithResult>		GameActionWithResultWeakPtr;
	typedef std::vector<ChessVariation>				ChessVariationVector;
	typedef std::shared_ptr<ActionResultExtra>		ActionResultExtraPtr;

public:
	GameActionResult() = default;
	GameActionResult(GameActionResult&&);
	GameActionResult& operator=(GameActionResult&&);
	
	void						SetActionPtr(GameActionWithResultWeakPtr weak_ptr);
	GameActionWithResultPtr		GetActionPtr() const;

	bool						BoardIsChangedByAction() const;

	CommandSharedPtr			GetCommandPtr() const;
	proto::CommandType			GetCommandTypeOfAction() const;

	inline bool					IsValid() const { return valid_; }
	inline void					MakeValid() { valid_ = true; }

	inline bool					HasChessVariations() const { return !chess_variations_.empty(); }
	const ChessVariationVector& ChessVariations() const { return chess_variations_; }

	void						AddChessVariation(const ChessVariation& chess_var);

	template<typename Callable>
	void						ForeachChessVariation(Callable&& callable) const;
	template<typename Callable>
	void						ForeachChessVariationWithBreaker(Callable&& callable) const;

	inline std::string			GetErrorInfo() const { return error_info_; }
	inline void					SetErrorInfo(const std::string& error_info) { error_info_ = error_info; }

	inline ActionResultExtraPtr	GetExtraInfo() const { return extra_info_; }
	inline void					SetExtraInfo(const ActionResultExtraPtr& extra_info) { extra_info_ = extra_info; }

private:
	GameActionWithResultWeakPtr	game_action_ptr_;	// hold a weak ref of GameActionWithResult whose result is me

	// used to play animation, generate chess-move history, undo, redo; 
	// NOTICE: the Motivation-Var shall be the first element which may be used when undoing and redoing
	ChessVariationVector		chess_variations_;  

	bool						valid_{ false }; // record whether the result is valid

	std::string					error_info_; // record the possible error info of GameActionWithResult when calculating or generating a result

	ActionResultExtraPtr		extra_info_; // extra info to save some custom data, such as data to recover(backward) a action of ClearBoard
};

template<typename Callable>
void GameActionResult::ForeachChessVariation(Callable&& callable) const
{
	if (!chess_variations_.empty())
	{
		for (const auto& result : chess_variations_) { callable(result); }
	}
}

template<typename Callable>
void GameActionResult::ForeachChessVariationWithBreaker(Callable&& callable) const
{
	if (!chess_variations_.empty())
	{
		for (const auto& result : chess_variations_) 
		{ 
			if (callable(result))
			{
				break;
			}
		}
	}
}

GAME_NAMESPACE_END

#endif