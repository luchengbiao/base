#ifndef __PROTO_COMMANDS_CMD_TEACHER_CHANGE_PRICE_H__
#define __PROTO_COMMANDS_CMD_TEACHER_CHANGE_PRICE_H__
#include "../base/command_cell.h"

PROTO_NAMESPACE_BEGIN

class CmdTeacherChangePrice : public CommandCell
{
	DECLARE_COMMAND_RUNTIME_INFO(CmdTeacherChangePrice)
};

PROTO_NAMESPACE_END

#endif