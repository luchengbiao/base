#ifndef __GAME_MODEL_GO_GO_GAME_BOARD_H__
#define __GAME_MODEL_GO_GO_GAME_BOARD_H__
#include "actions/go_action_friends.h"
#include "go_trait.h"
#include "game/model/base/board/game_board_base.h"
#include "game/model/base/chess/chess_info.h"
#include "game/model/base/grid/grid_index.h"
#include "game/model/go/go_record.h"

namespace proto { struct NewGameBoardEx; }

GAME_NAMESPACE_BEGIN

typedef std::shared_ptr<GoRecord> GoRecordPtr;

class GoGameBoard;
typedef std::unique_ptr<GoGameBoard> GoGameBoardUniquePtr;

class GoGameBoard : public GameBoardBase
{
	GO_ACTION_FRIENDS

	typedef std::vector<GridIndex> GridIndexVector;

public:
	static GoGameBoardUniquePtr Create(int grid, const GameRecordsPtr& all_games_to_be_cached = nullptr);

	bool						IsRuleBased() const;
	void						SetRuleBased(bool rule_based);
	
	bool						IsAutoTakingPieces() const;
	void						SetAutoTakingPieces(bool auto_taking_pieces);

	void						IncreaseHistoryOfChessMovedIn(const GameActionResult&);
	void						DecreaseHistoryOfChessMovedIn(const GameActionResult&);

	ChessId						LastChessIdMovedIn() const;
	GridIndexVector				LastGridsTaken(ChessId* chess_id_taken = nullptr) const;
	ChessId						NextChessIdToMoveInWithRule() const;

	int							IndexOfChessAt(const GridIndex& grid) const;
	int							MaxIndexOfChess() const;

protected:
	GoGameBoard(int grid, const GameRecordsPtr& all_games_to_be_cached);

	void						Init();

	void						CreateNewBoard(bool rule_based, bool auto_taking_pieces);
	bool						CreateNewBoard(const proto::NewGameBoardEx& entity);

	void						ClearBoard();

	inline GoRecordPtr			CurrentRecord() const { return std::static_pointer_cast<GoRecord>(CurrentCustomRecord()); }

	virtual void				SetBoardToOpening() override;
};

GAME_NAMESPACE_END

#endif