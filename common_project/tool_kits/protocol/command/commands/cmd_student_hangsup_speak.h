#ifndef __PROTO_COMMANDS_CMD_STUDENT_HANGSUP_SPEAK_H__
#define __PROTO_COMMANDS_CMD_STUDENT_HANGSUP_SPEAK_H__
#include "../base/command_cell.h"

PROTO_NAMESPACE_BEGIN

class CmdStudentHangsupSpeak : public CommandCell
{
	DECLARE_COMMAND_RUNTIME_INFO(CmdStudentHangsupSpeak)
};

PROTO_NAMESPACE_END

#endif