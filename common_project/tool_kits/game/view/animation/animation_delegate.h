#ifndef __GAME_VIEW_ANIMATION_ANIMATION_DELEGATE_H__
#define __GAME_VIEW_ANIMATION_ANIMATION_DELEGATE_H__
#include <QPoint>
#include <QPixmap>
#include "game/model/base/chess/chess_info.h"
#include "game/model/base/grid/grid_index.h"

GAME_NAMESPACE_BEGIN

class UIAnimationDelegate
{
public:
	virtual ~UIAnimationDelegate() = default;

	virtual QPixmap	MovingPixmapOfChessId(ChessId) = 0;

	virtual	QPoint	GlobalPositionOfGridCenterInBoard(const GridIndex&) = 0;

	virtual	QPoint	GlobalPositionOfChessCenterInBasket(ChessId) = 0;

	virtual void	HideChessInBoardGrid(const GridIndex&) = 0;

	virtual int		MilliSecondsOfMovingAnimation() = 0;
};

GAME_NAMESPACE_END

#endif