#ifndef __PROTO_COMMANDS_CMD_TIMES_SIGN_H__
#define __PROTO_COMMANDS_CMD_TIMES_SIGN_H__
#include "../base/command_cell.h"

PROTO_NAMESPACE_BEGIN

class CmdTimesSign : public CommandCell
{
	DECLARE_COMMAND_RUNTIME_INFO(CmdTimesSign)
};

PROTO_NAMESPACE_END

#endif