#ifndef __GAME_MODEL_BASE_ACTION_GAME_ACTION_WITH_RESULT_H__
#define __GAME_MODEL_BASE_ACTION_GAME_ACTION_WITH_RESULT_H__
#include <memory>
#include "game/model/base/action/game_action.h"
#include "game/model/base/action/game_action_result.h"
#include "game/model/base/board/game_board_base_fwd.h"
#include "protocol/command/base/command_fwd.h"

GAME_NAMESPACE_BEGIN

// NOTICE: in order to REUSE/SHARE a GameActionWithResult in many games(such as when creating a new board with current game or a history game), 
// there is a requirement that the GameActionResult cannot be modified after calculated firstly
class GameActionWithResult : public GameAction, public std::enable_shared_from_this<GameActionWithResult>
{
	typedef std::shared_ptr<GameActionResult> GameActionResultPtr;

public:
	GameActionWithResult() = default;

	GameActionWithResult(const GameActionWithResult&) = delete;
	GameActionWithResult& operator=(const GameActionWithResult&) = delete;

	virtual	GameActionResult			Do(GameBoardBase*) override;
	virtual	void						Undo(GameBoardBase*) override;
	virtual	void						Redo(GameBoardBase*) override;

	virtual	proto::CommandType			CmdType() const override;

	// default values of Properties
	virtual bool						PropertyOfChangingBoard() const override { return true; }
	virtual bool						PropertyOfNeedingToBeCachedInOneGame() const override { return true; }
	virtual bool						PropertyOfMakingBoardValid() const override { return false; }
	virtual bool						PropertyOfBeingCountedAsOneStepWhenBackForward() const override { return PropertyOfMakingBoardValid(); }

	virtual std::string					ClassName() const { return "GameActionWithResult"; }

	void								SetCommandPtr(const CommandSharedPtr&);
	CommandSharedPtr					GetCommandPtr() const;

	void								AssociateWithActionResult(GameActionResult& result);

	inline const GameActionResultPtr&	ActionResult() const { return result_ptr_; }

protected:
	virtual	GameActionResult			DoOnBoard(GameBoardBase*) = 0;
	virtual	void						UndoOnBoard(GameBoardBase*) = 0;
	virtual	void						RedoOnBoard(GameBoardBase*) = 0;

protected:
	CommandSharedPtr					command_ptr_;
	GameActionResultPtr					result_ptr_;
};

GAME_NAMESPACE_END

#endif