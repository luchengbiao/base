#ifndef __GAME_VIEW_GO_OPERATIONAL_GO_OPERATIONAL_DELEGATE_H__
#define __GAME_VIEW_GO_OPERATIONAL_GO_OPERATIONAL_DELEGATE_H__
#include <map>
#include <QPixmap>
#include <QRect>
#include "game/model/base/chess/chess_info.h"
#include "game/model/base/grid/grid_index.h"

GAME_NAMESPACE_BEGIN

using GridRectInBoardMap = std::map<GridIndex, QRect>;
using ChessRectInBasketMap = std::map<ChessId, QRect>;

class GoOperationalDelegate
{
public:
	virtual ~GoOperationalDelegate() = default;

	virtual bool					TestToOperateOnBoardNow(const QPoint& global_mouse_pos) = 0;

	virtual bool					TestToOperateOnThisChess(const GridIndex&, ChessId) = 0;

	virtual int						GridsOfBoard() = 0;

	virtual GridRectInBoardMap		GridsGlobalRectInBoard() = 0;

	virtual ChessRectInBasketMap	ChessesGlobalRectInBasket() = 0;

	virtual bool					GridContainsChessInBoard(const GridIndex& grid_index, ChessId*) = 0;

	virtual ChessId					NextChessIdToMoveIntoBoard() = 0;

	virtual bool					TryToMoveChessLocally(ChessId chess_id, const GridIndex& grid_from, const GridIndex& grid_to, std::string* failed_info = nullptr) = 0;

	virtual	void					TipOnToast(const QString& tip) = 0;
};

GAME_NAMESPACE_END

#endif