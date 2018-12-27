#ifndef __GAME_MODEL_CHESS_CHESS_TRAIT_H__
#define __GAME_MODEL_CHESS_CHESS_TRAIT_H__
#include <string>
#include "game/model/base/chess/chess_info_adapter.h"
#include "game/model/chess/chess_enum.h"

GAME_NAMESPACE_BEGIN

class ChessInfoMatrix;
class GridIndex;

struct ChessTrait
{
	using Color = ChessColor;
	using Type = ChessType;

	static Color ColorById(const ChessId chess_id);

	static Type TypeById(const ChessId chess_id);

	static ChessId IdByColorAndType(const Color color, const Type type);

	inline static const ChessId IdOfWhiteType(const Type type) { return IdByColorAndType(Color::White, type); }
	inline static const ChessId IdOfBlackType(const Type type) { return IdByColorAndType(Color::Black, type); }

	static const ChessId IdOfEmpty			= -1;
	static const ChessId IdOfWhitePawn		= 0;
	static const ChessId IdOfWhiteRook		= 1;
	static const ChessId IdOfWhiteKnight	= 2;
	static const ChessId IdOfWhiteBishop	= 3;
	static const ChessId IdOfWhiteQueen		= 4;
	static const ChessId IdOfWhiteKing		= 5;
	static const ChessId IdOfBlackPawn		= 10;
	static const ChessId IdOfBlackRook		= 11;
	static const ChessId IdOfBlackKnight	= 12;
	static const ChessId IdOfBlackBishop	= 13;
	static const ChessId IdOfBlackQueen		= 14;
	static const ChessId IdOfBlackKing		= 15;

	inline static bool IsSameColor(const ChessInfoAdapter& chess_lh, const ChessInfoAdapter& chess_rh) { return IsSameColor(chess_lh.GetId(), chess_rh.GetId()); }
	inline static bool IsSameColor(const ChessInfo* chess_lh, const ChessInfo* chess_rh) { return IsSameColor(chess_lh->GetId(), chess_rh->GetId()); }
	inline static bool IsSameColor(const ChessId chess_id_lh, const ChessId chess_id_rh) { return ColorById(chess_id_lh) == ColorById(chess_id_rh); }

	static std::string MovingImagePathById(const ChessId chess_id);
	static std::string NormalImagePathById(const ChessId chess_id);
	static std::string SelectedImagePathById(const ChessId chess_id);

	static std::string NotationOfMove(const ChessId chess_id, const GridIndex& grid_from, const GridIndex& grid_to);
	static std::string NotationOfBoard(const ChessInfoMatrix& matrix);

	inline static int RowForwardStepOfColor(const ChessColor color) { return color == ChessColor::White ? -1 : 1; }
	inline static int BottomRowIndexOfColor(const ChessColor color) { return color == ChessColor::White ? 0 : 7; }
	inline static int SecondRowIndexOfColor(const ChessColor color) { return color == ChessColor::White ? 6 : 1; }
	inline static int EnpassantRowIndexOfColor(const ChessColor color) { return color == ChessColor::White ? 4 : 3; }
};

GAME_NAMESPACE_END

#endif