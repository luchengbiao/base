#ifndef __PROTO_COMMANDS_CMD_TEACHER_MOVE_CHESS_H__
#define __PROTO_COMMANDS_CMD_TEACHER_MOVE_CHESS_H__

#include "../base/command_cell.h"

PROTO_NAMESPACE_BEGIN

class CmdTeacherMoveChess : public CommandCell
{
	DECLARE_COMMAND_RUNTIME_INFO(CmdTeacherMoveChess)
};

PROTO_NAMESPACE_END

#endif