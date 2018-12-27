#ifndef __PROTO_COMMANDS_CMD_STUDENT_CHANGE_PHOTO_PAGER_H__
#define __PROTO_COMMANDS_CMD_STUDENT_CHANGE_PHOTO_PAGER_H__
#include "../base/command_cell.h"

PROTO_NAMESPACE_BEGIN

class CmdStudentChangePhotoPager : public CommandCell
{
	DECLARE_COMMAND_RUNTIME_INFO(CmdStudentChangePhotoPager)
};

PROTO_NAMESPACE_END

#endif