#ifndef __PROTO_COMMANDS_CMD_TEACHER_START_SYNC_CURRENT_CHESS_BOARD_H__
#define __PROTO_COMMANDS_CMD_TEACHER_START_SYNC_CURRENT_CHESS_BOARD_H__
#include "../base/command_cell.h"

PROTO_NAMESPACE_BEGIN

class CmdTeacherStartSyncCurrentChessBoard : public CommandCell
{
	DECLARE_COMMAND_RUNTIME_INFO(CmdTeacherStartSyncCurrentChessBoard)
};

PROTO_NAMESPACE_END

#endif