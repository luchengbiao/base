#ifndef __PROTO_COMMANDS_CMD_END_COURSE_H__
#define __PROTO_COMMANDS_CMD_END_COURSE_H__
#include "../base/command_cell.h"

PROTO_NAMESPACE_BEGIN

class CmdEndCourse : public CommandCell
{
	DECLARE_COMMAND_RUNTIME_INFO(CmdEndCourse)
};

PROTO_NAMESPACE_END

#endif