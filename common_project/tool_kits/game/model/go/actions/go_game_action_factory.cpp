#include "go_game_action_factory.h"
#include <string>
#include "common/macros/assert.h"
#include "go_game_action_class_runntime_info.h"
#include "go_game_action_back_forward.h"
#include "go_game_action_clear_board.h"
#include "go_game_action_move_into_board.h"
#include "go_game_action_move_outof_board.h"
#include "go_game_action_move_insideof_board.h"
#include "go_game_action_new_board.h"
#include "go_game_action_new_board_ex.h"
#include "go_game_action_switch_to_history_board.h"
#include "go_game_action_import_manual.h"
#include "go_game_action_update_auto_taking_pieces.h"
#include "protocol/command/base/command.h"

using proto::CommandType;

static void RegisterChessGameActionClassRuntimeInfo()
{
	REGISTER_GO_GAME_ACTION_RUNTIME_INFO(game::GoActionMoveIntoBoard);
	REGISTER_GO_GAME_ACTION_RUNTIME_INFO(game::GoActionMoveOutOfBoard);
	REGISTER_GO_GAME_ACTION_RUNTIME_INFO(game::GoActionMoveInsideOfBoard);
	REGISTER_GO_GAME_ACTION_RUNTIME_INFO(game::GoActionClearBoard);
	REGISTER_GO_GAME_ACTION_RUNTIME_INFO(game::GoActionNewBoard);
	REGISTER_GO_GAME_ACTION_RUNTIME_INFO(game::GoActionNewBoardEx);
	REGISTER_GO_GAME_ACTION_RUNTIME_INFO(game::GoActionBackForward);
	REGISTER_GO_GAME_ACTION_RUNTIME_INFO(game::GoActionSwitchToHistoryBoard);
	REGISTER_GO_GAME_ACTION_RUNTIME_INFO(game::GoActionImportManual);
	REGISTER_GO_GAME_ACTION_RUNTIME_INFO(game::GoActionUpdateAutoTakingPieces);
}

static GoGameActionClassRuntimeInfo*& FristChessGameActionClassRuntimeInfoPointer()
{
	static GoGameActionClassRuntimeInfo* fristChessGameActionClassRuntimeInfoPointer = nullptr;

	return fristChessGameActionClassRuntimeInfoPointer;
}

GoGameActionClassRuntimeInfo::ClassRuntimeInfoWithType(game::GoGameActionType type, const std::string& className, PFuncGoGameActionCreator creator)
: _type(type), _className(className), _creator(creator), _next(FristChessGameActionClassRuntimeInfoPointer())
{
	GoGameActionClassRuntimeInfo*& firstPtr = FristChessGameActionClassRuntimeInfoPointer();

	D_ASSERT(!(firstPtr && firstPtr->Type() == this->Type()), L"The type has already been registered");

	firstPtr = this;
}

GAME_NAMESPACE_BEGIN

GoGameActionFactory::GoGameActionFactory()
{
	RegisterChessGameActionClassRuntimeInfo();
}

std::shared_ptr<GoGameAction> GoGameActionFactory::CreateGameActionWithType(GoGameActionType type) const
{
	std::shared_ptr<GoGameAction> shared_ptr;

	GoGameActionClassRuntimeInfo* ptr = FristChessGameActionClassRuntimeInfoPointer();

	while (ptr)
	{
		if (ptr->Type() == type)
		{
			shared_ptr = ptr->Create();
			break;
		}

		ptr = ptr->Next();
	}

	return shared_ptr;
}

static bool IsMovedIntoBoard(const proto::Command& cmd)
{
	return (cmd.IntAt(1) == -1) && (cmd.IntAt(2) == -1) && (cmd.IntAt(3) >= 0) && (cmd.IntAt(4) >= 0);
}

static bool IsMovedOutOfBoard(const proto::Command& cmd)
{
	return (cmd.IntAt(1) >= 0) && (cmd.IntAt(2) >= 0) && (cmd.IntAt(3) == -1) && (cmd.IntAt(4) == -1);
}

static bool IsMovedInsdieOfBoard(const proto::Command& cmd)
{
	return (cmd.IntAt(1) >= 0) && (cmd.IntAt(2) >= 0) && (cmd.IntAt(3) >= 0) && (cmd.IntAt(4) >= 0);
}

typedef std::function<bool(const proto::Command&)> CommandPredictor;

static const struct
{
	const CommandType			cmd_type_;
	const CommandPredictor		predictor_;
	const GoGameActionType		act_type_;
}
Command_Action_Map[] = 
{
	{ CommandType::TeacherMoveChess,				IsMovedIntoBoard,		GoGameActionType::MoveIntoBoard },
	{ CommandType::StudentMoveChess,				IsMovedIntoBoard,		GoGameActionType::MoveIntoBoard },
	{ CommandType::TeacherMoveChess,				IsMovedOutOfBoard,		GoGameActionType::MoveOutOfBoard },
	{ CommandType::StudentMoveChess,				IsMovedOutOfBoard,		GoGameActionType::MoveOutOfBoard },
	{ CommandType::TeacherMoveChess,				IsMovedInsdieOfBoard,	GoGameActionType::MoveInsideOfBoard },
	{ CommandType::StudentMoveChess,				IsMovedInsdieOfBoard,	GoGameActionType::MoveInsideOfBoard },
	{ CommandType::ClearGameBoard,					nullptr,				GoGameActionType::ClearBoard },
	{ CommandType::TeacherNewGameBoard,				nullptr,				GoGameActionType::NewBoard },
	{ CommandType::TeacherNewGameBoardEx,			nullptr,				GoGameActionType::NewBoardEx },
	{ CommandType::TeacherBackForward,				nullptr,				GoGameActionType::BackForward },
	{ CommandType::TeacherSwitchToHistoryBoard,		nullptr,				GoGameActionType::SwitchToHistoryBoard },
	{ CommandType::TeacherImportChessManual,		nullptr,				GoGameActionType::ImportChessManual },
	{ CommandType::TeacherUpdateAutoTakingPieces,	nullptr,				GoGameActionType::UpdateAutoTakingPieces },
};

std::shared_ptr<GameAction> GoGameActionFactory::CreateActionWithCommand(const CommandSharedPtr& cmd_ptr)
{
	std::shared_ptr<GoGameAction> action_ptr;

	const auto cmd_type = cmd_ptr->Type();
	for (const auto& ca : Command_Action_Map)
	{
		if (cmd_type == ca.cmd_type_ && (!ca.predictor_ || ca.predictor_(*cmd_ptr)))
		{
			action_ptr = this->CreateGameActionWithType(ca.act_type_);
			break;
		}
	}

	if (action_ptr)
	{
		action_ptr->SetCommandPtr(cmd_ptr);
	}

	return action_ptr;
}

GAME_NAMESPACE_END