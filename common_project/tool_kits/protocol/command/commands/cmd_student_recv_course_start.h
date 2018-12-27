#ifndef __PROTO_COMMANDS_CMD_STUDENT_RECV_COURSE_START_H__
#define __PROTO_COMMANDS_CMD_STUDENT_RECV_COURSE_START_H__
#include "../base/command_cell.h"

PROTO_NAMESPACE_BEGIN

class CmdStudentRecvCourseStart : public CommandCell
{
	DECLARE_COMMAND_RUNTIME_INFO(CmdStudentRecvCourseStart)
};

PROTO_NAMESPACE_END

#endif