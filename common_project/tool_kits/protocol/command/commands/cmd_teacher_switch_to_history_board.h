#ifndef __PROTO_COMMANDS_CMD_TEACHER_SWITCH_TO_HISTORY_BOARD_H__
#define __PROTO_COMMANDS_CMD_TEACHER_SWITCH_TO_HISTORY_BOARD_H__
#include "../base/command_cell.h"

PROTO_NAMESPACE_BEGIN

class CmdTeacherSwitchToHistoryBoard : public CommandCell
{
	DECLARE_COMMAND_RUNTIME_INFO(CmdTeacherSwitchToHistoryBoard)
};

PROTO_NAMESPACE_END

#endif