#ifndef __PROTO_COMMANDS_CMD_OBSERVE_DATA_H__
#define __PROTO_COMMANDS_CMD_OBSERVE_DATA_H__
#include "../base/command_cell.h"

PROTO_NAMESPACE_BEGIN

class CmdObserveData : public CommandCell
{
	DECLARE_COMMAND_RUNTIME_INFO(CmdObserveData)
};

PROTO_NAMESPACE_END

#endif