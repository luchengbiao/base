#ifndef __PROTO_COMMANDS_CMD_STUDENT_CHANGE_BOARD_PAGER_H__
#define __PROTO_COMMANDS_CMD_STUDENT_CHANGE_BOARD_PAGER_H__
#include "../base/command_cell.h"

PROTO_NAMESPACE_BEGIN

class CmdStudentChangeBoardPager : public CommandCell
{
	DECLARE_COMMAND_RUNTIME_INFO(CmdStudentChangeBoardPager)
};

PROTO_NAMESPACE_END

#endif