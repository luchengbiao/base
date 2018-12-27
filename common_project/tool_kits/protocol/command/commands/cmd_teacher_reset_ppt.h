#ifndef __PROTO_COMMANDS_CMD_TEACHER_RESET_PPT_H__
#define __PROTO_COMMANDS_CMD_TEACHER_RESET_PPT_H__
#include "../base/command_cell.h"

PROTO_NAMESPACE_BEGIN

class CmdTeacherResetPpt : public CommandCell
{
	DECLARE_COMMAND_RUNTIME_INFO(CmdTeacherResetPpt)
};

PROTO_NAMESPACE_END

#endif