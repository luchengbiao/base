#include "chess_id_generator.h"
#include "common/macros/assert.h"

GAME_NAMESPACE_BEGIN

class AlternateChessIdGenerator : public ChessIdGenerator
{
public:
	AlternateChessIdGenerator() : ChessIdGenerator(ChessIdGeneratorType::Alternate) {}

	virtual ChessId NextChessId(const ChessId last_chess_id_moved_in) override
	{
		return GoTrait::IdOfOppositePiece(last_chess_id_moved_in);
	}
};

class OnlyBlackChessIdGenerator : public ChessIdGenerator
{
public:
	OnlyBlackChessIdGenerator() : ChessIdGenerator(ChessIdGeneratorType::OnlyBlack) {}

	virtual ChessId NextChessId(const ChessId) override
	{
		return GoTrait::IdOfBlackPiece;
	}
};

class OnlyWhiteChessIdGenerator : public ChessIdGenerator
{
public:
	OnlyWhiteChessIdGenerator() : ChessIdGenerator(ChessIdGeneratorType::OnlyWhite) {}

	virtual ChessId NextChessId(const ChessId) override
	{
		return GoTrait::IdOfWhitePiece;
	}
};

ChessIdGeneratorHelper::ChessIdGeneratorHelper(ChessId last_chess_id_to_move_in, ChessIdGeneratorType type)
{
	this->Reset(last_chess_id_to_move_in, type);
}

ChessIdGeneratorType ChessIdGeneratorHelper::CurrentGeneratorType() const
{
	return current_generator_->Type();
}

ChessId ChessIdGeneratorHelper::NextChessId()
{
	last_chess_id_to_move_in_ = current_generator_->NextChessId(last_chess_id_to_move_in_);

	return last_chess_id_to_move_in_;
}

void ChessIdGeneratorHelper::Reset(ChessId last_chess_id_to_move_in, ChessIdGeneratorType type)
{
	this->LazyCreateGenerators();

	last_chess_id_to_move_in_ = last_chess_id_to_move_in;

	this->SwitchCurrentGenerator(type);
}

void ChessIdGeneratorHelper::LazyCreateGenerators()
{
	if (generators_.empty())
	{
		generators_.emplace_back(std::make_shared<AlternateChessIdGenerator>());
		generators_.emplace_back(std::make_shared<OnlyBlackChessIdGenerator>());
		generators_.emplace_back(std::make_shared<OnlyWhiteChessIdGenerator>());
	}
}

void ChessIdGeneratorHelper::SwitchCurrentGenerator(ChessIdGeneratorType type)
{
	current_generator_.reset();

	for (const auto& generator : generators_)
	{
		if (generator->Type() == type)
		{
			current_generator_ = generator;
			break;
		}
	}

	if (!current_generator_)
	{
		current_generator_ = generators_[0];
	}

	D_ASSERT(current_generator_, L"current_generator_ should not be null");
}

GAME_NAMESPACE_END