#include "chess_trait.h"
#include <sstream>
#include "common/macros/assert.h"
#include "game/model/base/chess/chess_info_matrix.h"
#include "game/model/base/grid/grid_index.h"

GAME_NAMESPACE_BEGIN

ChessTrait::Color ChessTrait::ColorById(const ChessId chess_id)
{
	if (ChessIdIsEmpty(chess_id)) { return Color::None; }

	return (chess_id / 10) == 0 ? Color::White : Color::Black;
}

ChessTrait::Type ChessTrait::TypeById(const ChessId chess_id)
{
	if (ChessIdIsEmpty(chess_id)) { return Type::None; }

	return static_cast<Type>(chess_id % 10);
}

ChessId ChessTrait::IdByColorAndType(const Color color, const Type type)
{
	ChessId id = ChessInfo::EmptyId;

	if (Color::None != color && Type::None != type)
	{
		id = static_cast<int>(color)* 10 + static_cast<int>(type);
	}

	return id;
}

#define CHESS_IMAGE(image_name) std::string(":/chess/res/images/chess/" image_name)
#define INDEX_OF_CHESS_ID(chess_id) ((chess_id >= 0 && chess_id <= 15) ? chess_id : 16)

struct ChessImage
{
	std::string			normal_image_;
	std::string			selected_image_;
	std::string			moving_image_;
};

static const ChessImage& ChessImageById(const ChessId chess_id)
{
	static const ChessImage chess_images[17] =
	{
		{ CHESS_IMAGE("pawn_red.png"), CHESS_IMAGE("pawn_red_selected.png"), CHESS_IMAGE("pawn_red_moving.png") }, // WhitePawn: 0
		{ CHESS_IMAGE("rook_red.png"), CHESS_IMAGE("rook_red_selected.png"), CHESS_IMAGE("rook_red_moving.png") }, // WhiteRook: 1
		{ CHESS_IMAGE("knight_red.png"), CHESS_IMAGE("knight_red_selected.png"), CHESS_IMAGE("knight_red_moving.png") }, // WhiteKnight: 2
		{ CHESS_IMAGE("bishop_red.png"), CHESS_IMAGE("bishop_red_selected.png"), CHESS_IMAGE("bishop_red_moving.png") }, // WhiteBishop: 3
		{ CHESS_IMAGE("queen_red.png"), CHESS_IMAGE("queen_red_selected.png"), CHESS_IMAGE("queen_red_moving.png") }, // WhiteQueen: 4
		{ CHESS_IMAGE("king_red.png"), CHESS_IMAGE("king_red_selected.png"), CHESS_IMAGE("king_red_moving.png") }, // WhiteKing: 5
		{/*6*/ }, {/*7*/ }, {/*8*/ }, {/*9*/ },
		{ CHESS_IMAGE("pawn_black.png"), CHESS_IMAGE("pawn_black_selected.png"), CHESS_IMAGE("pawn_black_moving.png") }, // BlackPawn: 10
		{ CHESS_IMAGE("rook_black.png"), CHESS_IMAGE("rook_black_selected.png"), CHESS_IMAGE("rook_black_moving.png") }, // BlackRook: 11
		{ CHESS_IMAGE("knight_black.png"), CHESS_IMAGE("knight_black_selected.png"), CHESS_IMAGE("knight_black_moving.png") }, // BlackKnight: 12
		{ CHESS_IMAGE("bishop_black.png"), CHESS_IMAGE("bishop_black_selected.png"), CHESS_IMAGE("bishop_black_moving.png") }, // BlackBishop: 13
		{ CHESS_IMAGE("queen_black.png"), CHESS_IMAGE("queen_black_selected.png"), CHESS_IMAGE("queen_black_moving.png") }, // BlackQueen: 14
		{ CHESS_IMAGE("king_black.png"), CHESS_IMAGE("king_black_selected.png"), CHESS_IMAGE("king_black_moving.png") }, // BlackKing: 15
		{/*16*/}
	};

	return chess_images[INDEX_OF_CHESS_ID(chess_id)];
}

std::string ChessTrait::MovingImagePathById(const ChessId chess_id)
{
	return ChessImageById(chess_id).moving_image_;
}

std::string ChessTrait::NormalImagePathById(const ChessId chess_id)
{
	return ChessImageById(chess_id).normal_image_;
}

std::string ChessTrait::SelectedImagePathById(const ChessId chess_id)
{
	return ChessImageById(chess_id).selected_image_;
}

struct ChessId2Char
{
	ChessId chess_id_;
	char	c;
};

static char NotationOfChessId(const ChessId chess_id)
{
	static const char chars[17] =
	{
		'P', // WhitePawn: 0
		'R', // WhiteRook: 1
		'N', // WhiteKnight: 2
		'B', // WhiteBishop: 3
		'Q', // WhiteQueen: 4
		'K', // WhiteKing: 5
		' ', ' ', ' ', ' ', // 6789
		'p', // BlackPawn: 10
		'r', // BlackRook: 11
		'n', // BlackKnight: 12
		'b', // BlackBishop: 13
		'q', // BlackQueen: 14
		'k', // BlackKing: 15
		' ', // 16
	};

	return chars[INDEX_OF_CHESS_ID(chess_id)];
}

static std::string NotationOfGrid(const GridIndex& grid)
{
	char ch[2] = { 'a' + grid.X(), '8' - grid.Y()};

	return std::string(ch, 2);
}

std::string ChessTrait::NotationOfMove(const ChessId chess_id, const GridIndex& grid_from, const GridIndex& grid_to)
{
	std::ostringstream ss;

	ss << NotationOfChessId(chess_id) << NotationOfGrid(grid_from) << "-" << NotationOfGrid(grid_to);

	return ss.str();
}

std::string  ChessTrait::NotationOfBoard(const ChessInfoMatrix& matrix)
{
	D_ASSERT(matrix.Rows() == 8 && matrix.Columns() == 8, L"matrix should be 8x8");

	std::ostringstream ss;

	for (int y = 0; y < 8; ++y)
	{
		for (int x = 0; x < 8; ++x)
		{
			auto chess = matrix.ChessAt(x, y);

			if (chess->IsEmpty())
			{
				int emptyCnt = 1;

				for (; x < 7 && matrix.ChessAt(x + 1, y)->IsEmpty(); ++x)
					++emptyCnt;

				ss << emptyCnt;
			}
			else
			{
				ss << NotationOfChessId(chess->GetId());
			}
		}

		if (y < 7)
		{
			ss << '/';
		}
	}

	return ss.str();
}

#undef CHESS_IMAGE
#undef INDEX_OF_CHESS_ID

GAME_NAMESPACE_END