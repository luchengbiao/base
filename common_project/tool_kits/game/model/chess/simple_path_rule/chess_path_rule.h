#ifndef __GAME_MODEL_CHESS_SIMPLE_PATH_RULE_CHESS_RULE_H__
#define __GAME_MODEL_CHESS_SIMPLE_PATH_RULE_CHESS_RULE_H__
#include <map>
#include <vector>
#include "game/model/chess/chess_trait.h"
#include "game/model/chess/simple_path_rule/move_detail.h"

GAME_NAMESPACE_BEGIN

typedef std::vector<ChessMoveDetail> ChessMoveDetailVector;

namespace chess_path_rule
{
	template<typename BoardInfo>
	std::map<GridIndex, bool> GridsReachableFrom(const BoardInfo& board, const GridIndex& grid_from)
	{
		std::map<GridIndex, bool> grids;

		auto chess_moves = ChessMoveDetailsFrom(board, grid_from);
		if (!chess_moves.empty())
		{
			for (const auto& chess_move : chess_moves)
			{
				bool captured = false;
				switch (chess_move.move_type_)
				{
				case ChessMoveType::Capture:
				case ChessMoveType::ShortCastling:
				case ChessMoveType::LongCastling:
				case ChessMoveType::Enpassant:
				case ChessMoveType::CaptureAndPromotion:
					captured = true;
					break;
				}

				grids.emplace(chess_move.grid_to_, captured);
			}
		}

		return grids;
	}

	template<typename BoardInfo>
	ChessMoveDetailVector ChessMoveDetailsFrom(const BoardInfo& board, const GridIndex& grid_from)
	{
		const auto chess_id = board.ChessIdAt(grid_from);
		const auto chess_type = ChessTrait::TypeById(chess_id);
		const auto chess_color = ChessTrait::ColorById(chess_id);

		if (chess_type == ChessType::Pawn)			{ return ChessMoveDetailsFromPawn(board, grid_from, chess_color); }
		else if (chess_type == ChessType::Rook)		{ return ChessMoveDetailsFromRook(board, grid_from, chess_color); }
		else if (chess_type == ChessType::Knight)	{ return ChessMoveDetailsFromKnight(board, grid_from, chess_color); }
		else if (chess_type == ChessType::Bishop)	{ return ChessMoveDetailsFromBishop(board, grid_from, chess_color); }
		else if (chess_type == ChessType::Queen)	{ return ChessMoveDetailsFromQueen(board, grid_from, chess_color); }
		else if (chess_type == ChessType::King)		{ return ChessMoveDetailsFromKing(board, grid_from, chess_color); }

		return ChessMoveDetailVector();
	}

	inline bool IsRowLegal(const int row) { return row >= 0 && row < 8; }
	inline bool IsColLegal(const int col) { return col >= 0 && col < 8; }
	inline bool IsColAndRowLegal(const int col, const int row) { return IsColLegal(col) && IsRowLegal(row); }
	inline bool IsGridLegal(const GridIndex& grid) { return grid.IsInSquare(8); }
	template<typename BoardInfo>
	bool NoChessAtGrid(const BoardInfo& board, const GridIndex& grid) { return ChessIdIsEmpty(board.ChessIdAt(grid)); }

	// return value: 
	// true: grid_to is legal and not existing chess at grid_to
	template<typename BoardInfo>
	bool TestMovingChessToGrid(const BoardInfo& board, 
							   ChessMoveDetailVector& chess_moves, 
							   const ChessId chess_id, 
							   const GridIndex& grid_to, 
							   const ChessMoveType chessActionType = ChessMoveType::None)
	{
		if (IsGridLegal(grid_to))
		{
			if (NoChessAtGrid(board, grid_to))
			{
				chess_moves.emplace_back(chess_id, grid_to, chessActionType == ChessMoveType::None ? ChessMoveType::Normal : chessActionType);

				return true;
			}
			else
			{
				const auto tmp_id = board.ChessIdAt(grid_to);
				if (!ChessTrait::IsSameColor(chess_id, tmp_id))
				{
					chess_moves.emplace_back(chess_id, grid_to, chessActionType == ChessMoveType::None ? ChessMoveType::Capture : chessActionType);
				}
			}
		}

		return false;
	}
	
