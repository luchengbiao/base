#ifndef __GAME_MODEL_CHESS_SIMPLE_PATH_RULE_MOVE_DETAIL_H__
#define __GAME_MODEL_CHESS_SIMPLE_PATH_RULE_MOVE_DETAIL_H__
#include "game/model/base/grid/grid_index.h"
#include "game/model/base/chess/chess_info.h"
#include "game/model/chess/simple_path_rule/move_type.h"

GAME_NAMESPACE_BEGIN

struct ChessMoveDetail
{
	ChessId			chess_id_from_{ ChessInfo::EmptyId };

	GridIndex		grid_to_;

	ChessMoveType	move_type_{ ChessMoveType::None };

	GridIndex		grid_captured_; // 如果是吃子情况，则记录被吃子位置信息，一般情况等于grid_to_，吃过路兵时才有特殊/

	ChessMoveDetail(ChessId chess_id_from, const GridIndex& grid_to, ChessMoveType move_type, const GridIndex& grid_captured = GridIndex::InvalidGrid)
		: chess_id_from_(chess_id_from), grid_to_(grid_to), move_type_(move_type), grid_captured_(grid_captured)
	{}
};

GAME_NAMESPACE_END

#endif