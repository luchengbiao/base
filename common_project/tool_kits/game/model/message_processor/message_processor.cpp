#include "message_processor.h"
#include "base/util/base64.h"
#include "class_manager/class_manager.h"
#include "game/public/public_setting/public_setting.h"
#include "log/log.h"
#include "protocol/command/base/command_factory.h"
#include "protocol/command/base/command_group.h"

#ifdef _DEBUG
#define PRINT_MESSAGE_COMMAND_LOG 1
#endif

#define PRINT_THE_NUMBER_OF_EXISTING_COMMANDS()  //CHECK_THE_NUMBER_OF_EXISTING_COMMANDS()

GAME_NAMESPACE_BEGIN

MessageProcessor::MessageProcessor(const std::string& msg_channel_account, NetworkServiceType msg_channel_service, const CommandFactoryPtr& cmd_factory)
: cmd_factory_(cmd_factory)
{
	PublicSetting::SetChannelAccount(msg_channel_account);
	PublicSetting::SetChannelServiceType(msg_channel_service);

	connect(ClassManager::GetInstance(), &ClassManager::SignalAgoraMessageChannelReceive, this, &MessageProcessor::SlotAgoraMessageChannel);
}

MessageProcessor::~MessageProcessor() = default;

void MessageProcessor::SlotAgoraMessageChannel(std::string channel_id, std::string account, uint32_t uid, std::string msg)
{
	PRINT_THE_NUMBER_OF_EXISTING_COMMANDS();

	/*
	this->DecodeMessageIfEncoded(msg);

	auto cmd_group = proto::CommandGroup::Create();
	cmd_group->UnpackCommands(msg, cmd_factory_.get());

	this->HandleCommandGroup(*cmd_group, ChannelAccount() == account);
	*/
	cmd_observers_.NotifyObservers(&CommandObserver::OnMessage, msg, ChannelAccount() == account);

	PRINT_THE_NUMBER_OF_EXISTING_COMMANDS();
}

void MessageProcessor::HandleCommandGroup(const proto::CommandGroup& cmd_group, bool sender_is_self)
{
	cmd_group.Foreach([=](const CommandSharedPtr& cmd_ptr){

#if PRINT_MESSAGE_COMMAND_LOG == 1
		if (cmd_ptr->Type() != proto::CommandType::CheckHeartbeatFromPeer)
		{
			LOG_NFLT("") << *cmd_ptr;
		}
#endif

		cmd_observers_.NotifyObservers(&CommandObserver::OnCommand, cmd_ptr, sender_is_self);
	});
}

void MessageProcessor::AddCommandObserver(CommandObserver* observer)
{
	cmd_observers_.AddObserver(observer);
}

void MessageProcessor::RemoveCommandObserver(CommandObserver* observer)
{
	cmd_observers_.RemoveObserver(observer);
}

GAME_NAMESPACE_END

#undef  PRINT_MESSAGE_COMMAND_LOG