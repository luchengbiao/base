#include "cmd_group_with_timestamp.h"
#include <algorithm>
#include <unordered_set>
#include "base/util/base64.h"
#include "log/log.h"
#include "protocol/command/base/command_factory.h"
#include "protocol/utils/byte_parser.h"

PROTO_NAMESPACE_BEGIN

static const std::string Str_BeginningOfSection{ "xqs_0" };
static const std::string Str_EndOfSection{ "xqs_1" };

class CmdGroupWithTimestampPrivate
{
	friend class CmdGroupWithTimestamp;

	int UnpackFromString(const std::string& str, int index)
	{
		do 
		{
			int totalLen = str.length();

			if ((index + 8) > totalLen) { break; } // at least 8 bytes: cmdLen(4) + timestamp(4)
			int cmdLen = byte_parser::Bytes2Int(&str.at(index), false); // here is big endian format

			if (cmdLen < 8) { break; } // cmdLen is 8 at least
			if ((index + cmdLen) > totalLen) { break; } // too long

			timestamp_ = byte_parser::Bytes2Int(&str.at(index + 4), false); // here is big endian format
			string_cmd_group_ = str.substr(index + 8, cmdLen - 8);

			return cmdLen;

		} while (false);

		return 0;
	}

	void UnpackGroup(bool needDecodeBase64, const CommandFactory& cmdFactory)
	{
		if (string_cmd_group_.empty())
		{
			return;
		}

		DBG_WRAPPER(LOG_DBG("{0} -- {1}") << timestamp_ << string_cmd_group_);

		if (IsBeginningOfSection() || IsEndOfSection())
		{
			return;
		}

		std::string strDecode;
		std::string* pTargetStr = &string_cmd_group_;

		if (needDecodeBase64)
		{
			pTargetStr = &strDecode;
			nbase::Base64Decode(string_cmd_group_, pTargetStr);
		}

		holder_->UnpackCommands(*pTargetStr, cmdFactory);

		string_cmd_group_.swap(std::string());
	}

	void MergeDrawGroup(const CommandFactory* cmdFactory, const std::unordered_set<CommandType>& setCmdToMerge, CommandType cmdGroupType)
	{
		do
		{
			std::vector<int> vecIndexToMerge;

			auto& cmdGroup = holder_->commands_;
			int count = cmdGroup.size();

			for (int i = 0; i < count; ++i)
			{
				auto itor = setCmdToMerge.find(cmdGroup.at(i)->Type());
				if (itor == setCmdToMerge.end() && vecIndexToMerge.empty())
				{
					break;
				}

				vecIndexToMerge.push_back(i);
			}

			if (vecIndexToMerge.size() < 2)
			{
				break;
			}

			CommandSharedPtr cmdPtr = cmdFactory->CreateCommandWithType(cmdGroupType);
			auto cmdGroupPtr = std::dynamic_pointer_cast<CommandGroup>(cmdPtr);
			if (!cmdGroupPtr)
			{
				break;
			}

			for (int index : vecIndexToMerge)
			{
				auto& ptrToMerge = cmdGroup.at(index);
				cmdGroupPtr->Add(ptrToMerge);
				ptrToMerge.reset();
			}

			cmdGroup.erase(std::remove_if(cmdGroup.begin(), cmdGroup.end(),
				[](const CommandSharedPtr& itor) { return !itor; }),
				cmdGroup.end());

			cmdGroup.emplace(cmdGroup.begin() + vecIndexToMerge.front(), cmdGroupPtr);

		} while (true);
	}

	bool IsBeginningOfSection() const
	{
		return string_cmd_group_ == Str_BeginningOfSection;
	}

	bool IsEndOfSection() const
	{
		return string_cmd_group_ == Str_EndOfSection;
	}

	CmdGroupWithTimestampPrivate(CmdGroupWithTimestamp* holder)
		: holder_(holder)
	{}

	CmdGroupWithTimestamp*		holder_;
	unsigned int				timestamp_;
	std::string					string_cmd_group_;
};

IMPLEMENT_COMMAND_RUNTIME_INFO_WTTHOUT_CTOR(CommandType::CmdGroupWithTimestamp, CmdGroupWithTimestamp, CommandGroup)

CmdGroupWithTimestamp::CmdGroupWithTimestamp()
: impl_(new CmdGroupWithTimestampPrivate(this))
{}

CmdGroupWithTimestamp::~CmdGroupWithTimestamp() = default;

unsigned int CmdGroupWithTimestamp::Timestamp() const
{
	return impl_->timestamp_;
}

void CmdGroupWithTimestamp::SetTimestamp(unsigned int timestamp)
{
	impl_->timestamp_ = timestamp;
}

void CmdGroupWithTimestamp::UnpackGroup(bool needDecodeBase64, const CommandFactory& cmdFactory)
{
	return impl_->UnpackGroup(needDecodeBase64, cmdFactory);
}

bool CmdGroupWithTimestamp::IsBeginningOfSection() const
{
	return impl_->IsBeginningOfSection();
}

bool CmdGroupWithTimestamp::IsEndOfSection() const
{
	return impl_->IsEndOfSection();
}

int CmdGroupWithTimestamp::UnpackFromString(const std::string& str, int startIndex)
{
	return impl_->UnpackFromString(str, startIndex);
}

std::string CmdGroupWithTimestamp::PackToString() const
{
	auto str_cmds = CommandGroup::PackToString();

	int total_len = 8 + str_cmds.size(); // 8: 4 for total_len, 4 for timestamp

	std::string prefix;
	prefix.reserve(8);
	prefix.append(byte_parser::Int2Bytes(total_len, false)); // here is big endian format
	prefix.append(byte_parser::Int2Bytes(impl_->timestamp_, false)); // here is big endian format

	return prefix + str_cmds;
}

std::string	 CmdGroupWithTimestamp::_StringOfLog(const std::string& strPrefix) const
{
	std::stringstream ss;
	ss << strPrefix;
	ss << "Timestamp( " << impl_->timestamp_ << " ) " << CommandGroup::_StringOfLog("");

	return ss.str();
}

PROTO_NAMESPACE_END