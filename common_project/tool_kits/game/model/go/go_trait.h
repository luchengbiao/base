#ifndef __GAME_MODEL_GO_GO_TRAIT__
#define __GAME_MODEL_GO_GO_TRAIT__
#include <string>
#include <QPoint>
#include "common/macros/assert.h"
#include "go_enum.h"
#include "game/model/base/chess/chess_info_adapter.h"
#include "game/public/enums/game_board_type.h"

GAME_NAMESPACE_BEGIN

#define GO_IMAGE(image_name) std::string(":/go/res/images/go/" image_name)
#define INDEX_OF_GO_CHESS_ID(chess_id) ((chess_id == 0 || chess_id == 1) ? chess_id : 2)

#define GO_GRID_INDICES(name) int name[20] = \
{ \
{ 3 }, { 3 }, { 3 }, { 3 }, { 3 }, { 3 }, { 3 }, { 3 }, { 3 }, \
{ 0 }, \
{ 3 }, { 3 }, { 3 }, \
{ 1 }, \
{ 3 }, { 3 }, { 3 }, { 3 }, { 3 }, \
{ 2 }, \
}

class ChessInfoMatrix;
class GridIndex;

struct GoTrait
{
	using Color = GoColor;
	using Type = GoType;

	static Color ColorById(const ChessId chess_id)
	{
		if (ChessIdIsEmpty(chess_id)) { return Color::None; }

		return chess_id == 1 ? Color::White : Color::Black;
	}

	static Type TypeById(const ChessId chess_id)
	{
		if (ChessIdIsEmpty(chess_id)) { return Type::None; }

		return chess_id == 1 ? Type::White : Type::Black;
	}

	static ChessId IdByColorAndType(const Color color, const Type type)
	{
		ChessId id = ChessInfo::EmptyId;

		if (Type::None != type)
		{
			id = Type::White == type ? 1 : 0;
		}

		return id;
	}

	static const ChessId IdOfEmpty = -1;
	static const ChessId IdOfBlackPiece = 0;
	static const ChessId IdOfWhitePiece = 1;
	inline static const ChessId IdOfOppositePiece(const ChessId this_id) { return this_id == IdOfBlackPiece ? IdOfWhitePiece : IdOfBlackPiece; }

	static bool IsSameColor(const ChessInfoAdapter& chess_lh, const ChessInfoAdapter& chess_rh) { return IsSameColor(chess_lh.GetId(), chess_rh.GetId()); }
	static bool IsSameColor(const ChessInfo* chess_lh, const ChessInfo* chess_rh) { return IsSameColor(chess_lh->GetId(), chess_rh->GetId()); }
	static bool IsSameColor(const ChessId chess_id_lh, const ChessId chess_id_rh) { return ColorById(chess_id_lh) == ColorById(chess_id_rh); }

	struct GoPieceImage
	{
		const std::string	normal_image_;
		const std::string	selected_image_;
		const std::string	moving_image_;
	};

	inline static bool IsGridValid(const int grid)
	{
		return grid == 9 || grid == 13 || grid == 19;
	}

	static const GoPieceImage& GoPieceInfoByIdAndGrid(const ChessId chess_id, const int grid)
	{
		D_ASSERT(IsGridValid(grid), L"Invalid grid count");

		static GO_GRID_INDICES(grid_indices);

		static const GoPieceImage go_pieces[3][3] = {
			{
				{ GO_IMAGE("9_black.png"), GO_IMAGE("9_black_selected.png"), GO_IMAGE("9_black_moving.png") }, //BlackPiece: 0
				{ GO_IMAGE("9_white.png"), GO_IMAGE("9_white_selected.png"), GO_IMAGE("9_white_moving.png") }, //BlackPiece: 1
				{},
			},

			{
				{ GO_IMAGE("13_black.png"), GO_IMAGE("13_black_selected.png"), GO_IMAGE("13_black_moving.png") }, //BlackPiece: 0
				{ GO_IMAGE("13_white.png"), GO_IMAGE("13_white_selected.png"), GO_IMAGE("13_white_moving.png") }, //BlackPiece: 1
				{},
			},

			{
				{ GO_IMAGE("19_black.png"), GO_IMAGE("19_black_selected.png"), GO_IMAGE("19_black_moving.png") }, //BlackPiece: 0
				{ GO_IMAGE("19_white.png"), GO_IMAGE("19_white_selected.png"), GO_IMAGE("19_white_moving.png") }, //BlackPiece: 1
				{},
			},
		};

		return go_pieces[grid_indices[grid]][INDEX_OF_GO_CHESS_ID(chess_id)];
	}

	struct GoBoardInfo
	{
		const GameBoardType	board_type_;
		const QPoint		offset_;
		const int			grid_width_;
		const int			chess_width_;
	};
	static const GoBoardInfo& GoBoardInfoByGrid(const int grid)
	{
		D_ASSERT(IsGridValid(grid), L"Invalid grid count");

		static GO_GRID_INDICES(grid_indices);

		static GoBoardInfo infos[3] =
		{
			{ GameBoardType::Go9,	QPoint(26, 25), 58, 84 }, /*9*/
			{ GameBoardType::Go13,	QPoint(26, 25), 39, 69 }, /*13*/
			{ GameBoardType::Go19,	QPoint(26, 25), 26, 63 }, /*19*/
		};

		return infos[grid_indices[grid]];
	}

	static int GoBoardGridsByType(const GameBoardType board_type)
	{
		int grid = 0;
		
		switch (board_type)
		{
		case GameBoardType::Go9:
			grid = 9;
			break;

		case GameBoardType::Go13:
			grid = 13;
			break;

		case GameBoardType::Go19:
			grid = 19;
			break;

		default:
			break;
		}

		D_ASSERT(IsGridValid(grid), L"Invalid grid count");

		return grid;
	}
};

template<int X>
struct GoTraitX : GoTrait
{
	static inline std::string MovingImagePathById(const ChessId chess_id) { return GoTrait::GoPieceInfoByIdAndGrid(chess_id, X).moving_image_; }
	static inline std::string NormalImagePathById(const ChessId chess_id) { return GoTrait::GoPieceInfoByIdAndGrid(chess_id, X).normal_image_; }
	static inline std::string SelectedImagePathById(const ChessId chess_id) { return GoTrait::GoPieceInfoByIdAndGrid(chess_id, X).selected_image_; }
};

#undef GO_IMAGE
#undef INDEX_OF_GO_CHESS_ID
#undef GO_GRID_INDICES

GAME_NAMESPACE_END

#endif