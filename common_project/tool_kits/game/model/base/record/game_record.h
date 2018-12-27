#ifndef __GAME_MODEL_BASE_RECORD_GAME_RECORD_H__
#define __GAME_MODEL_BASE_RECORD_GAME_RECORD_H__
#include <vector>
#include "game/model/base/record/game_action_with_state.h"
#include "game/public/enums/game_board_type.h"

GAME_NAMESPACE_BEGIN

class ChessInfoMatrix;
typedef std::shared_ptr<ChessInfoMatrix> ChessMatrixPtr;

class GameActions : public std::vector<GameActionPtr>
{
public:
	GameActions() = default;
};
typedef std::shared_ptr<GameActions> GameActionsPtr;

typedef std::shared_ptr<GameActionWithStates> GameActionWithStatesPtr;

class CustomRecord;
typedef std::shared_ptr<CustomRecord> CustomRecordPtr;

// GameSnapshot is composed of GameBoardType, ChessMatrix and CustomRecord(maybe ChessRecord or GoRecord)
class GameSnapshot
{
public:
	GameSnapshot(const GameBoardType board_type, const ChessMatrixPtr& chesses, const CustomRecordPtr& custom_record)
		: board_type_(board_type), chesses_(chesses), custom_record_(custom_record)
	{
		D_ASSERT(board_type_ != GameBoardType::None, L"board_type_ should not be GameBoardType::None");
		D_ASSERT(chesses_, L"chesses_ should not be null");
		D_ASSERT(custom_record_, L"custom_record_ should not be null");
	}
	
	inline GameBoardType			BoardType() const { return board_type_; }
	inline ChessMatrixPtr			GetChesses() const { return chesses_; }

	inline CustomRecordPtr			GetCustomRecord() const { return custom_record_; }
	inline void						SetCustomRecord(const CustomRecordPtr& custom_record) { custom_record_ = custom_record; }
	
	template<typename CustomRecordT>
	inline std::shared_ptr<CustomRecordT> GetCustomRecord() const { return std::dynamic_pointer_cast<CustomRecordT>(custom_record_); }

protected:
	GameBoardType					board_type_{ GameBoardType::None };
	ChessMatrixPtr					chesses_;
	CustomRecordPtr					custom_record_;

	friend class GameRecord;
};

typedef std::unique_ptr<GameSnapshot> GameSnapshotUniquePtr;
typedef std::shared_ptr<GameSnapshot> GameSnapshotPtr;

class GameRecord;
typedef std::unique_ptr<GameRecord> GameRecordUniquePtr;

// GameRecord is composed of GameBoardType, ChessMatrix, GameActionWithStates and CustomRecord(maybe ChessRecord or GoRecord)
class GameRecord : public GameSnapshot
{
public:
	GameRecord(const GameBoardType board_type, const ChessMatrixPtr& chesses, const GameActionWithStatesPtr& actions, const CustomRecordPtr& custom_record)
		: GameSnapshot(board_type, chesses, custom_record), actions_(actions)
	{
		D_ASSERT(actions_, "actions_ should not be null");
	}

	GameRecordUniquePtr				Clone() const;

	GameSnapshotUniquePtr			Snapshot() const;
	void							CopyFromSnapshot(const GameSnapshot&);

	void							CopyFrom(const GameRecord&);

	void							Reset();

	inline GameActionWithStatesPtr	GetActions() const { return actions_; }

	inline bool						IsValid() const { return actions_ ? actions_->IsValid() : false; }

	inline unsigned int				StepsToBack() const { return actions_ ? actions_->StepsToBack() : 0; }
	inline unsigned int				StepsToForward() const { return actions_ ? actions_->StepsToForward() : 0; }

protected:
	GameActionWithStatesPtr			actions_;

	friend class GameBoardBase;
};

typedef std::shared_ptr<GameRecord> GameRecordPtr;

class GameRecords : public std::vector<GameRecordPtr>
{
public:
	GameRecords() = default;

	unsigned int					NumberOfValidGameRecords() const;

	void							RemoveInvalidGameRecords();
};

typedef std::shared_ptr<GameRecords> GameRecordsPtr;

GAME_NAMESPACE_END

#endif