#ifndef __PROTO_COMMANDS_CMD_TEACHER_MARK_MANUAL_CHESS_H__
#define __PROTO_COMMANDS_CMD_TEACHER_MARK_MANUAL_CHESS_H__

#include "../base/command_cell.h"

PROTO_NAMESPACE_BEGIN

class CmdTeacherMarkManualChess : public CommandCell
{
	DECLARE_COMMAND_RUNTIME_INFO(CmdTeacherMarkManualChess)
};

PROTO_NAMESPACE_END

#endif