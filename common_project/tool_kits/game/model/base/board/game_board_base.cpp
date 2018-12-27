#include "game_board_base.h"
#include <algorithm>
#include "common/macros/assert.h"
#include "game/model/base/action/game_action_facotry.h"
#include "game/model/base/action/game_action_with_result.h"
#include "game/model/base/board/game_board_observer.h"
#include "game/model/base/record/custom_record.h"
#include "log/log.h"

#define PRINT_THE_NUMBER_OF_EXISTING_ACTIONS()  CHECK_THE_NUMBER_OF_EXISTING_ACTIONS()

GAME_NAMESPACE_BEGIN

GameBoardBase::GameBoardBase(GameBoardType board_type, int rows, int cols, const GameRecordsPtr& all_games_to_be_cached)
: board_type_(board_type)
, rows_(rows)
, cols_(cols)
, all_games_(all_games_to_be_cached ? all_games_to_be_cached : std::make_shared<GameRecords>())
, game_observers_(new GameBoardObserverList()) // compiler not support std::make_unique yet
{
	D_ASSERT(IsValidBoardType(board_type) && rows_ > 0 && cols_ > 0, L"param error");
}

void GameBoardBase::CreateNewGameWithCustomRecord(const CustomRecordPtr& custom_record)
{
	D_ASSERT(custom_record, L"custom_record should not be null");

	this->TryToSaveCurrentGame();

	current_game_ = std::make_shared<GameRecord>(board_type_,
												 std::make_shared<ChessInfoMatrix>(rows_, cols_), 
												 std::make_shared<GameActionWithStates>(),
												 custom_record);

	this->SaveGameRecord(current_game_);
}

void GameBoardBase::CreateNewGameWithCurrentGame()
{
	this->TryToSaveCurrentGame();

	this->CreateNewGameWithClone(current_game_);
}

bool GameBoardBase::CreateNewGameWithHistoryGame(int index_of_history)
{
	bool ret = false;

	this->TryToSaveCurrentGame();
	this->RemoveInvalidGameRecords();

	if (index_of_history >= 0 && index_of_history < all_games_->size())
	{
		this->CreateNewGameWithClone(all_games_->at(index_of_history));

		ret = true;
	}

	return ret;
}

void GameBoardBase::CreateNewGameWithClone(const GameRecordPtr& clone)
{
	D_ASSERT(clone, L"clone should not be null");
	D_ASSERT(clone->BoardType() == BoardType(), L"the board type of clone should be same with mine");

	this->TryToSaveCurrentGame();

	current_game_ = clone->Clone();

	this->SaveGameRecord(current_game_);
}

void GameBoardBase::TryToSaveCurrentGame()
{
	if (current_game_ && current_game_->IsValid())
	{
		SaveGameRecord(current_game_);
	}
}

void GameBoardBase::SaveGameRecord(const GameRecordPtr& record)
{
	if (record && 
		std::find(all_games_->cbegin(), all_games_->cend(), record) == all_games_->cend())
	{
		all_games_->emplace_back(record);
	}
}

void GameBoardBase::RemoveInvalidGameRecords()
{
	all_games_->RemoveInvalidGameRecords();

	DBG_WRAPPER(LOG_NINFO(L"{0}: Number of total boards: {1}, Number of valid boards: {2}") << all_games_.get() << all_games_->size() << all_games_->NumberOfValidGameRecords());
}

void GameBoardBase::SwitchToGame(int index)
{
	this->TryToSaveCurrentGame();
	this->RemoveInvalidGameRecords();

	if (index >= 0 && index < all_games_->size())
	{
		auto last_game = current_game_;

		current_game_ = all_games_->at(index);
		D_ASSERT(current_game_->BoardType() == BoardType(), L"the board type of current game should be same with mine");
	}
}

GameBoardType GameBoardBase::BoardTypeOfHistoryGame(int index_of_history)
{
	auto board_type = GameBoardType::None;

	this->RemoveInvalidGameRecords();

	if (index_of_history >= 0 && index_of_history < all_games_->size())
	{
		board_type = all_games_->at(index_of_history)->BoardType();
	}

	return board_type;
}

void GameBoardBase::AddGameObserver(GameBoardObserver* observer)
{
	game_observers_->AddObserver(observer);
}

void GameBoardBase::RemoveGameObserver(GameBoardObserver* observer)
{
	game_observers_->RemoveObserver(observer);
}

int GameBoardBase::ChessRows() const
{
	return CurrentChesses()->Rows();
}

int GameBoardBase::ChessColumns() const
{
	return CurrentChesses()->Columns();
}

ChessInfo* GameBoardBase::ChessAt(const GridIndex& grid) const
{
	return CurrentChesses()->ChessAt(grid.X(), grid.Y());
}

ChessInfo* GameBoardBase::ChessAt(int x, int y) const
{
	return CurrentChesses()->ChessAt(x, y);
}

const ChessInfoMatrix& GameBoardBase::GetChesses() const
{
	return *CurrentChesses();
}

void GameBoardBase::EmptyAllChesses()
{
	CurrentChesses()->SetAllChessesTo(ChessInfo::EmptyId);
}

void GameBoardBase::UndoChessesViaActionResult(const GameActionResult& result)
{
	result.ForeachChessVariation([=](const ChessVariation& chess_var){
		auto chess = ChessAt(chess_var.GetGridIndex());
		if (chess) { chess->SetId(chess_var.GetOldChessId()); }
	});
}

