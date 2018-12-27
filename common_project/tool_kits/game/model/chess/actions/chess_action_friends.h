#ifndef __GAME_MODEL_CHESS_ACTIONS_CHESS_ACTION_FRIENDS_H__
#define __GAME_MODEL_CHESS_ACTIONS_CHESS_ACTION_FRIENDS_H__
#include "game/macros.h"

GAME_NAMESPACE_BEGIN

#define CHESS_ACTION_FRIENDS \
	friend class ChessActionMoveOutOfBoard; \
	friend class ChessActionMoveIntoBoard; \
	friend class ChessActionMoveInsideOfBoard; \
	friend class ChessActionClearBoard; \
	friend class ChessActionNewBoard; \
	friend class ChessActionNewBoardEx; \
	friend class ChessActionBackForward; \
	friend class ChessActionSwitchToHistoryBoard; \
	friend class ChessActionImportManual; \

GAME_NAMESPACE_END

#endif