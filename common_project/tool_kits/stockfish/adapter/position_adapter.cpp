#include "position_adapter.h"
#include "position_adapter_fwd.h"
#include "bitboard.h"
#include "position.h"
#include "thread.h"
#include "ucioption.h"

STK_NAMESPACE_BEGIN

static bool DetailOfMove(Position& pos, Move m, MoveDetail& d);

// FEN string of the initial position, normal chess
static const char* StartWithWhite = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
static const char* StartWithBlack = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1";

PositionAdapter::PositionAdapter()
{
	static bool once_flag = true;
	if (once_flag)
	{
		once_flag = false;

		UCI::init(Options);
		Bitboards::init();
		Position::init();
	}

	position_.reset(new Position);
	thread_.reset(new MainThread);
}

PositionAdapter::PositionAdapter(bool white_is_first)
: PositionAdapter()
{
	starting_FEN_ = white_is_first ? StartWithWhite : StartWithBlack;
	position_->set(starting_FEN_, false, thread_.get());

	this->DirtyCurrentLegalMoves();
}

PositionAdapter::PositionAdapter(const PositionAdapter& rh)
: PositionAdapter()
{
	this->ResetToStarting(rh.starting_FEN_);

	for (std::size_t i = 0; i < rh.moved_stack_.size(); ++i)
	{
		this->DoMove(rh.moved_stack_[i]);
	}
}

void PositionAdapter::CurrentPieces(Piece pieces[SQUARE_NB])
{
	for (auto square = SQ_A1; square <= SQ_H8; ++square)
	{
		pieces[square] = position_->piece_on(square);
	}
}

PositionAdapter& PositionAdapter::operator=(const PositionAdapter& rh)
{
	if (&rh != this)
	{
		bool included = false;

		do 
		{
			if (starting_FEN_ != rh.starting_FEN_) { break; }

			if (rh.moved_stack_.size() < moved_stack_.size()) { break; }

			included = true;
			for (std::size_t i = 0; i < moved_stack_.size(); ++i)
			{
				if (rh.moved_stack_[i].move_ != moved_stack_[i].move_)
				{
					included = false;
					break;
				}
			}
		} while (false);

		if (included)
		{
			for (std::size_t i = moved_stack_.size(); i < rh.moved_stack_.size(); ++i)
			{
				this->DoMove(rh.moved_stack_[i]);
			}
		}
		else
		{
			this->ResetToStarting(rh.starting_FEN_);

			for (std::size_t i = 0; i < rh.moved_stack_.size(); ++i)
			{
				this->DoMove(rh.moved_stack_[i]);
			}
		}
	}

	return *this;
}

PositionAdapter::~PositionAdapter()
{}

MoveDetails PositionAdapter::CurrentLegalMoves()
{
	this->TryToUpdateCurrentLegalMoves();

	return current_legal_moves_;
}

bool PositionAdapter::CurrentSideIsWhite() const
{
	return position_->side_to_move() == Color::WHITE;
}

bool PositionAdapter::IsMoveLegal(const Square square_from, const Square square_to)
{
	this->TryToUpdateCurrentLegalMoves();

	for (const auto& move : current_legal_moves_)
	{
		if (move.square_from_ == square_from && move.square_to_ == square_to)
		{
			return true;
		}
	}

	return false;
}

bool PositionAdapter::IsGameOver(GameOverType* type_out)
{
	auto type = GameOverType::None;

	this->TryToUpdateCurrentLegalMoves();

	if (current_legal_moves_.empty())
	{
		if (!Checkers())
		{
			type = GameOverType::Draw;
		}
		else
		{
			type = position_->side_to_move() == Color::WHITE ? GameOverType::BlackWin : GameOverType::WhiteWin;
		}
	}

	if (type_out) { *type_out = type; }

	return type != GameOverType::None;
}

bool PositionAdapter::Checkers() const
{
	return position_->checkers();
}

MoveDetail PositionAdapter::GetBestMove(const Square square_from, const Square square_to)
{
	this->TryToUpdateCurrentLegalMoves();

	MoveDetail best_move;

	for (const auto& move : current_legal_moves_)
	{
		if (move.square_from_ == square_from && move.square_to_ == square_to)
		{
			if (best_move.move_ == MOVE_NONE || (move.is_promotion_ && type_of(move.piece_to_promote_) == QUEEN))
			{
				best_move = move;
			}
		}
	}

	return best_move;
}

bool PositionAdapter::CanMoveToPromote(const Square square_from, const Square square_to, const Piece piece_to_promote, MoveDetail* move_detail)
{
	this->TryToUpdateCurrentLegalMoves();

	for (const auto& move : current_legal_moves_)
	{
		if (move.square_from_ == square_from && move.square_to_ == square_to && 
			move.is_promotion_ && move.piece_to_promote_ == piece_to_promote)
		{
			if (move_detail) { *move_detail = move; }

			return true;
		}
	}

	return false;
}

