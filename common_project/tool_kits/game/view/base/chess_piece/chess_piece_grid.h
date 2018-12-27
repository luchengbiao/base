#ifndef __GAME_VIEW_BASE_CHESS_PIECE_CHESS_PIECE_GRID_H__
#define __GAME_VIEW_BASE_CHESS_PIECE_CHESS_PIECE_GRID_H__
#include "game/model/base/grid/grid_index.h"

GAME_NAMESPACE_BEGIN

class ChessPieceGrid
{
public:
	ChessPieceGrid() = default;

	inline void			SetGridIndex(int x, int y) { grid_index_.SetXY(x, y); }
	inline void			SetGridIndex(const GridIndex& grid_index) { grid_index_ = grid_index; }
	inline GridIndex	GetGridIndex() const { return grid_index_; }

private:
	GridIndex			grid_index_;
};

GAME_NAMESPACE_END

#endif