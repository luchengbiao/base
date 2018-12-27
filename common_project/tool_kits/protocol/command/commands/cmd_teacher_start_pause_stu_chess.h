#ifndef __PROTO_COMMANDS_CMD_TEACHER_START_PAUSE_STU_CHESS_H__
#define __PROTO_COMMANDS_CMD_TEACHER_START_PAUSE_STU_CHESS_H__
#include "../base/command_cell.h"

PROTO_NAMESPACE_BEGIN

class CmdTeacherStartPauseStuChess : public CommandCell
{
	DECLARE_COMMAND_RUNTIME_INFO(CmdTeacherStartPauseStuChess)
};

PROTO_NAMESPACE_END

#endif