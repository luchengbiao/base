#ifndef __GAME_MODEL_CHESS_STOCKFISH_CONVERTER_H__
#define __GAME_MODEL_CHESS_STOCKFISH_CONVERTER_H__
#include <string>
#include "game/model/base/chess/chess_info.h"
#include "game/model/base/grid/grid_index.h"
#include "stockfish/src/types.h"

GAME_NAMESPACE_BEGIN

class ChessInfoMatrix;

namespace stk_cvter
{
	Square			GridToSquare(const GridIndex& grid);

	GridIndex		SquareToGrid(Square square);

	Piece			ChessIdToPiece(ChessId chess_id);

	ChessId			PieceToChessId(Piece);

	std::string		SanOfPiecePlacement(const ChessInfoMatrix&);
}

GAME_NAMESPACE_END

#endif