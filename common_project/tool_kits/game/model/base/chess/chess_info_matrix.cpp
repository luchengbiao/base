#include "chess_info_matrix.h"

GAME_NAMESPACE_BEGIN

ChessInfoMatrix::ChessInfoMatrix(int rows, int cols)
: rows_(rows)
, cols_(cols)
{
	D_ASSERT(rows_ > 0 && cols_ > 0, L"Rows and cols should be greater than 0");

	int count = rows_ * cols_;
	for (int i = 0; i < count; ++i)
	{
		chesses_.emplace_back(new ChessInfo);
	}
}

ChessInfoMatrix::~ChessInfoMatrix()
{
	for (auto ptr : chesses_)
	{
		delete ptr;
	}
}

ChessInfoMatrixUniquePtr ChessInfoMatrix::Clone() const
{
	auto clone = new ChessInfoMatrix(rows_, cols_);

	clone->CopyFrom(*this);

	return ChessInfoMatrixUniquePtr(clone);
}

ChessInfo* ChessInfoMatrix::ChessAt(const GridIndex& grid) const
{ 
	return ChessAt(grid.Y(), grid.X()); 
}

ChessInfo* ChessInfoMatrix::ChessAt(int x, int y) const
{ 
	if (y >= 0 && y < rows_ && x >= 0 && x < cols_)
	{
		return chesses_[IndexByXY(x, y)];
	}
	 
	return nullptr;
}

ChessId ChessInfoMatrix::ChessIdAt(const GridIndex& grid) const	
{ 
	return this->ChessIdAt(grid.Y(), grid.X());
}

ChessId ChessInfoMatrix::ChessIdAt(int x, int y) const
{ 
	auto chess_info = this->ChessAt(x, y);

	return chess_info ? chess_info->GetId() : ChessInfo::EmptyId;
}

void ChessInfoMatrix::CopyFrom(const ChessInfoMatrix& rhs)
{
	if (&rhs != this)
	{
		D_ASSERT(rows_ == rhs.rows_ && cols_ == rhs.cols_, L"Rows and cols should be equal with that being copied from");

		for (int i = 0; i < chesses_.size(); ++i)
		{
			chesses_[i]->SetId(rhs.chesses_[i]->GetId());
		}
	}
}

void ChessInfoMatrix::Reset()
{
	this->SetAllChessesTo(ChessInfo::EmptyId);
}

void ChessInfoMatrix::SetAllChessesTo(ChessId chess_id)
{
	for (auto chess : chesses_)
	{
		chess->SetId(chess_id);
	}
}

GAME_NAMESPACE_END