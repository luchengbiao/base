#ifndef __PROTO_COMMANDS_CMD_CHECK_HEARTBEAT_FROM_PEER_H__
#define __PROTO_COMMANDS_CMD_CHECK_HEARTBEAT_FROM_PEER_H__
#include "../base/command_cell.h"

PROTO_NAMESPACE_BEGIN

class CmdCheckHeartbeatFromPeer : public CommandCell
{
	DECLARE_COMMAND_RUNTIME_INFO(CmdCheckHeartbeatFromPeer)
};

PROTO_NAMESPACE_END

#endif