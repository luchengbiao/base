#ifndef __PROTO_COMMANDS_CMD_TEACHER_UPDATE_AUTO_TAKING_PIECES_H__
#define __PROTO_COMMANDS_CMD_TEACHER_UPDATE_AUTO_TAKING_PIECES_H__
#include "../base/command_cell.h"

PROTO_NAMESPACE_BEGIN

class CmdTeacherUpdateAutoTakingPieces : public CommandCell
{
	DECLARE_COMMAND_RUNTIME_INFO(CmdTeacherUpdateAutoTakingPieces)
};

PROTO_NAMESPACE_END

#endif