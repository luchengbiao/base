#ifndef __PROTO_COMMANDS_CMD_TEACHER_CHANGE_TO_PPT_H__
#define __PROTO_COMMANDS_CMD_TEACHER_CHANGE_TO_PPT_H__
#include "../base/command_cell.h"

PROTO_NAMESPACE_BEGIN

class CmdTeacherChangeToPpt : public CommandCell
{
	DECLARE_COMMAND_RUNTIME_INFO(CmdTeacherChangeToPpt)
};

PROTO_NAMESPACE_END

#endif