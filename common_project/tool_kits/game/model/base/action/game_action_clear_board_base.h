#ifndef __GAME_MODEL_BASE_ACTION_GAME_ACTION_CLEAR_BOARD_BASE_H__
#define __GAME_MODEL_BASE_ACTION_GAME_ACTION_BACK_FORWARD_BASE_H__
#include <assert.h>
#include <type_traits>
#include "game_action_result_extra.h"
#include "game_action_with_result.h"
#include "game/model/base/record/game_record.h"

GAME_NAMESPACE_BEGIN

class ClearBoardExtra : public ActionResultExtra
{
public:
	ClearBoardExtra(GameSnapshotPtr game_snapshot)
		: game_snapshot_(std::move(game_snapshot))
	{}

	GameSnapshotPtr	Snapshot() const { return game_snapshot_; }

private:
	GameSnapshotPtr game_snapshot_;
};

template < typename Base, typename = std::enable_if_t<std::is_base_of<GameActionWithResult, Base>::value>>
class GameActionClearBoardBase : public Base
{
public:
	virtual bool PropertyOfMakingBoardValid() const override { return true; }
};

GAME_NAMESPACE_END

#endif