#ifndef __PROTO_COMMANDS_CMD_EXCHANGE_DATA_H__
#define __PROTO_COMMANDS_CMD_EXCHANGE_DATA_H__
#include "../base/command_cell.h"

PROTO_NAMESPACE_BEGIN

class CmdExchangeData : public CommandCell
{
	DECLARE_COMMAND_RUNTIME_INFO(CmdExchangeData)
};

PROTO_NAMESPACE_END

#endif