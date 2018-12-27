#include "game_record.h"
#include <algorithm>
#include "game/model/base/chess/chess_info_matrix.h"
#include "game/model/base/record/custom_record.h"

GAME_NAMESPACE_BEGIN

GameRecordUniquePtr GameRecord::Clone() const
{
	ChessMatrixPtr chesses = chesses_->Clone();
	GameActionWithStatesPtr actions = actions_->Clone();
	CustomRecordPtr custom_record = custom_record_->Clone();

	return GameRecordUniquePtr(new GameRecord(board_type_, chesses, actions, custom_record));
}

GameSnapshotUniquePtr GameRecord::Snapshot() const
{
	ChessMatrixPtr chesses = chesses_->Clone();
	CustomRecordPtr custom_record = custom_record_->Clone();

	return GameSnapshotUniquePtr(new GameSnapshot(board_type_, chesses, custom_record));
}

void GameRecord::CopyFromSnapshot(const GameSnapshot& snapshot)
{
	if (&snapshot != this)
	{
		chesses_->CopyFrom(*snapshot.chesses_);
		custom_record_->CopyFrom(*snapshot.custom_record_);
	}
}

void GameRecord::CopyFrom(const GameRecord& rhs)
{
	if (&rhs != this)
	{
		chesses_->CopyFrom(*rhs.chesses_);
		custom_record_->CopyFrom(*rhs.custom_record_);
		actions_->CopyFrom(*rhs.actions_);
	}
}

void GameRecord::Reset()
{
	chesses_->Reset();
	custom_record_->Reset();
	actions_->Reset();
}

unsigned int GameRecords::NumberOfValidGameRecords() const
{
	return std::count_if(cbegin(), cend(), [](const GameRecordPtr& record){ return record->IsValid(); });
}

void GameRecords::RemoveInvalidGameRecords()
{
	this->erase(std::remove_if(begin(), end(), 
							   [](const GameRecordPtr& record){ return !record->IsValid(); }),
							   end());
}

GAME_NAMESPACE_END