	template<typename BoardInfo>
	ChessMoveDetailVector ChessMoveDetailsFromPawn(const BoardInfo& board, const GridIndex& grid_from, const ChessColor color)
	{
		const auto chess_id = ChessTrait::IdByColorAndType(color, ChessType::Pawn);

		ChessMoveDetailVector chess_moves;

		const int row_forward_step = board.RowForwardStepOfColor(color);
		const int col_forward_step = 1;
		const int cur_row = grid_from.Y();
		const int cur_col = grid_from.X();
		int tmp_row, tmp_col;
		bool promotion = false;

		// 1: try to forward 1 step
		tmp_row = cur_row + row_forward_step;
		tmp_col = cur_col;
		if (IsColAndRowLegal(tmp_col, tmp_row) 
			&& NoChessAtGrid(board, GridIndex(tmp_col, tmp_row)))
		{
			//judge whether the pawn can promote
			promotion = (tmp_row == board.BottomRowIndexOfColor(color));

			chess_moves.emplace_back(chess_id, GridIndex(tmp_col, tmp_row), promotion ? ChessMoveType::Promotion : ChessMoveType::Normal);
		}

		// 2: try to forward 2 steps
		if (cur_row == board.SecondRowIndexOfColor(color) 
			&& NoChessAtGrid(board, GridIndex(tmp_col, tmp_row)))
		{
			tmp_row = cur_row + row_forward_step * 2;
			tmp_col = cur_col;
			if (IsColAndRowLegal(tmp_col, tmp_row) && NoChessAtGrid(board, GridIndex(tmp_col, tmp_row)))
			{
				chess_moves.emplace_back(chess_id, GridIndex(tmp_col, tmp_row), ChessMoveType::Normal);
			}
		}

		// 3: try to capture front-left and front-right
		{
			GridIndex grids_to_capture[2] = { GridIndex(cur_col - 1, cur_row + row_forward_step), GridIndex(cur_col + 1, cur_row + row_forward_step) };
			for (const auto& grid : grids_to_capture)
			{
				if (IsGridLegal(grid))
				{
					const ChessId tmp_id = board.ChessIdAt(grid);
					if (ChessIdIsNotEmpty(tmp_id)
						&& color != ChessTrait::ColorById(tmp_id))
					{
						//judge whether the pawn can promote
						promotion = (grid.Y() == board.BottomRowIndexOfColor(color));

						chess_moves.emplace_back(chess_id, grid, promotion ? ChessMoveType::CaptureAndPromotion : ChessMoveType::Capture);
					}
				}
			}
		}

		return chess_moves;
	}

	template<typename BoardInfo>
	ChessMoveDetailVector ChessMoveDetailsFromRook(const BoardInfo& board, const GridIndex& grid_from, const ChessColor color)
	{
		const auto chess_id = ChessTrait::IdByColorAndType(color, ChessType::Rook);

		ChessMoveDetailVector chess_moves;

		const int cur_row = grid_from.Y();
		const int cur_col = grid_from.X();

		for (int row = cur_row - 1; row >= 0 && TestMovingChessToGrid(board, chess_moves, chess_id, GridIndex(cur_col, row)); --row);

		for (int row = cur_row + 1; row < 8 && TestMovingChessToGrid(board, chess_moves, chess_id, GridIndex(cur_col, row)); ++row);

		for (int col = cur_col - 1; col >= 0 && TestMovingChessToGrid(board, chess_moves, chess_id, GridIndex(col, cur_row)); --col);

		for (int col = cur_col + 1; col < 8 && TestMovingChessToGrid(board, chess_moves, chess_id, GridIndex(col, cur_row)); ++col);

		return chess_moves;
	}

