#ifndef __CHESS_MODEL_BASE_CHESS_CHESS_INFO_MATRIX_H__
#define __CHESS_MODEL_BASE_CHESS_CHESS_INFO_MATRIX_H__
#include <memory>
#include <vector>
#include "common/macros/assert.h"
#include "game/model/base/chess/chess_info.h"
#include "game/model/base/grid/grid_index.h"

GAME_NAMESPACE_BEGIN

class ChessInfoMatrix;
typedef std::unique_ptr<ChessInfoMatrix> ChessInfoMatrixUniquePtr;

class ChessInfoMatrix
{
public:
	ChessInfoMatrix(int rows, int cols);
	~ChessInfoMatrix();

	ChessInfoMatrix(const ChessInfoMatrix&) = delete;
	ChessInfoMatrix& operator=(const ChessInfoMatrix&) = delete;

	ChessInfoMatrixUniquePtr	Clone() const;

	int							Rows() const { return rows_; }
	int							Columns() const { return cols_; }

	ChessInfo*					ChessAt(const GridIndex& grid) const;
	ChessInfo*					ChessAt(int x, int y) const;

	ChessId						ChessIdAt(const GridIndex& grid) const;
	ChessId						ChessIdAt(int x, int y) const;

	void						CopyFrom(const ChessInfoMatrix&);

	template<typename Callable>
	void						ForeachChess(Callable&& callable);

	template<int GridX, int GridY = GridX>
	void						ChessIds(ChessIdArr2D<GridX, GridY>& arr2d) const;

	void						Reset();
	
	void						SetAllChessesTo(ChessId chess_id);

private:
	inline int					IndexByXY(int x, int y) const;
	inline int					XOfIndex(int index) const;
	inline int					YOfIndex(int index) const;

private:
	int							rows_{ 0 }; // row == y
	int							cols_{ 0 }; // col == x
	std::vector<ChessInfo*>		chesses_;
};

template<typename Callable>
void ChessInfoMatrix::ForeachChess(Callable&& callable)
{
	for (auto chess : chesses_)
	{
		callable(chess);
	}
}

template<int GridX, int GridY>
void ChessInfoMatrix::ChessIds(ChessIdArr2D<GridX, GridY>& arr2d) const
{
	D_ASSERT(rows_ >= GridY && cols_ == GridX, L"The size of array is too small");

	int index = -1;
	for (auto chess : chesses_)
	{
		index += 1;

		arr2d[XOfIndex(index)][YOfIndex(index)] = chess->GetId();
	}
}

inline int ChessInfoMatrix::IndexByXY(int x, int y) const
{
	return y * cols_ + x;
}

inline int ChessInfoMatrix::XOfIndex(int index) const
{
	return index % cols_;
}

inline int ChessInfoMatrix::YOfIndex(int index) const
{
	return index / cols_;
}

GAME_NAMESPACE_END

#endif