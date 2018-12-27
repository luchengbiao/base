#ifndef __PROTO_COMMANDS_CMD_TEACHER_SHOW_BOARD_RULE_H__
#define __PROTO_COMMANDS_CMD_TEACHER_SHOW_BOARD_RULE_H__
#include "../base/command_cell.h"

PROTO_NAMESPACE_BEGIN

class CmdTeacherShowBoardRule : public CommandCell
{
	DECLARE_COMMAND_RUNTIME_INFO(CmdTeacherShowBoardRule)
};

PROTO_NAMESPACE_END

#endif