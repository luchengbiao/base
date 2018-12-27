#ifndef __PROTO_COMMANDS_CMD_REPLAY_RECEIVE_H__
#define __PROTO_COMMANDS_CMD_REPLAY_RECEIVE_H__
#include "../base/command_cell.h"

PROTO_NAMESPACE_BEGIN

class CmdReplayReceive : public CommandCell
{
	DECLARE_COMMAND_RUNTIME_INFO(CmdReplayReceive)
};

PROTO_NAMESPACE_END

#endif