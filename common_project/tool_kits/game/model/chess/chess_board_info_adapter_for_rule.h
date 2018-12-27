#ifndef __GAME_MODEL_CHESS_CHESS_BOARD_INFO_ADAPTER_FOR_RULE_BASE_H__
#define __GAME_MODEL_CHESS_CHESS_BOARD_INFO_ADAPTER_FOR_RULE_BASE_H__
#include <assert.h>
#include <memory>
#include "game/model/chess/chess_trait.h"

GAME_NAMESPACE_BEGIN

struct BoardInfoAdapter
{
	template<typename ChessBoardT>
	BoardInfoAdapter(const ChessBoardT& chess_board)
	{
		chess_board.ChessIds(chesses_);
	}

	inline int RowForwardStepOfColor(const ChessColor color) const
	{
		return ChessTrait::RowForwardStepOfColor(color);
	}

	inline int BottomRowIndexOfColor(const ChessColor color) const
	{
		return ChessTrait::BottomRowIndexOfColor(color);
	}

	inline int SecondRowIndexOfColor(const ChessColor color) const
	{
		return ChessTrait::SecondRowIndexOfColor(color);
	}

	inline int EnpassantRowIndexOfColor(const ChessColor color) const
	{
		return ChessTrait::EnpassantRowIndexOfColor(color);
	}

	ChessId ChessIdAt(const GridIndex& grid) const
	{
		if (grid.IsInSquare(8))
		{
			return chesses_[grid.X()][grid.Y()];
		}
		
		return ChessInfo::EmptyId;
	}

private:
	ChessId  chesses_[8][8];
};

GAME_NAMESPACE_END

#endif