	template<typename BoardInfo>
	ChessMoveDetailVector ChessMoveDetailsFromKnight(const BoardInfo& board, const GridIndex& grid_from, const ChessColor color)
	{
		const auto chess_id = ChessTrait::IdByColorAndType(color, ChessType::Knight);

		ChessMoveDetailVector chess_moves;

		const int cur_row = grid_from.Y();
		const int cur_col = grid_from.X();

		const int offsets[8][2] = { {-1, -2}, {-1, +2}, 
									{+1, -2}, {+1, +2}, 
									{-2, -1}, {-2, +1}, 
									{+2, -1}, {+2, +1} };

		for (int i = 0; i < 8; ++i)
		{
			TestMovingChessToGrid(board, chess_moves, chess_id, GridIndex(cur_col + offsets[i][1], cur_row + offsets[i][0]));
		}

		return chess_moves;
	}

	template<typename BoardInfo>
	ChessMoveDetailVector ChessMoveDetailsFromBishop(const BoardInfo& board, const GridIndex& grid_from, const ChessColor color)
	{
		const auto chess_id = ChessTrait::IdByColorAndType(color, ChessType::Bishop);

		ChessMoveDetailVector chess_moves;

		const int cur_row = grid_from.Y();
		const int cur_col = grid_from.X();
		int tmp_row, tmp_col;

		const int offsets[4][2] = { { -1, -1 }, { -1, +1 }, { +1, -1 }, { +1, +1 } };

		for (int i = 0; i < 4; ++i)
		{
			tmp_row = cur_row + offsets[i][0];
			tmp_col = cur_col + offsets[i][1];

			while (TestMovingChessToGrid(board, chess_moves, chess_id, GridIndex(tmp_col, tmp_row)))
			{
				tmp_row += offsets[i][0];
				tmp_col += offsets[i][1];
			}
		}

		return chess_moves;
	}

	template<typename BoardInfo>
	ChessMoveDetailVector ChessMoveDetailsFromQueen(const BoardInfo& board, const GridIndex& grid_from, const ChessColor color)
	{
		const auto chess_id = ChessTrait::IdByColorAndType(color, ChessType::Queen);

		ChessMoveDetailVector chess_moves;

		{
			ChessMoveDetailVector chess_moves_rook = ChessMoveDetailsFromRook(board, grid_from, color);
			if (!chess_moves_rook.empty())
			{
				chess_moves.insert(chess_moves.cend(), chess_moves_rook.cbegin(), chess_moves_rook.cend());
			}
		}

		{
			ChessMoveDetailVector chess_moves_bishop = ChessMoveDetailsFromBishop(board, grid_from, color);
			if (!chess_moves_bishop.empty())
			{
				chess_moves.insert(chess_moves.cend(), chess_moves_bishop.cbegin(), chess_moves_bishop.cend());
			}
		}

		for (auto& chess_move : chess_moves)
		{
			chess_move.chess_id_from_ = chess_id;
		}

		return chess_moves;
	}

	template<typename BoardInfo>
	ChessMoveDetailVector ChessMoveDetailsFromKing(const BoardInfo& board, const GridIndex& grid_from, const ChessColor color)
	{
		const auto chess_id = ChessTrait::IdByColorAndType(color, ChessType::King);

		ChessMoveDetailVector chess_moves;

		const int cur_row = grid_from.Y();
		const int cur_col = grid_from.X();
		int tmp_row, tmp_col;

		const int offsets[8][2] = { { -1, -1 }, { -1, +0 }, { -1, +1 },
									{ +0, -1 }, { +0, +1 },
									{ +1, -1 }, { +1, +0 }, { +1, +1 } };

		for (int i = 0; i < 8; ++i)
		{
			TestMovingChessToGrid(board, chess_moves, chess_id, GridIndex(cur_col + offsets[i][1], cur_row + offsets[i][0]));
		}

		// TODO: castling

		return chess_moves;
	}
}

GAME_NAMESPACE_END

#endif