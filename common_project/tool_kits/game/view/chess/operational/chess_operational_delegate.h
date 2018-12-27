#ifndef __GAME_VIEW_CHESS_OPERATIONAL_CHESS_OPERATIONAL_DELEGATE_H__
#define __GAME_VIEW_CHESS_OPERATIONAL_CHESS_OPERATIONAL_DELEGATE_H__
#include <map>
#include <QPixmap>
#include <QRect>
#include "game/model/base/chess/chess_info.h"
#include "game/model/base/grid/grid_index.h"

GAME_NAMESPACE_BEGIN

typedef std::map<GridIndex, QRect> GridRectInBoardMap;
typedef std::map<ChessId, QRect> ChessRectInBasketMap;
typedef std::map<GridIndex, bool> ChessReachableGridMap;

class ChessOperationalDelegate
{
public:
	virtual ~ChessOperationalDelegate() = default;

	virtual bool					BoardIsWithRule() = 0;

	virtual bool					TestToOperateOnBoardNow(const QPoint& global_mouse_pos) = 0;

	virtual bool					TestToOperateOnThisChess(const GridIndex&, ChessId) = 0;

	virtual GridRectInBoardMap		GridsGlobalRectInBoard() = 0;

	virtual ChessRectInBasketMap	ChessesGlobalRectInBasket() = 0;

	virtual QPixmap					MovingPixmapOfChessId(ChessId) = 0;

	virtual QPixmap					SelectedPixmapOfChessId(ChessId) = 0;

	// <Grid, Captured-like>, Captured-like: Captured, Enpassant, Castling.
	virtual ChessReachableGridMap	GridsReachableFrom(const GridIndex& grid_from) = 0;

	virtual bool					GridContainsChessInBoard(const GridIndex& grid_index, ChessId*) = 0;

	virtual void					HideChessInBoardGrid(const GridIndex&) = 0;

	virtual void					ShowChessInBoardGrid(const GridIndex&) = 0;

	virtual void					OnMouseWillMove(const GridIndex&, ChessId) = 0;

	virtual void					HighlightGridInBoard(const GridIndex&) = 0;

	virtual void					UnhighlightGridInBoard(const GridIndex&) = 0;

	virtual bool					TryToMoveChessLocally(ChessId chess_id, const GridIndex& grid_from, const GridIndex& grid_to) = 0;
};

GAME_NAMESPACE_END

#endif