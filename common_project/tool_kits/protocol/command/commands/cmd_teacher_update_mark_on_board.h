#ifndef __PROTO_COMMANDS_CMD_TEACHER_UPDATE_MARK_H__
#define __PROTO_COMMANDS_CMD_TEACHER_UPDATE_MARK_H__
#include "../base/command_cell.h"

PROTO_NAMESPACE_BEGIN

class CmdTeacherUpdateMarkOnBoard : public CommandCell
{
	DECLARE_COMMAND_RUNTIME_INFO(CmdTeacherUpdateMarkOnBoard)
};

PROTO_NAMESPACE_END

#endif