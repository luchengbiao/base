#ifndef __PROTO_COMMANDS_CMD_TEACHER_START_COURSE_H__
#define __PROTO_COMMANDS_CMD_TEACHER_START_COURSE_H__
#include "../base/command_cell.h"

PROTO_NAMESPACE_BEGIN

class CmdTeacherStartCourse : public CommandCell
{
	DECLARE_COMMAND_RUNTIME_INFO(CmdTeacherStartCourse)
};

PROTO_NAMESPACE_END

#endif