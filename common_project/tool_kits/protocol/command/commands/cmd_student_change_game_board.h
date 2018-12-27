#ifndef __PROTO_COMMANDS_CMD_STUDENT_CHANGE_GAME_BOARD_H__
#define __PROTO_COMMANDS_CMD_STUDENT_CHANGE_GAME_BOARD_H__
#include "../base/command_cell.h"

PROTO_NAMESPACE_BEGIN

class CmdStudentChangeGameBoard : public CommandCell
{
	DECLARE_COMMAND_RUNTIME_INFO(CmdStudentChangeGameBoard)
};

PROTO_NAMESPACE_END

#endif