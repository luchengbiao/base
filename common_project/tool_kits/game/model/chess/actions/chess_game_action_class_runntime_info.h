#ifndef __CHESS_MODEL_CHESS_ACTIONS_GAME_ACTION_CLASS_RUNTIME_INFO_H__
#define __CHESS_MODEL_CHESS_ACTIONS_GAME_ACTION_CLASS_RUNTIME_INFO_H__
#include <memory>
#include "common/class_runtime_info/class_runtime_info.h"
#include "game/model/chess/actions/chess_game_action_types.h"

namespace game { class ChessGameAction; }

using PFuncChessGameActionCreator = std::unique_ptr<game::ChessGameAction>(*)();
using ChessGameActionClassRuntimeInfo = ClassRuntimeInfoWithType<game::ChessGameAction, game::ChessGameActionType, PFuncChessGameActionCreator>;

#define DECLARE_CHESS_GAME_ACTION_RUNTIME_INFO(className) \
public: \
	static std::unique_ptr<className> Create(); \
	static const ChessGameActionClassRuntimeInfo& RegisterClassRuntimeInfo(); \
	virtual const ChessGameActionClassRuntimeInfo& GetClassRuntimeInfo() const override; \
	className(); \
private: \
	virtual	GameActionResult DoOnBoard(ChessGameBoard*) override; \
	virtual	void UndoOnBoard(ChessGameBoard*) override; \
	virtual	void RedoOnBoard(ChessGameBoard*) override;

#define IMPLEMENT_CHESS_GAME_ACTION_RUNTIME_INFO_WTIHOUT_CTOR(type, className) \
	std::unique_ptr<className> className::Create() \
{ std::unique_ptr<className> uniquePtr; uniquePtr.reset(new className); return uniquePtr; } \
	const ChessGameActionClassRuntimeInfo& className::RegisterClassRuntimeInfo() \
{ static ChessGameActionClassRuntimeInfo className##ClassRuntimeInfo(type, #className, (PFuncChessGameActionCreator)(&className::Create)); return className##ClassRuntimeInfo; } \
	const ChessGameActionClassRuntimeInfo& className::GetClassRuntimeInfo() const { return className::RegisterClassRuntimeInfo(); } \

#define IMPLEMENT_CHESS_GAME_ACTION_RUNTIME_INFO(type, className) \
	className::className() = default; \
	IMPLEMENT_CHESS_GAME_ACTION_RUNTIME_INFO_WTIHOUT_CTOR(type, className) \

#define REGISTER_CHESS_GAME_ACTION_RUNTIME_INFO(className)  className::RegisterClassRuntimeInfo()

#endif