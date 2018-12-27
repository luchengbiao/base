#ifndef STOCKFISH_ADAPTER_POSITION_ADAPTER_H_INCLUDED
#define STOCKFISH_ADAPTER_POSITION_ADAPTER_H_INCLUDED
#include <deque>
#include <memory>
#include <string>
#include <vector>
#include "base/memory/objectpool.h"
#include "move_info.h"
#include "stockfish/adapter/game_over_type.h"
#include "stockfish/adapter/move_info.h"
#include "stockfish/src/position.h"
#include "stockfish/src/thread.h"

typedef std::unique_ptr<Position> PositionPtr;
typedef std::unique_ptr<MainThread> MainThreadPtr;

STK_NAMESPACE_BEGIN

typedef std::vector<MoveDetail> MoveDetails;
typedef std::deque<MoveDetail>	MovedStack;
typedef nbase::ObjectPool<StateInfo> StateInfoPool;

class PositionAdapter
{
public:
	PositionAdapter(bool white_is_first);
	~PositionAdapter();

	PositionAdapter(const PositionAdapter&);
	PositionAdapter& operator=(const PositionAdapter&);

	void			CurrentPieces(Piece pieces[SQUARE_NB]); // SQ_A1 ~ SQ_H8

	MoveDetails		CurrentLegalMoves();

	bool			CurrentSideIsWhite() const;

	bool			IsMoveLegal(const Square square_from, const Square square_to);

	bool			IsGameOver(GameOverType* type);
	bool			Checkers() const;

	MoveDetail		GetBestMove(const Square square_from, const Square square_to);
	bool			CanMoveToPromote(const Square square_from, const Square square_to, const Piece piece_to_promote, MoveDetail* move_detail = nullptr);

	MoveDetail		DoMove(const Square square_from, const Square square_to);
	MoveDetail		DoMove(const Move move);
	bool			UndoLastMove(MoveDetail*);

	void			ResetToStarting(bool white_is_first);
	void			ResetToStarting(const std::string& starting_FEN);

protected:
	PositionAdapter();

	void			DoMove(const MoveDetail& move_detail);
	void			DirtyCurrentLegalMoves() { legal_moves_is_dirty_ = true; }
	void			TryToUpdateCurrentLegalMoves();

private:
	std::string		starting_FEN_;
	PositionPtr		position_;
	MainThreadPtr	thread_;

	StateInfoPool	state_info_pool_{ 5, false };
	StateInfo*		current_state_info_{ nullptr };
	MovedStack		moved_stack_;

	MoveDetails		current_legal_moves_;
	bool			legal_moves_is_dirty_{ true };
};


STK_NAMESPACE_END

#endif