#include "heartbeat_helper.h"
#include "common/macros/assert.h"
#include "game/public/public_setting/public_setting.h"
#include "log/log.h"
#include "protocol/command/base/command_group.h"
#include "protocol/command/commands/cmd_check_heartbeat_from_peer.h"
#include "qthread_manager/closure.h"
#include "command_handler.h"

using namespace proto;

GAME_NAMESPACE_BEGIN

std::shared_ptr<HeartbeatHelper> HeartbeatHelper::Create(const CommandHandlerWeakPtr& command_handler)
{
	std::shared_ptr<HeartbeatHelper> ptr(new HeartbeatHelper(command_handler));

	if (ptr)
	{
		ptr->heartbeat_flag_.SetWeakPtr(ptr->GetWeakPtr());
	}

	return (ptr);
}

HeartbeatHelper::HeartbeatHelper(const CommandHandlerWeakPtr& command_handler)
: command_handler_(command_handler)
{
	qRegisterMetaType<StdClosure>("StdClosure");
	connect(this, SIGNAL(SignalClosure(StdClosure)), this, SLOT(SlotClosure(StdClosure)));
}

bool HeartbeatHelper::TryToHandleMasterHeartbeatCommands(const CommandGroupSharedPtr& cmd_group, bool sender_is_self)
{
	bool all_are_heartbeat = true;
	cmd_group->Foreach([=, &all_are_heartbeat](const CommandSharedPtr& cmd_ptr){
		if (cmd_ptr->Type() == proto::CommandType::CheckHeartbeatFromPeer)
		{
			this->HandleMasterHeartbeatCommand(cmd_ptr, sender_is_self);
		}
		else
		{
			all_are_heartbeat = false;
		}
	});

	return all_are_heartbeat;
}

void HeartbeatHelper::Stop()
{
	heartbeat_flag_.Cancel();
}

void HeartbeatHelper::SlotClosure(StdClosure closure)
{
	if (closure) { closure(); }
}

void HeartbeatHelper::HandleMasterHeartbeatCommand(const CommandSharedPtr& cmd_ptr, bool sender_is_self)
{
	if (!UsageTypeIsPlayback())
	{
	}
}

GAME_NAMESPACE_END