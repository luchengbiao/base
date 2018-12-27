#ifndef __PROTO_COMMANDS_CMD_TEACHER_FORBIDDEN_STUDENT_OPER_H__
#define __PROTO_COMMANDS_CMD_TEACHER_FORBIDDEN_STUDENT_OPER_H__
#include "../base/command_cell.h"

PROTO_NAMESPACE_BEGIN

class CmdTeacherForbiddenStudentOper : public CommandCell
{
	DECLARE_COMMAND_RUNTIME_INFO(CmdTeacherForbiddenStudentOper)
};

PROTO_NAMESPACE_END

#endif