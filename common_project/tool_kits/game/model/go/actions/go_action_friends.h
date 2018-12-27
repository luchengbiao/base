#ifndef __GAME_MODEL_GO_ACTIONS_GO_ACTION_FRIENDS_H__
#define __GAME_MODEL_GO_ACTIONS_GO_ACTION_FRIENDS_H__
#include "game/macros.h"

GAME_NAMESPACE_BEGIN

#define GO_ACTION_FRIENDS \
	friend class GoActionMoveOutOfBoard; \
	friend class GoActionMoveIntoBoard; \
	friend class GoActionMoveInsideOfBoard; \
	friend class GoActionClearBoard; \
	friend class GoActionNewBoard; \
	friend class GoActionNewBoardEx; \
	friend class GoActionBackForward; \
	friend class GoActionSwitchToHistoryBoard; \
	friend class GoActionImportManual; \
	friend class GoActionUpdateAutoTakingPieces; \

GAME_NAMESPACE_END

#endif