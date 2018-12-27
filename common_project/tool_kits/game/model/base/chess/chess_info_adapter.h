#ifndef __GAME_MODEL_BASE_CHESS_CHESS_INFO_ADAPTER_H__
#define __GAME_MODEL_BASE_CHESS_CHESS_INFO_ADAPTER_H__
#include "game/model/base/chess/chess_info.h"

GAME_NAMESPACE_BEGIN

// use to record last id temporarily
class ChessInfoAdapter
{
public:
	explicit ChessInfoAdapter(ChessInfo* chess_info)
		: chess_info_(chess_info)
	{}

	ChessInfoAdapter(const ChessInfoAdapter&) = delete;
	ChessInfoAdapter& operator=(const ChessInfoAdapter&) = delete;

	ChessId GetId() const { return chess_info_->GetId(); }
	ChessId GetLastId() const { return last_id_; }

	void SetId(ChessId id) { last_id_ = chess_info_->GetId(); chess_info_->SetId(id); }

	bool IsEmpty() const { return chess_info_->IsEmpty(); }
	bool IsNotEmpty() const {  return chess_info_->IsNotEmpty(); };
	void SetToEmpty() { SetId(ChessInfo::EmptyId); }

	explicit operator bool() const
	{
		return chess_info_ != nullptr;
	}

private:
	ChessInfo*	chess_info_{ nullptr };
	ChessId		last_id_{ ChessInfo::EmptyId };
};

GAME_NAMESPACE_END

#endif