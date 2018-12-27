#include "go_record.h"
#include "game/model/base/chess/chess_info_matrix.h"
#include "game/model/go/actions/go_game_action_move_into_board.h"
#include "go_trait.h"

GAME_NAMESPACE_BEGIN

GoMoveRecord::GoMoveRecord(GoMoveRecord&& rh)
: chess_id_last_moved_in_(rh.chess_id_last_moved_in_)
, grid_last_moved_in_(rh.grid_last_moved_in_)
, chess_id_last_taken_(rh.chess_id_last_taken_)
, grids_last_taken_(std::move(rh.grids_last_taken_))
, old_index_of_last_chess_moved_in_(rh.old_index_of_last_chess_moved_in_)
, new_index_of_last_chess_moved_in_(rh.new_index_of_last_chess_moved_in_)
{
	rh.chess_id_last_moved_in_ = ChessInfo::EmptyId;
	rh.grid_last_moved_in_ = GridIndex::InvalidGrid;
	rh.chess_id_last_taken_ = ChessInfo::EmptyId;
}

GoMoveRecord& GoMoveRecord::operator=(GoMoveRecord&& rhs)
{
	if (&rhs != this)
	{
		chess_id_last_moved_in_ = rhs.chess_id_last_moved_in_;
		grid_last_moved_in_ = rhs.grid_last_moved_in_;
		chess_id_last_taken_ = rhs.chess_id_last_taken_;
		grids_last_taken_ = std::move(rhs.grids_last_taken_);
		old_index_of_last_chess_moved_in_ = rhs.old_index_of_last_chess_moved_in_;
		new_index_of_last_chess_moved_in_ = rhs.new_index_of_last_chess_moved_in_;
	}

	return (*this);
}

GoRecord::GoRecord(int grid, bool rule_based, bool auto_taking_pieces, bool first_hand_is_white_side)
: MyBase(rule_based)
, index_matrix_(new ChessInfoMatrix(grid, grid))
, max_index_(0)
, auto_taking_pieces_(auto_taking_pieces)
, first_hand_is_white_side_(first_hand_is_white_side)
{
	index_matrix_->SetAllChessesTo(0);
}

GoRecord::GoRecord(const GoRecord& rhs)
: MyBase(rhs.rule_based_)
, auto_taking_pieces_(rhs.auto_taking_pieces_)
, index_matrix_(rhs.index_matrix_->Clone())
, max_index_(rhs.max_index_)
, first_hand_is_white_side_(rhs.first_hand_is_white_side_)
, go_moves_(rhs.go_moves_)
{}

void GoRecord::SetAutoTakingPieces(bool auto_taking_pieces)
{
	if (!IsRuleBased())
	{
		auto_taking_pieces_ = auto_taking_pieces;
	}
}

CustomRecordUniquePtr GoRecord::Clone() const
{
	return CustomRecordUniquePtr(new GoRecord(*this));
}

void GoRecord::CopyFrom(const CustomRecord& rhs)
{
	this->CopyFrom(dynamic_cast<const GoRecord&>(rhs));
}

void GoRecord::CopyFrom(const GoRecord& rhs)
{
	if (&rhs != this)
	{
		MyBase::CopyFrom(rhs);

		index_matrix_->CopyFrom(*rhs.index_matrix_);
		max_index_ = rhs.max_index_;
		auto_taking_pieces_ = rhs.auto_taking_pieces_;
		first_hand_is_white_side_ = rhs.first_hand_is_white_side_;
		go_moves_ = rhs.go_moves_;
	}
}

GoRecord& GoRecord::operator=(const GoRecord& rhs)
{
	this->CopyFrom(rhs);

	return (*this);
}

