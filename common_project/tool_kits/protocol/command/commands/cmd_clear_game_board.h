#ifndef __PROTO_COMMANDS_CMD_CLEAR_GAME_BOARD_H__
#define __PROTO_COMMANDS_CMD_CLEAR_GAME_BOARD_H__
#include "../base/command_cell.h"

PROTO_NAMESPACE_BEGIN

class CmdClearGameBoard : public CommandCell
{
	DECLARE_COMMAND_RUNTIME_INFO(CmdClearGameBoard)
};

PROTO_NAMESPACE_END

#endif