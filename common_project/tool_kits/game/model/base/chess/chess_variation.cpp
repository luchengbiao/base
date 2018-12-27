#include "chess_variation.h"

GAME_NAMESPACE_BEGIN

GridMotion ChessVariation::GetGridMotion() const
{
	GridMotion motion;

	switch (reason_)
	{
	case Reason::MovedIntoBoardActively:
		motion.SetFromTo(grid_index_relative_, grid_index_);
		break;

	case Reason::MovedInsideOfBoardActively:
		motion.SetFromTo(grid_index_, grid_index_relative_);
		break;

	case Reason::MovedOutOfBoardActively:
		motion.SetFromTo(grid_index_, grid_index_relative_);
		break;

	case Reason::CapturedPassively:
		motion.SetFromTo(grid_index_, grid_index_relative_);
		break;

	default:
		break;
	}

	return motion;
}

ChessId ChessVariation::GetChessIdMoved() const
{
	auto chess_id = ChessInfo::EmptyId;

	switch (reason_)
	{
	case Reason::MovedIntoBoardActively:
		chess_id = new_chess_id_;
		break;

	case Reason::MovedInsideOfBoardActively:
		chess_id = old_chess_id_;
		break;

	case Reason::MovedOutOfBoardActively:
		chess_id = old_chess_id_;
		break;

	case Reason::CapturedPassively:
		chess_id = old_chess_id_;
		break;

	default:
		break;
	}

	return chess_id;
}

GAME_NAMESPACE_END