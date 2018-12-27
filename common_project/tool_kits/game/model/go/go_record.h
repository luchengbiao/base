#ifndef __GAME_MODEL_GO_GO_RECORD_H__
#define __GAME_MODEL_GO_GO_RECORD_H__
#include <vector>
#include "game/model/base/action/game_action_result.h"
#include "game/model/base/chess/chess_info.h"
#include "game/model/base/grid/grid_index.h"
#include "game/model/base/record/custom_record.h"

GAME_NAMESPACE_BEGIN

typedef std::vector<GridIndex> GridIndexVector;

class GoMoveRecord
{
public:
	GoMoveRecord() = default;
	GoMoveRecord(GoMoveRecord&& rh);
	GoMoveRecord& operator=(GoMoveRecord&& rh);

	ChessId			chess_id_last_moved_in_{ ChessInfo::EmptyId };
	GridIndex		grid_last_moved_in_;

	ChessId			chess_id_last_taken_{ ChessInfo::EmptyId };
	GridIndexVector	grids_last_taken_;

	int				old_index_of_last_chess_moved_in_{ 0 };
	int				new_index_of_last_chess_moved_in_{ 0 };
};

typedef std::vector<GoMoveRecord> GoMoveRecords;

class ChessInfoMatrix;
typedef ChessInfoMatrix ChessIndexMatrix;
typedef std::unique_ptr<ChessIndexMatrix> ChessIndexMatrixUniquePtr;

class GoRecord : public CustomRecord
{
	typedef CustomRecord MyBase;

public:
	GoRecord(int grid, bool rule_based, bool auto_taking_pieces, bool first_hand_is_white_side = false);
	GoRecord(const GoRecord&);
	GoRecord& operator=(const GoRecord&);

	inline bool						IsAutoTakingPieces() const { return auto_taking_pieces_; }
	void							SetAutoTakingPieces(bool auto_taking_pieces);

	virtual CustomRecordUniquePtr	Clone() const override;

	virtual void					CopyFrom(const CustomRecord&) override;

	virtual	void					Reset() override;

	virtual void					SetRuleBased(bool rule_based) override;

	virtual PlayerSide				GetCurrentSide() override;

	void							CopyFrom(const GoRecord&);

	void							IncreaseHistoryOfChessMovedIn(const GameActionResult& result);
	void							DecreaseHistoryOfChessMovedIn(const GameActionResult& result);
	void							ClearHistoryOfChessMovedIn();

	ChessId							LastChessIdMovedIn() const;
	GridIndexVector					LastGridsTaken(ChessId* chess_id_taken = nullptr) const;
	ChessId							NextChessIdToMoveInWithRule() const;

	int								IndexOfChessAt(const GridIndex& grid) const;
	int								MaxIndexOfChess() const;

private:
	ChessIndexMatrixUniquePtr		index_matrix_;
	int								max_index_{ 0 };
	bool							first_hand_is_white_side_{ false };
	bool							auto_taking_pieces_{false};
	GoMoveRecords					go_moves_;

	friend class GoGameBoard;
};

GAME_NAMESPACE_END

#endif