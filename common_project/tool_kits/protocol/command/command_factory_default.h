#ifndef __PROTO_COMMAND_COMMAND_FACTORY_DEFAULT_H__
#define __PROTO_COMMAND_COMMAND_FACTORY_DEFAULT_H__
#include "base/command_factory.h"

PROTO_NAMESPACE_BEGIN

class DefaultCommandFactory : public CommandFactory
{
public:
	DefaultCommandFactory();

	virtual CommandUniquePtr	CreateCommandWithType(CommandType) const override;
};

PROTO_NAMESPACE_END

#endif