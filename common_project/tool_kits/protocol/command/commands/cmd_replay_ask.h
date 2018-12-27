#ifndef __PROTO_COMMANDS_CMD_REPLAY_ASK_H__
#define __PROTO_COMMANDS_CMD_REPLAY_ASK_H__
#include "../base/command_cell.h"

PROTO_NAMESPACE_BEGIN

class CmdReplayAsk : public CommandCell
{
	DECLARE_COMMAND_RUNTIME_INFO(CmdReplayAsk)
};

PROTO_NAMESPACE_END

#endif