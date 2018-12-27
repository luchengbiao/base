#include "command_factory.h"
#include "log/log.h"
#include "protocol/utils/byte_parser.h"

PROTO_NAMESPACE_BEGIN

std::vector<CommandSharedPtr> CommandFactory::CreateCommandsFromString(const std::string& str_raw) const
{
	std::vector<CommandSharedPtr> vec_cmd;

	int totalLength = str_raw.length();

	for (int i = 0; i < totalLength;)
	{
		if (i + 6 > totalLength) // safe to get cmdLen(4) + cmdType(1) + varCount(1)
		{
			break;
		}

		int cmdLen = byte_parser::Bytes2Int(&str_raw.at(i));
		if (cmdLen < 6) // at least 6 bytes: cmdLen(4) + cmdType(1) + varCount(1)
		{
			break;
		}

		CommandType cmdType = (CommandType)str_raw.at(i + 4);
		CommandSharedPtr cmdPtr = this->CreateCommandWithType(cmdType);

		if (cmdPtr)
		{
			cmdPtr->UnpackFromString(str_raw, i);

			vec_cmd.emplace_back(std::move(cmdPtr));
		}
		else
		{
			DBG_WRAPPER(LOG_WAR("No corresponding command class: {0}") << static_cast<int>(cmdType));
		}

		i += cmdLen;
	}

	return vec_cmd;
}

PROTO_NAMESPACE_END