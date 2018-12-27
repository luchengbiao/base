#ifndef __GAME_VIEW_GO_BOARD_PAINTER_GO_BOARD_PAINTER_H__
#define __GAME_VIEW_GO_BOARD_PAINTER_GO_BOARD_PAINTER_H__
#include <QPixmap>
#include "game/macros.h"

GAME_NAMESPACE_BEGIN

class GoBoardPainter
{
public:
	static QPixmap CreatePixmap(int grid_count);
};

GAME_NAMESPACE_END

#endif