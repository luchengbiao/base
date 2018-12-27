#ifndef __PROTO_COMMANDS_CMD_START_OBSERVE_H__
#define __PROTO_COMMANDS_CMD_START_OBSERVE_H__
#include "../base/command_cell.h"

PROTO_NAMESPACE_BEGIN

class CmdStartObserve : public CommandCell
{
	DECLARE_COMMAND_RUNTIME_INFO(CmdStartObserve)
};

PROTO_NAMESPACE_END

#endif