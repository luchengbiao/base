#ifndef __PROTO_COMMANDS_CMD_TEACHER_SYNC_CURRENT_CHESS_BOARD_H__
#define __PROTO_COMMANDS_CMD_TEACHER_SYNC_CURRENT_CHESS_BOARD_H__
#include "../base/command_cell.h"

PROTO_NAMESPACE_BEGIN

class CmdTeacherSyncCurrentChessBoard : public CommandCell
{
	DECLARE_COMMAND_RUNTIME_INFO(CmdTeacherSyncCurrentChessBoard)
};

PROTO_NAMESPACE_END

#endif