void GameBoardBase::RedoChessesViaActionResult(const GameActionResult& result)
{
	result.ForeachChessVariation([=](const ChessVariation& chess_var){
		auto chess = ChessAt(chess_var.GetGridIndex());
		if (chess) { chess->SetId(chess_var.GetNewChessId()); }
	});
}

void GameBoardBase::VSInfoOfCurrentGame(GameMode& game_mode, VSPlayerInfo* black_player, VSPlayerInfo* white_player, PlayerSide* current_side)
{
	CurrentCustomRecord()->GetCurrentVSInfo(game_mode, black_player, white_player, current_side);
}

void GameBoardBase::SyncCurrentGame(const GameBoardBase& game_board)
{
	if (&game_board != this) 
	{
		current_game_->CopyFrom(*game_board.current_game_);
	}
}

void GameBoardBase::SetActionFacotry(const GameActionFactoryPtr& game_action_factory)
{
	game_action_factory_ = game_action_factory;
}

void GameBoardBase::DoCommand(const CommandSharedPtr& cmd_ptr, bool notify_observers)
{
	if (game_action_factory_)
	{
		PRINT_THE_NUMBER_OF_EXISTING_ACTIONS();

		GameActionPtr action_ptr = game_action_factory_->CreateActionWithCommand(cmd_ptr);
		if (action_ptr)
		{
			this->DoAction(action_ptr, notify_observers);
		}

		PRINT_THE_NUMBER_OF_EXISTING_ACTIONS();
	}
}

bool GameBoardBase::TryToDoCommand(const CommandSharedPtr& cmd_ptr, bool notify_observers_if_valid, std::string* failed_info)
{
	if (game_action_factory_)
	{
		PRINT_THE_NUMBER_OF_EXISTING_ACTIONS();

		GameActionPtr action_ptr = game_action_factory_->CreateActionWithCommand(cmd_ptr);
		if (action_ptr)
		{
			return this->TryToDoAction(action_ptr, notify_observers_if_valid, failed_info);
		}

		PRINT_THE_NUMBER_OF_EXISTING_ACTIONS();
	}

	return false;
}

void GameBoardBase::DoAction(const GameActionPtr& action, bool notify_observers)
{
	auto result = action->Do(this);

	if (result.IsValid())
	{
		this->CacheGameActionDone(action);

		if (notify_observers)
		{
			this->NotifyGameActionDone(result);
		}
	}
}

bool GameBoardBase::TryToDoAction(const GameActionPtr& action, bool notify_observers_if_valid, std::string* failed_info)
{
	auto result = action->Do(this);
	if (result.IsValid())
	{
		this->CacheGameActionDone(action);

		if (notify_observers_if_valid)
		{
			this->NotifyGameActionDone(result);
		}

		return true;
	}
	else
	{
		if (failed_info) { *failed_info = result.GetErrorInfo(); }
	}

	return false;
}

void GameBoardBase::CacheGameActionDone(const GameActionPtr& action)
{
	if (action->PropertyOfNeedingToBeCachedInOneGame()) // some action is not belonging to one game, such as new game(board) action
	{
		CurrentActions()->Cache(action);
	}
}

bool GameBoardBase::Undo(bool notify_immediately)
{
	bool one_step = false;

	CurrentActions()->RForeachActionWithBreaker([=, &one_step](GameActionWithState& action){ // reverse travel
		if (action.IsDone())
		{
			action.Action()->Undo(this);
			action.SetState(GameActionState::Undone);
			one_step |= action.Action()->PropertyOfBeingCountedAsOneStepWhenBackForward(); // break when encountering the first action that is counted as one step

			if (notify_immediately)
			{
				this->NotifyGameActionUndone(*std::static_pointer_cast<GameActionWithResult>(action.Action())->ActionResult());
			}
		}

		return one_step;
	});

	return one_step;
}

bool GameBoardBase::Redo(bool notify_immediately)
{
	bool one_step = false;

	CurrentActions()->ForeachActionWithBreaker([=, &one_step](GameActionWithState& action){
		if (action.IsUndone())
		{
			if (one_step && action.Action()->PropertyOfBeingCountedAsOneStepWhenBackForward())
			{
				return true; // break when encountering the second action that is counted as one step and and still kept Undone state
			}

			action.Action()->Redo(this);
			action.SetState(GameActionState::Done);
			one_step |= action.Action()->PropertyOfBeingCountedAsOneStepWhenBackForward();

			if (notify_immediately)
			{
				this->NotifyGameActionRedone(*std::static_pointer_cast<GameActionWithResult>(action.Action())->ActionResult());
			}
		}

		return false;
	});

	return one_step;
}

void GameBoardBase::Reset()
{
	all_games_->clear();

	current_game_->Reset();

	this->SaveGameRecord(current_game_);

	this->SetBoardToOpening();
}

void GameBoardBase::NotifyGameActionDone(const GameActionResult& result)
{
	game_observers_->NotifyObservers(&GameBoardObserver::OnGameActionDone, result);
}

void GameBoardBase::NotifyGameActionUndone(const GameActionResult& result)
{
	game_observers_->NotifyObservers(&GameBoardObserver::OnGameActionUndone, result);
}

void GameBoardBase::NotifyGameActionRedone(const GameActionResult& result)
{
	game_observers_->NotifyObservers(&GameBoardObserver::OnGameActionRedone, result);
}

GAME_NAMESPACE_END

