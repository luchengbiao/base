#ifndef __GAME_MODEL_COMMAND_HANDLER_COMMAND_CHECKER_H__
#define __GAME_MODEL_COMMAND_HANDLER_COMMAND_CHECKER_H__
#include "game/macros.h"
#include "protocol/command/command_types.h"

GAME_NAMESPACE_BEGIN

class CommandChecker
{
public:
	static bool CommandIsRelatedToChess(const proto::CommandType cmd_type);
	static bool CommandIsRelatedToHeartbeat(const proto::CommandType cmd_type);
	static bool CommandIsRelatedToMD5Sync(const proto::CommandType cmd_type);
	static bool CommandInfluencesMD5(const proto::CommandType cmd_type);
	static bool CommandNeedsToBeHandledInGameBoard(const proto::CommandType cmd_type);
	static bool CommandInfluencesDrawOrMarkBoard(const proto::CommandType cmd_type);
	static bool CommandNeedsRecovery(const proto::CommandType cmd_type);

private:
	CommandChecker() = delete;
};

GAME_NAMESPACE_END

#endif