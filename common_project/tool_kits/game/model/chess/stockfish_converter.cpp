#include "stockfish_converter.h"
#include <assert.h>
#include <sstream>
#include "chess_trait.h"
#include "game/model/base/chess/chess_info_matrix.h"

GAME_NAMESPACE_BEGIN

namespace stk_cvter
{
	struct ChessId2Piece
	{
		ChessId		chess_id_;
		Piece		piece_;
	};
	enum { ArrayCount = 16 };
	typedef ChessId2Piece ChessId2PieceArray[ArrayCount];
	typedef ChessId2Piece Piece2ChessIdArray[ArrayCount];

	// array index is ChessId
	static const ChessId2PieceArray& ChessId2Pieces()
	{
		static const ChessId2PieceArray arr =
		{
			{ ChessTrait::IdOfWhitePawn, W_PAWN }, // 0
			{ ChessTrait::IdOfWhiteRook, W_ROOK }, // 1
			{ ChessTrait::IdOfWhiteKnight, W_KNIGHT }, // 2
			{ ChessTrait::IdOfWhiteBishop, W_BISHOP }, // 3
			{ ChessTrait::IdOfWhiteQueen, W_QUEEN }, // 4
			{ ChessTrait::IdOfWhiteKing, W_KING }, // 5
			{ ChessInfo::EmptyId, NO_PIECE }, // 6
			{ ChessInfo::EmptyId, NO_PIECE }, // 7
			{ ChessInfo::EmptyId, NO_PIECE }, // 8
			{ ChessInfo::EmptyId, NO_PIECE }, // 9
			{ ChessTrait::IdOfBlackPawn, B_PAWN }, // 10
			{ ChessTrait::IdOfBlackRook, B_ROOK }, // 11
			{ ChessTrait::IdOfBlackKnight, B_KNIGHT }, // 12
			{ ChessTrait::IdOfBlackBishop, B_BISHOP }, // 13
			{ ChessTrait::IdOfBlackQueen, B_QUEEN }, // 14
			{ ChessTrait::IdOfBlackKing, B_KING }, // 15
		};

		return arr;
	}

	// array index is Piece
	static const Piece2ChessIdArray& Piece2ChessIds()
	{
		static const Piece2ChessIdArray arr =
		{
			{ ChessInfo::EmptyId, NO_PIECE }, // 0
			{ ChessTrait::IdOfWhitePawn, W_PAWN }, // 1
			{ ChessTrait::IdOfWhiteKnight, W_KNIGHT }, // 2
			{ ChessTrait::IdOfWhiteBishop, W_BISHOP }, // 3
			{ ChessTrait::IdOfWhiteRook, W_ROOK }, // 4
			{ ChessTrait::IdOfWhiteQueen, W_QUEEN }, // 5
			{ ChessTrait::IdOfWhiteKing, W_KING }, // 6
			{ ChessInfo::EmptyId, NO_PIECE }, // 7
			{ ChessInfo::EmptyId, NO_PIECE }, // 8
			{ ChessTrait::IdOfBlackPawn, B_PAWN }, // 9
			{ ChessTrait::IdOfBlackKnight, B_KNIGHT }, // 10
			{ ChessTrait::IdOfBlackBishop, B_BISHOP }, // 11
			{ ChessTrait::IdOfBlackRook, B_ROOK }, // 12
			{ ChessTrait::IdOfBlackQueen, B_QUEEN }, // 13
			{ ChessTrait::IdOfBlackKing, B_KING }, // 14
			{ ChessInfo::EmptyId, NO_PIECE }, // 15
		};

		return arr;
	}

	Square GridToSquare(const GridIndex& grid)
	{
		return grid.IsValid() ? (static_cast<File>(grid.X()) | static_cast<Rank>(7 - grid.Y())) : Square::SQ_NONE;
	}

	GridIndex SquareToGrid(Square square)
	{
		return is_ok(square) ? GridIndex(file_of(square), 7 - rank_of(square)) : GridIndex::InvalidGrid;
	}

	Piece ChessIdToPiece(ChessId chess_id)
	{
		if (chess_id >= 0 && chess_id < ArrayCount)
		{
			return ChessId2Pieces()[chess_id].piece_;
		}

		return Piece::NO_PIECE;
	}

	ChessId PieceToChessId(Piece piece)
	{
		if (piece >= 0 && piece < ArrayCount)
		{
			return Piece2ChessIds()[piece].chess_id_;
		}

		return ChessInfo::EmptyId;
	}
}

GAME_NAMESPACE_END