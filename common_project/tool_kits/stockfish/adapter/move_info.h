#ifndef STOCKFISH_ADAPTER_MOVE_INFO_H_INCLUDED
#define STOCKFISH_ADAPTER_MOVE_INFO_H_INCLUDED
#include "stockfish/macros.h"
#include "stockfish/src/types.h"

STK_NAMESPACE_BEGIN

struct MoveDetail
{
	Move	move_{ Move::MOVE_NONE };

	Square	square_from_{ Square::SQ_NONE };
	Square	square_to_{ Square::SQ_NONE };

	bool	is_short_castling_{ false };			// 短易位(王翼易位) O-O/
	bool	is_long_castling_{ false };				// 长易位(后翼易位) O-O-O/

	bool	is_captured_{ false };					// 吃子 x/
	Square	square_captured_{ Square::SQ_NONE };	// 被吃子的格子，吃过路兵时特殊/

	bool	is_promotion_{ false };					// 升变 =/
	Piece	piece_to_promote_{ Piece::NO_PIECE };	// 升变后变成的棋子/

	bool	is_check_{ false };						// 将军 +/
	bool	is_checkmate_{ false };					// 将死 #/

	inline bool	IsValid() const { return move_ != Move::MOVE_NONE; }
	explicit operator bool() const { return IsValid(); }
};

STK_NAMESPACE_END

#endif