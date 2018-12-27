#ifndef __PROTO_COMMANDS_CMD_TEACHER_CHECK_MANUAL_CHESS_H__
#define __PROTO_COMMANDS_CMD_TEACHER_CHECK_MANUAL_CHESS_H__
#include "../base/command_cell.h"

PROTO_NAMESPACE_BEGIN

class CmdTeacherCheckManualChess : public CommandCell
{
	DECLARE_COMMAND_RUNTIME_INFO(CmdTeacherCheckManualChess)
};

PROTO_NAMESPACE_END

#endif