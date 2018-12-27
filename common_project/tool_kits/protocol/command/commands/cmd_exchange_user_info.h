#ifndef __PROTO_COMMANDS_CMD_EXCHANGE_USER_INFO_H__
#define __PROTO_COMMANDS_CMD_EXCHANGE_USER_INFO_H__
#include "../base/command_cell.h"

PROTO_NAMESPACE_BEGIN

class CmdExchangeUserInfo : public CommandCell
{
	DECLARE_COMMAND_RUNTIME_INFO(CmdExchangeUserInfo)
};

PROTO_NAMESPACE_END

#endif