#ifndef __GAME_MODEL_CHESS_CHESS_RECORD_H__
#define __GAME_MODEL_CHESS_CHESS_RECORD_H__
#include "game/model/base/record/custom_record.h"
#include "game/public/enums/game_over_type.h"
#include "stockfish/adapter/position_adapter_fwd.h"

GAME_NAMESPACE_BEGIN

class ChessRecord : public CustomRecord
{
	typedef CustomRecord MyBase;

public:
	ChessRecord(bool rule_based, bool first_hand_is_white_side);
	ChessRecord(const ChessRecord&);
	ChessRecord& operator=(const ChessRecord&);

	virtual CustomRecordUniquePtr	Clone() const override;

	virtual void					CopyFrom(const CustomRecord&) override;

	void							CopyFrom(const ChessRecord&);

	virtual	void					Reset() override;

	inline PositionAdapterPtr		RuleBoard() const { return rule_board_; }

	virtual void					SetRuleBased(bool rule_based) override;

	virtual PlayerSide				GetCurrentSide() override;

	bool							IsGameOver(GameOverType* type = nullptr);
	bool							IsChecking() const;

private:
	bool							first_hand_is_white_side_{ true };
	PositionAdapterPtr				rule_board_{ nullptr };

	friend class ChessGameBoard;
};

GAME_NAMESPACE_END

#endif