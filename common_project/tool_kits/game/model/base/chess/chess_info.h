#ifndef __GAME_MODEL_BASE_CHESS_CHESS_INFO_H__
#define __GAME_MODEL_BASE_CHESS_CHESS_INFO_H__
#include "game/macros.h"

GAME_NAMESPACE_BEGIN

class ChessInfo
{
public:
	using Id = int;

	static const Id EmptyId = -1;

public:
	explicit ChessInfo(Id id = EmptyId) : id_(id) {}

	inline Id	GetId() const { return id_; }
	inline void	SetId(Id id) { id_ = id; }

	inline bool	IsEmpty() const { return EmptyId == id_; }
	inline bool	IsNotEmpty() const { return EmptyId != id_; };
	inline void	SetToEmpty() { SetId(EmptyId); }

private:
	Id		id_{ EmptyId };
};

typedef ChessInfo::Id ChessId;

template<int X, int Y = X, typename ChessIdT = ChessId>
using ChessIdArr2D = ChessIdT[X][Y];

inline bool	ChessIdIsEmpty(const ChessId chess_id) { return chess_id == ChessInfo::EmptyId; }
inline bool	ChessIdIsNotEmpty(const ChessId chess_id) { return chess_id != ChessInfo::EmptyId; }

GAME_NAMESPACE_END

#endif