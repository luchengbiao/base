#ifndef __GAME_MODEL_BASE_CHESS_CHESS_VARIATION_H__
#define __GAME_MODEL_BASE_CHESS_CHESS_VARIATION_H__
#include "game/model/base/chess/chess_info.h"
#include "game/model/base/grid/grid_motion.h"

GAME_NAMESPACE_BEGIN

class ChessVariation
{
public:
	enum class Reason
	{
		None,

		// GridIndexRelative(Invalid) -> GridIndex(Valid)
		MovedIntoBoardActively,		

		// GridIndex(Valid) -> GridIndexRelative(Valid)
		MovedInsideOfBoardActively, 

		// GridIndex(Valid) -> GridIndexRelative(Invalid)
		MovedOutOfBoardActively,	

		// GridIndex(Valid) -> GridIndexRelative(Invalid)
		CapturedPassively,
	};

public:
	template<typename ChessIdVariationT>
	ChessVariation(const GridIndex& grid_index_relative, const GridIndex& grid_index, const ChessIdVariationT& chess_id_var, Reason reason)
		: ChessVariation(grid_index_relative, grid_index, chess_id_var.GetLastId(), chess_id_var.GetId(), reason)
	{}

	ChessVariation(const GridIndex& grid_index_relative, const GridIndex& grid_index, ChessId old_chess_id, ChessId new_chess_id, Reason reason)
		: grid_index_relative_(grid_index_relative), grid_index_(grid_index), old_chess_id_(old_chess_id), new_chess_id_(new_chess_id), reason_(reason)
	{}

	inline GridIndex	GetGridIndexRelative() const { return grid_index_relative_; }
	inline GridIndex	GetGridIndex() const { return grid_index_; }

	inline ChessId		GetOldChessId() const { return old_chess_id_; }
	inline ChessId		GetNewChessId() const { return new_chess_id_; }

	inline Reason		GetReason() const { return reason_; }
	inline bool			IsMovedIntoBoardActively() const { return Reason::MovedIntoBoardActively == reason_; }
	inline bool			IsMovedInsideOfBoardActively() const { return Reason::MovedInsideOfBoardActively == reason_; }
	inline bool			IsMovedOutOfBoardActively() const { return Reason::MovedOutOfBoardActively == reason_; }
	inline bool			IsCapturedPassively() const { return Reason::CapturedPassively == reason_; }
	inline bool			IsMovedActively() const { return IsMovedIntoBoardActively() || IsMovedInsideOfBoardActively() || IsMovedOutOfBoardActively(); }
	
	GridMotion			GetGridMotion() const;
	ChessId				GetChessIdMoved() const;

private:
	GridIndex			grid_index_relative_;
	GridIndex			grid_index_;

	ChessId				old_chess_id_;
	ChessId				new_chess_id_;

	Reason				reason_{ Reason::None };
};

GAME_NAMESPACE_END

#endif