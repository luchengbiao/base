#ifndef __GAME_MODEL_BASE_BOARD_GAME_BOARD_BASE_H__
#define __GAME_MODEL_BASE_BOARD_GAME_BOARD_BASE_H__
#include <vector>
#include "base/framework/observer_list.h"
#include "game_board_base_fwd.h"
#include "game/model/base/chess/chess_info_matrix.h"
#include "game/model/base/record/game_record.h"
#include "game/public/enums/game_board_type.h"
#include "game/public/enums/game_mode.h"
#include "game/public/enums/player_side.h"
#include "game/public/structs/vs_player_info/vs_player_info.h"
#include "protocol/command/base/command.h"

GAME_NAMESPACE_BEGIN

class ChessInfo;
class GameActionFactory;
class GameActionResult;
class GameBoardObserver;
class GridIndex;

typedef ObserverList<GameBoardObserver>			GameBoardObserverList;
typedef std::unique_ptr<GameBoardObserverList>	GameBoardObserverListPtr;
typedef std::shared_ptr<GameActionFactory>		GameActionFactoryPtr;

class GameBoardBase
{
public:
	GameBoardBase(const GameBoardBase&) = delete;
	GameBoardBase& operator=(const GameBoardBase&) = delete;

	virtual ~GameBoardBase() = default;

	inline GameBoardType				BoardType() const { return board_type_; }
	GameBoardType						BoardTypeOfHistoryGame(int index_of_history);

	void								SetActionFacotry(const GameActionFactoryPtr& game_action_factory);
	GameActionFactoryPtr				ActionFacotry() const { return game_action_factory_; }

	void								DoCommand(const CommandSharedPtr& cmd_ptr, bool notify_observers = true);
	bool								TryToDoCommand(const CommandSharedPtr& cmd_ptr, bool notify_observers_if_valid = true, std::string* failed_info = nullptr);

	bool								Undo(bool notify_immediately = false);
	bool								Redo(bool notify_immediately = false);

	void								NotifyGameActionDone(const GameActionResult&);
	void								NotifyGameActionUndone(const GameActionResult&);
	void								NotifyGameActionRedone(const GameActionResult&);

	void								AddGameObserver(GameBoardObserver* observer);
	void								RemoveGameObserver(GameBoardObserver* observer);

	int									ChessRows() const;
	int									ChessColumns() const;

	ChessInfo*							ChessAt(const GridIndex&) const;
	ChessInfo*							ChessAt(int x, int y) const;
	const ChessInfoMatrix&				GetChesses() const;
	template<int GridX, int GridY = GridX>
	void								ChessIds(ChessIdArr2D<GridX, GridY>& arr2d) const;

	void								TryToSaveCurrentGame();

	void								VSInfoOfCurrentGame(GameMode& game_mode, 
															VSPlayerInfo* black_player = nullptr, 
															VSPlayerInfo* white_player = nullptr, 
															PlayerSide* current_side = nullptr);

	void								SyncCurrentGame(const GameBoardBase&);

	virtual	void						Reset();

protected:
	GameBoardBase(GameBoardType board_type, int rows, int cols, const GameRecordsPtr& all_games_to_be_cached = nullptr);

	void								DoAction(const GameActionPtr& action, bool notify_observers);
	bool								TryToDoAction(const GameActionPtr& action, bool notify_observers_if_valid, std::string* failed_info);
	void								CacheGameActionDone(const GameActionPtr& action);

	inline ChessMatrixPtr&				CurrentChesses() const { return current_game_->chesses_; }
	inline GameActionWithStatesPtr&		CurrentActions() const { return current_game_->actions_; }
	inline CustomRecordPtr&				CurrentCustomRecord() const { return current_game_->custom_record_; }

	void								CreateNewGameWithCustomRecord(const CustomRecordPtr& custom_record);
	void								CreateNewGameWithCurrentGame();
	bool								CreateNewGameWithHistoryGame(int index_of_history);
	void								CreateNewGameWithClone(const GameRecordPtr& clone);

	void								SaveGameRecord(const GameRecordPtr& record);
	void								RemoveInvalidGameRecords();

	void								SwitchToGame(int index);

	void								EmptyAllChesses();
	virtual void						SetBoardToOpening() = 0;

	void								UndoChessesViaActionResult(const GameActionResult& result);
	void								RedoChessesViaActionResult(const GameActionResult& result);

protected:
	const GameBoardType					board_type_{ GameBoardType::None };
	const int							rows_{ 0 };
	const int							cols_{ 0 };

	// storage to save all game records, 
	// maybe passed from the outside and shared among multi instances like GO(9, 13, 19)
	GameRecordsPtr						all_games_;

	GameRecordPtr						current_game_; // tuple of <current_chesses, current_actions, current_custom_record...>

	GameActionFactoryPtr				game_action_factory_;
	GameBoardObserverListPtr			game_observers_;
};

template<int GridX, int GridY>
void GameBoardBase::ChessIds(ChessIdArr2D<GridX, GridY>& arr2d) const
{
	CurrentChesses()->ChessIds(arr2d);
}

GAME_NAMESPACE_END

#endif