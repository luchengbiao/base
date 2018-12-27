#ifndef __GAME_MODEL_CHESS_CHESS_GAME_BOARD_H__
#define __GAME_MODEL_CHESS_CHESS_GAME_BOARD_H__
#include <map>
#include "actions/chess_action_friends.h"
#include "stockfish/adapter/position_adapter_fwd.h"
#include "game/model/base/board/game_board.h"
#include "game/model/base/grid/grid_motion.h"
#include "game/model/chess/chess_record.h"

namespace proto { struct NewGameBoardEx; }

GAME_NAMESPACE_BEGIN

typedef std::shared_ptr<ChessRecord> ChessRecordPtr;

class ChessGameBoard;
typedef std::unique_ptr<ChessGameBoard> ChessGameBoardUniquePtr;

class ChessGameBoard : public GameBoard<8>
{
	CHESS_ACTION_FRIENDS

public:
	static ChessGameBoardUniquePtr	Create(const GameRecordsPtr& all_games_to_be_cached = nullptr);

	bool							IsRuleBased() const;
	void							SetRuleBased(bool rule_based);

	std::map<GridIndex, bool>		GridsReachableFrom(const GridIndex& grid_from) const;
	bool							IsMoveLegalWithRule(const GridIndex& grid_from, const GridIndex& grid_to);

	GridMotion						LastGridMotionOfCurrentGame(ChessId* chess_id_moved = nullptr) const;

	bool							IsGameOverWithRule(GameOverType* type = nullptr);
	bool							IsCheckingWithRule() const;

protected:
	ChessGameBoard(const GameRecordsPtr& all_games_to_be_cached);

	void							Init();

	void							CreateNewBoard(bool rule_based, bool first_hand_is_white_side = true);
	bool							CreateNewBoard(const proto::NewGameBoardEx& entity);

	void							ClearBoard();

	inline ChessRecordPtr			CurrentRecord() const { return std::static_pointer_cast<ChessRecord>(CurrentCustomRecord()); }
	inline PositionAdapterPtr		CurrentRuleBoard() const { return CurrentRecord()->rule_board_; }

	void							SetBoardToOpening();

	std::map<GridIndex, bool>		GridsReachableFromWithRule(const GridIndex& grid_from) const;
	std::map<GridIndex, bool>		GridsReachableFromWithoutRule(const GridIndex& grid_from) const;
};

GAME_NAMESPACE_END

#endif