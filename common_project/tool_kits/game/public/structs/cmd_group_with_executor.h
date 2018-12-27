#ifndef __GAME_PUBLIC_CMD_GROUP_WITH_EXECUTOR_H__
#define __GAME_PUBLIC_CMD_GROUP_WITH_EXECUTOR_H__
#include "game/public/enums/client_type.h"
#include "protocol/command/commands/cmd_group_with_timestamp.h"

typedef std::shared_ptr<proto::CmdGroupWithTimestamp> CmdGroupWithTimestampSharedPtr;

GAME_NAMESPACE_BEGIN

typedef ClientType CmdExecutor;

struct CmdGroupWithExecutor
{
	CmdGroupWithTimestampSharedPtr	cmd_group_;
	CmdExecutor						executor_{ CmdExecutor::None };

	CmdGroupWithExecutor() = default;

	CmdGroupWithExecutor(const CmdGroupWithTimestampSharedPtr& cmd_group, const CmdExecutor executor)
		:cmd_group_(cmd_group), executor_(executor)
	{}
};

typedef std::vector<CmdGroupWithExecutor> CmdGroupWithExecutors;

GAME_NAMESPACE_END

#endif