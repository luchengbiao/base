#ifndef __PROTO_COMMANDS_CMD_STUDENT_CHARGE_BALANCE_H__
#define __PROTO_COMMANDS_CMD_STUDENT_CHARGE_BALANCE_H__
#include "../base/command_cell.h"

PROTO_NAMESPACE_BEGIN

class CmdStudentChargeBalance : public CommandCell
{
	DECLARE_COMMAND_RUNTIME_INFO(CmdStudentChargeBalance)
};

PROTO_NAMESPACE_END

#endif