MoveDetail PositionAdapter::DoMove(const Square square_from, const Square square_to)
{
	MoveDetail best_move = GetBestMove(square_from, square_to);

	if (best_move.move_ != MOVE_NONE)
	{
		this->DoMove(best_move);
	}

	return best_move;
}

MoveDetail PositionAdapter::DoMove(const Move move)
{
	MoveDetail move_detail;
	DetailOfMove(*position_, move, move_detail);

	if (move_detail.move_ != MOVE_NONE)
	{
		this->DoMove(move_detail);
	}

	return move_detail;
}

void PositionAdapter::DoMove(const MoveDetail& move_detail)
{
	current_state_info_ = state_info_pool_.takeOut();
	position_->do_move(move_detail.move_, *current_state_info_);

	moved_stack_.emplace_back(move_detail);

	this->DirtyCurrentLegalMoves();
}

bool PositionAdapter::UndoLastMove(MoveDetail* detail)
{
	if (!moved_stack_.empty())
	{
		position_->undo_move(moved_stack_.back().move_);

		if (detail) { *detail = moved_stack_.back(); }

		moved_stack_.pop_back();

		auto pre = current_state_info_->previous;
		state_info_pool_.giveBack(current_state_info_);
		current_state_info_ = pre;

		this->DirtyCurrentLegalMoves();

		return true;
	}

	return false;
}

void PositionAdapter::TryToUpdateCurrentLegalMoves()
{
	if (legal_moves_is_dirty_)
	{
		legal_moves_is_dirty_ = false;

		current_legal_moves_.clear();

		for (MoveList<LEGAL> it(*position_); *it; ++it)
		{
			MoveDetail d;
			if (DetailOfMove(*position_, *it, d))
			{
				current_legal_moves_.push_back(std::move(d));
			}
		}
	}
}

void PositionAdapter::ResetToStarting(bool white_is_first)
{
	return ResetToStarting(std::string(white_is_first ? StartWithWhite : StartWithBlack));
}

void PositionAdapter::ResetToStarting(const std::string& starting_FEN)
{
	while (current_state_info_)
	{
		state_info_pool_.giveBack(current_state_info_);

		current_state_info_ = current_state_info_->previous;
	}

	moved_stack_.swap(MovedStack());

	starting_FEN_ = starting_FEN;
	position_->set(starting_FEN_, false, thread_.get());
	
	this->DirtyCurrentLegalMoves();
}

static bool DetailOfMove(Position& pos, Move m, MoveDetail& d)
{
	if (m == MOVE_NONE) { return false; }

	if (m == MOVE_NULL) { return false; }

	assert(MoveList<LEGAL>(pos).contains(m));

	Color us = pos.side_to_move();
	Square from = from_sq(m);
	Square to = to_sq(m);
	Piece pc = pos.piece_on(from);
	PieceType pt = type_of(pc);

	d.square_from_ = from;
	d.square_to_ = to;

	const MoveType type = type_of(m);

	if (type == CASTLE)
	{
		d.is_short_castling_ = to > from;
		d.is_long_castling_ = !d.is_short_castling_;
	}
	else
	{
		if (pos.capture(m))
		{
			d.is_captured_ = true;
			if (type == ENPASSANT)
			{
				//Square ep = pos.ep_square();
				d.square_captured_ = (to > from) ? (to - Square(8)) : (to + Square(8)); //when ENPASSANT square_captured and square_to are definitely on a same File
			}
			else
			{
				d.square_captured_ = to;
			}
		}

		if (type == PROMOTION)
		{
			d.is_promotion_ = true;
			d.piece_to_promote_ = make_piece(us, promotion_type(m));
		}
	}

	if (pos.gives_check(m, CheckInfo(pos)))
	{
		d.is_check_ = true;

		StateInfo st;
		pos.do_move(m, st);
		d.is_checkmate_ = MoveList<LEGAL>(pos).size() == 0;
		pos.undo_move(m);

		/*
		// prevent assert(captured != KING) in Position::do_move
		PieceType captured = type == ENPASSANT ? PAWN : type_of(pos.piece_on(to));
		if (captured == KING)
		{
		d.is_checkmate_ = true;
		}
		else
		{
		StateInfo st;
		pos.do_move(m, st);

		d.is_checkmate_ = MoveList<LEGAL>(pos).size() == 0;

		pos.undo_move(m);
		}
		*/
	}

	d.move_ = m;

	return true;
}


STK_NAMESPACE_END