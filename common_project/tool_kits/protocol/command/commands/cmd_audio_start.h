#ifndef __PROTO_COMMANDS_CMD_AUDIO_START_H__
#define __PROTO_COMMANDS_CMD_AUDIO_START_H__
#include "../base/command_cell.h"

PROTO_NAMESPACE_BEGIN

class CmdAudioStart : public CommandCell
{
	DECLARE_COMMAND_RUNTIME_INFO(CmdAudioStart)
};

PROTO_NAMESPACE_END

#endif