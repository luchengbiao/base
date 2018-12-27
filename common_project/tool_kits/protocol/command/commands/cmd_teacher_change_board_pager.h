#ifndef __PROTO_COMMANDS_CMD_TEACHER_CHANGE_BOARD_PAGER_H__
#define __PROTO_COMMANDS_CMD_TEACHER_CHANGE_BOARD_PAGER_H__
#include "../base/command_cell.h"

PROTO_NAMESPACE_BEGIN

class CmdTeacherChangeBoardPager : public CommandCell
{
	DECLARE_COMMAND_RUNTIME_INFO(CmdTeacherChangeBoardPager)
};

PROTO_NAMESPACE_END

#endif