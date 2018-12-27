#ifndef __GAME_MODEL_CHESS_ACTIONS_CHESS_GAME_ACTION_H__
#define __GAME_MODEL_CHESS_ACTIONS_CHESS_GAME_ACTION_H__
#include "game/model/base/action/game_action_with_result.h"
#include "game/model/chess/actions/chess_game_action_class_runntime_info.h"

GAME_NAMESPACE_BEGIN

class GameBoardBase;
class ChessGameBoard;

class ChessGameAction : public GameActionWithResult
{
public:
	using ClassRuntimeInfo = ChessGameActionClassRuntimeInfo;

public:
	ChessGameAction() = default;

	ChessGameActionType				Type() const;
	virtual	std::string				ClassName() const override;

	virtual const ClassRuntimeInfo&	GetClassRuntimeInfo() const = 0;

protected:
	virtual	GameActionResult		DoOnBoard(GameBoardBase*) override final;
	virtual	GameActionResult		DoOnBoard(ChessGameBoard*) = 0;

	virtual	void					UndoOnBoard(GameBoardBase*) override final;
	virtual	void					UndoOnBoard(ChessGameBoard*) = 0;

	virtual	void					RedoOnBoard(GameBoardBase*) override final;
	virtual	void					RedoOnBoard(ChessGameBoard*) = 0;
};

GAME_NAMESPACE_END

#endif