void GoRecord::IncreaseHistoryOfChessMovedIn(const GameActionResult& result)
{
	auto action = std::dynamic_pointer_cast<GoActionMoveIntoBoard>(result.GetActionPtr());
	if (action)
	{
		GoMoveRecord go_move;

		result.ForeachChessVariation([this, &go_move](const ChessVariation& chess_var){
			if (chess_var.IsMovedIntoBoardActively())
			{
				go_move.chess_id_last_moved_in_ = chess_var.GetNewChessId();
				go_move.grid_last_moved_in_ = chess_var.GetGridIndex();

				auto chess = index_matrix_->ChessAt(go_move.grid_last_moved_in_);
				D_ASSERT(chess, "chess should not be null");

				go_move.old_index_of_last_chess_moved_in_ = chess->GetId();
				go_move.new_index_of_last_chess_moved_in_ = ++max_index_;

				chess->SetId(go_move.new_index_of_last_chess_moved_in_);
			}
			else if (chess_var.IsCapturedPassively() && ChessIdIsEmpty(chess_var.GetNewChessId()) && ChessIdIsNotEmpty(chess_var.GetOldChessId()))
			{
				go_move.chess_id_last_taken_ = chess_var.GetOldChessId();
				go_move.grids_last_taken_.push_back(chess_var.GetGridIndex());
			}
		});

		go_moves_.emplace_back(std::move(go_move));
	}
}

void GoRecord::DecreaseHistoryOfChessMovedIn(const GameActionResult& result)
{
	auto action = std::dynamic_pointer_cast<GoActionMoveIntoBoard>(result.GetActionPtr());
	if (action)
	{
		if (!go_moves_.empty())
		{
			auto go_move = go_moves_.back();

			auto chess = index_matrix_->ChessAt(go_move.grid_last_moved_in_);
			D_ASSERT(chess, "chess should not be null");

			chess->SetId(go_move.old_index_of_last_chess_moved_in_);
			--max_index_;

			go_moves_.pop_back();
		}
	}
}

void GoRecord::ClearHistoryOfChessMovedIn()
{
	index_matrix_->SetAllChessesTo(0);
	max_index_ = 0;
	go_moves_.clear();
}

ChessId GoRecord::LastChessIdMovedIn() const
{
	return go_moves_.empty() ? ChessInfo::EmptyId : go_moves_.back().chess_id_last_moved_in_;
}

GridIndexVector GoRecord::LastGridsTaken(ChessId* chess_id_taken) const
{
	if (!go_moves_.empty())
	{
		if (chess_id_taken) { *chess_id_taken = go_moves_.back().chess_id_last_taken_; }

		return go_moves_.back().grids_last_taken_;
	}

	return GridIndexVector();
}

ChessId GoRecord::NextChessIdToMoveInWithRule() const
{
	ChessId chess_id = ChessInfo::EmptyId;

	if (IsRuleBased())
	{
		if (!go_moves_.empty())
		{
			chess_id = GoTrait::IdOfOppositePiece(go_moves_.back().chess_id_last_moved_in_);
		}
		else
		{
			chess_id = first_hand_is_white_side_ ? GoTrait::IdOfWhitePiece : GoTrait::IdOfBlackPiece;
		}
	}

	return chess_id;
}

int GoRecord::IndexOfChessAt(const GridIndex& grid) const
{
	auto chess = index_matrix_->ChessAt(grid);
	D_ASSERT(chess, "chess should not be null");

	return chess->GetId();
}

int GoRecord::MaxIndexOfChess() const
{
	return max_index_;
}

void GoRecord::SetRuleBased(bool rule_based)
{
	rule_based_ = rule_based;
	if (rule_based_)
	{
		auto_taking_pieces_ = false;
	}
}

PlayerSide GoRecord::GetCurrentSide()
{
	PlayerSide side = PlayerSide::None;

	if (IsRuleBased())
	{
		if (go_moves_.empty())
		{
			side = first_hand_is_white_side_ ? PlayerSide::White : PlayerSide::Black;
		}
		else
		{
			side = (LastChessIdMovedIn() == GoTrait::IdOfBlackPiece) ? PlayerSide::White : PlayerSide::Black;
		}
	}

	return side;
}

void GoRecord::Reset()
{
	MyBase::Reset();

	index_matrix_->SetAllChessesTo(0);
	max_index_ = 0;
	auto_taking_pieces_ = false;
	first_hand_is_white_side_ = true;
	go_moves_.clear();
}

GAME_NAMESPACE_END