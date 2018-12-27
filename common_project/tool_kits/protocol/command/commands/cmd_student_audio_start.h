#ifndef __PROTO_COMMANDS_CMD_STUDENT_AUDIO_START_H__
#define __PROTO_COMMANDS_CMD_STUDENT_AUDIO_START_H__
#include "../base/command_cell.h"

PROTO_NAMESPACE_BEGIN

class CmdStudentAudioStart : public CommandCell
{
	DECLARE_COMMAND_RUNTIME_INFO(CmdStudentAudioStart)
};

PROTO_NAMESPACE_END

#endif