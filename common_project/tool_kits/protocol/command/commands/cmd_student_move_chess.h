#ifndef __PROTO_COMMANDS_CMD_STUDENT_MOVE_CHESS_H__
#define __PROTO_COMMANDS_CMD_STUDENT_MOVE_CHESS_H__
#include "../base/command_cell.h"

PROTO_NAMESPACE_BEGIN

class CmdStudentMoveChess : public CommandCell
{
	DECLARE_COMMAND_RUNTIME_INFO(CmdStudentMoveChess)
};

PROTO_NAMESPACE_END

#endif