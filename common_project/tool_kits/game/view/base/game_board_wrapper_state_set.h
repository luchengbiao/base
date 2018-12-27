#ifndef __GAME_VIEW_BASE_GAME_BOARD_WRAPPER_STATE_SET_H__
#define __GAME_VIEW_BASE_GAME_BOARD_WRAPPER_STATE_SET_H__
#include "game/macros.h"

GAME_NAMESPACE_BEGIN

struct StateSet
{
	bool		game_board_enabled_{ true };
	bool		in_recovery_{ false };

	inline bool IsGameBoardEnabled() const { return game_board_enabled_; }
	inline bool IsGameBoardDisabled() const { return !game_board_enabled_; }
	inline void SetGameBoardEnabled(bool value) { game_board_enabled_ = value; }

	inline bool IsInRecovery() const { return in_recovery_; }
	inline bool IsNotInRecovery() const { return !in_recovery_; }
	inline void SetInRecovery(bool value) { in_recovery_ = value; }
};

GAME_NAMESPACE_END

#endif