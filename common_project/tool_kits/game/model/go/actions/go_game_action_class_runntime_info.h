#ifndef __CHESS_MODEL_GO_ACTIONS_GAME_ACTION_CLASS_RUNTIME_INFO_H__
#define __CHESS_MODEL_GO_ACTIONS_GAME_ACTION_CLASS_RUNTIME_INFO_H__
#include <memory>
#include "common/class_runtime_info/class_runtime_info.h"
#include "game/model/go/actions/go_game_action_types.h"

namespace game { class GoGameAction; }

using PFuncGoGameActionCreator = std::unique_ptr<game::GoGameAction>(*)();
using GoGameActionClassRuntimeInfo = ClassRuntimeInfoWithType<game::GoGameAction, game::GoGameActionType, PFuncGoGameActionCreator>;

#define DECLARE_GO_GAME_ACTION_RUNTIME_INFO(className) \
public: \
	static std::unique_ptr<className> Create(); \
	static const GoGameActionClassRuntimeInfo& RegisterClassRuntimeInfo(); \
	virtual const GoGameActionClassRuntimeInfo& GetClassRuntimeInfo() const override; \
	className(); \
private: \
	virtual	GameActionResult DoOnBoard(GoGameBoard*) override; \
	virtual	void UndoOnBoard(GoGameBoard*) override; \
	virtual	void RedoOnBoard(GoGameBoard*) override;

#define IMPLEMENT_GO_GAME_ACTION_RUNTIME_INFO_WTIHOUT_CTOR(type, className) \
	std::unique_ptr<className> className::Create() \
{ std::unique_ptr<className> uniquePtr; uniquePtr.reset(new className); return uniquePtr; } \
	const GoGameActionClassRuntimeInfo& className::RegisterClassRuntimeInfo() \
{ static GoGameActionClassRuntimeInfo className##ClassRuntimeInfo(type, #className, (PFuncGoGameActionCreator)(&className::Create)); return className##ClassRuntimeInfo; } \
	const GoGameActionClassRuntimeInfo& className::GetClassRuntimeInfo() const { return className::RegisterClassRuntimeInfo(); } \

#define IMPLEMENT_GO_GAME_ACTION_RUNTIME_INFO(type, className) \
	className::className() = default; \
	IMPLEMENT_GO_GAME_ACTION_RUNTIME_INFO_WTIHOUT_CTOR(type, className) \

#define REGISTER_GO_GAME_ACTION_RUNTIME_INFO(className)  className::RegisterClassRuntimeInfo()

#endif