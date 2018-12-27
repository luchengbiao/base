#ifndef __GAME_MODEL_BASE_ACTION_GAME_ACTION_BACK_FORWARD_BASE_H__
#define __GAME_MODEL_BASE_ACTION_GAME_ACTION_BACK_FORWARD_BASE_H__
#include <type_traits>
#include "common/macros/assert.h"
#include "game_action_with_result.h"
#include "game/model/base/chess/chess_info.h"
#include "game/model/base/grid/grid_index.h"
#include "protocol/command/base/command.h"

GAME_NAMESPACE_BEGIN

template < typename Base, typename = std::enable_if_t<std::is_base_of<GameActionWithResult, Base>::value>>
class GameActionBackForwardBase : public Base
{
public:
	bool IsBack() const
	{
		D_ASSERT(command_ptr_, L"command_ptr_ should not be null");

		return command_ptr_->IntAt(0) == 1;
	}

	bool IsForward() const
	{
		D_ASSERT(command_ptr_, L"command_ptr_ should not be null");

		return command_ptr_->IntAt(0) == 0;
	}

	int Steps() const
	{
		return command_ptr_->IntAt(1);
	}

	virtual bool PropertyOfNeedingToBeCachedInOneGame() const override { return false; }
};

GAME_NAMESPACE_END

#endif