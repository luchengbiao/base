#ifndef __PROTO_BASE_COMMAND_FACTORY_H__
#define __PROTO_BASE_COMMAND_FACTORY_H__
#include <vector>
#include "command.h"

PROTO_NAMESPACE_BEGIN

class CommandFactory
{
public:
	virtual ~CommandFactory() = default;

	template<typename... Args>
	CommandUniquePtr						CreateCommandAndPushVariants(CommandType cmd_type, Args&&... args) const;

	virtual CommandUniquePtr				CreateCommandWithType(CommandType cmd_type) const = 0;

	virtual std::vector<CommandSharedPtr>	CreateCommandsFromString(const std::string& str_raw) const;
};

template<typename... Args>
CommandUniquePtr CommandFactory::CreateCommandAndPushVariants(CommandType cmd_type, Args&&... args) const
{
	auto cmd_ptr = this->CreateCommandWithType(cmd_type);

	if (cmd_ptr)
	{
		cmd_ptr->PushVariants(std::forward<Args>(args)...);
	}

	return cmd_ptr;
}

PROTO_NAMESPACE_END

#endif