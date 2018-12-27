#include "chess_record.h"
#include "stockfish/adapter/position_adapter.h"

GAME_NAMESPACE_BEGIN

ChessRecord::ChessRecord(bool rule_based, bool first_hand_is_white_side)
: MyBase(rule_based)
{
	if (rule_based_)
	{
		rule_board_ = std::make_shared<stk::PositionAdapter>(first_hand_is_white_side);
	}
}

ChessRecord::ChessRecord(const ChessRecord& rh)
: MyBase(rh.rule_based_)
, first_hand_is_white_side_(rh.first_hand_is_white_side_)
{
	if (rule_based_)
	{
		rule_board_ = rh.rule_board_ ? std::make_shared<stk::PositionAdapter>(*rh.rule_board_) : std::make_shared<stk::PositionAdapter>(first_hand_is_white_side_);
	}
}

CustomRecordUniquePtr ChessRecord::Clone() const
{
	return CustomRecordUniquePtr(new ChessRecord(*this));
}

void ChessRecord::CopyFrom(const CustomRecord& rhs)
{
	this->CopyFrom(dynamic_cast<const ChessRecord&>(rhs));
}

void ChessRecord::CopyFrom(const ChessRecord& rhs)
{
	if (&rhs != this)
	{
		MyBase::CopyFrom(rhs);

		first_hand_is_white_side_ = rhs.first_hand_is_white_side_;

		if (rule_based_)
		{
			if (rule_board_)
			{
				if (rhs.rule_board_)
				{
					*rule_board_ = *rhs.rule_board_;
				}
				else
				{
					rule_board_->ResetToStarting(first_hand_is_white_side_);
				}
			}
			else
			{
				if (rhs.rule_board_)
				{
					rule_board_ = std::make_shared<stk::PositionAdapter>(*rhs.rule_board_);
				}
				else
				{
					rule_board_ = std::make_shared<stk::PositionAdapter>(first_hand_is_white_side_);
				}
			}
		}
	}
}

ChessRecord& ChessRecord::operator=(const ChessRecord& rhs)
{
	this->CopyFrom(rhs);

	return (*this);
}

void ChessRecord::SetRuleBased(bool rule_based)
{
	if (rule_based && !rule_based_) // only support false-true change
	{
		rule_based_ = true;

		rule_board_ = std::make_shared<stk::PositionAdapter>(true);
	}
}

PlayerSide ChessRecord::GetCurrentSide()
{
	PlayerSide side = PlayerSide::None;

	if (IsRuleBased())
	{
		side = rule_board_->CurrentSideIsWhite() ? PlayerSide::White : PlayerSide::Black;
	}

	return side;
}

void ChessRecord::Reset()
{
	MyBase::Reset();

	first_hand_is_white_side_ = false;
}

game::GameOverType FromStkGameOverType(const stk::GameOverType stk_type)
{
	game::GameOverType type = game::GameOverType::None;

	switch (stk_type)
	{
	case stk::GameOverType::None:
		type = game::GameOverType::None;
		break;

	case stk::GameOverType::WhiteWin:
		type = game::GameOverType::WhiteWin;
		break;

	case stk::GameOverType::BlackWin:
		type = game::GameOverType::BlackWin;
		break;

	case stk::GameOverType::Draw:
		type = game::GameOverType::Draw;
		break;

	default:
		break;
	}

	return type;
}

bool ChessRecord::IsGameOver(GameOverType* type)
{
	bool ret = false;

	if (rule_based_ && rule_board_)
	{
		stk::GameOverType type_tmp = stk::GameOverType::None;
		ret = rule_board_->IsGameOver(&type_tmp);

		if (type) { *type = FromStkGameOverType(type_tmp); }
	}

	return ret;
}

bool ChessRecord::IsChecking() const
{
	bool ret = false;

	if (rule_based_ && rule_board_)
	{
		ret = rule_board_->Checkers();
	}

	return ret;
}

GAME_NAMESPACE_END