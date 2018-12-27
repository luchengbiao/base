#include "command_group.h"
#include <algorithm>
#include <memory>
#include <sstream>
#include "command_factory.h"

PROTO_NAMESPACE_BEGIN

IMPLEMENT_COMMAND_RUNTIME_INFO(CommandType::CommandGroup, CommandGroup, Command)

void CommandGroup::UnpackCommands(const std::string& str_raw, const CommandFactory& cmdFactory)
{
	this->AddCommands(cmdFactory.CreateCommandsFromString(str_raw));

	this->MergeCommands(cmdFactory);
}

void CommandGroup::MergeCommands(const CommandFactory& cmdFactory)
{
	std::unordered_set<CommandType> setStuDraw = { CommandType::StudentDrawStart, CommandType::StudentDrawMove, CommandType::StudentDrawEnd };
	this->MergeContinuousCommandsToGroup(cmdFactory, setStuDraw, CommandType::StudentDrawGroup);

	std::unordered_set<CommandType> setTeaDraw = { CommandType::TeacherDrawStart, CommandType::TeacherDrawMove, CommandType::TeacherDrawEnd };
	this->MergeContinuousCommandsToGroup(cmdFactory, setTeaDraw, CommandType::TeacherDrawGroup);

	std::unordered_set<CommandType> setTeaEraser = { CommandType::TeacherEraserDrawDown, CommandType::TeacherEraserDrawMove, CommandType::TeacherEraserDrawUp };
	this->MergeContinuousCommandsToGroup(cmdFactory, setTeaEraser, CommandType::TeacherEraserGroup);
}

std::vector<int> CommandGroup::GetContinousIndicesOfRelativeCommands(const std::unordered_set<CommandType>& setCmdType)
{
	std::vector<int> vecIndex;

	for (int i = 0; i < commands_.size(); ++i)
	{
		if (setCmdType.find(commands_.at(i)->Type()) != setCmdType.end())
		{
			vecIndex.push_back(i);
			continue;
		}

		if (vecIndex.size() > 1)
		{
			break;
		}

		if (!vecIndex.empty())
		{
			vecIndex.clear();
		}
	}

	return vecIndex;
}

void CommandGroup::MergeContinuousCommandsToGroup(const CommandFactory& cmdFactory, const std::unordered_set<CommandType>& setCmdToMerge, CommandType cmdGroupType)
{
	do
	{
		std::vector<int> vecIndexToMerge = this->GetContinousIndicesOfRelativeCommands(setCmdToMerge);

		if (vecIndexToMerge.size() < 2)
		{
			break;
		}

		CommandSharedPtr cmdPtr = cmdFactory.CreateCommandWithType(cmdGroupType);
		auto cmdGroupPtr = std::dynamic_pointer_cast<CommandGroup>(cmdPtr);
		if (!cmdGroupPtr)
		{
			break;
		}

		for (int index : vecIndexToMerge)
		{
			auto& ptrToMerge = commands_.at(index);
			cmdGroupPtr->Add(ptrToMerge);
			ptrToMerge.reset();
		}

		commands_.erase(std::remove_if(commands_.begin(), commands_.end(),
			[](const CommandSharedPtr& itor) { return !itor; }),
			commands_.end());

		commands_.emplace(commands_.begin() + vecIndexToMerge.front(), cmdGroupPtr);

	} while (true);
}

void CommandGroup::Add(const CommandSharedPtr& ptr) 
{ 
	commands_.emplace_back(ptr); 
}

int CommandGroup::GetCommandCount() const 
{ 
	return commands_.size(); 
}

CommandSharedPtr CommandGroup::GetCommandAt(size_t index) const 
{ 
	return (index < commands_.size()) ? commands_.at(index) : CommandSharedPtr(); 
}

void CommandGroup::EraseTargetCommandsIfContaining(const std::vector<Command*>& vecCmdToErase)
{
	if (!commands_.empty())
	{
		commands_.erase(std::remove_if(std::begin(commands_), std::end(commands_),
			[&](const CommandSharedPtr& itor) { return std::find(std::begin(vecCmdToErase), std::end(vecCmdToErase), itor.get()) != std::end(vecCmdToErase); }),
			std::end(commands_));
	}
}

int CommandGroup::UnpackFromString(const std::string& str, int startIndex)
{
	return 0;
}

std::string CommandGroup::PackToString() const
{
	std::string str_packed;
	Foreach([&](const CommandSharedPtr& cmd_ptr){ str_packed.append(cmd_ptr->PackToString()); });

	return str_packed;
}

bool CommandGroup::EqualTo(const Command& rh) const
{
	auto cmd_group_ptr = dynamic_cast<const CommandGroup*>(&rh);
	if (!cmd_group_ptr) { return false; }

	if (this == cmd_group_ptr) { return true; }

	const CommandGroup& cmd_group = *cmd_group_ptr;

	bool not_equal = true;

	const int cmd_count = GetCommandCount();
	if (cmd_count != cmd_group.GetCommandCount()) { return false; }

	if (cmd_count > 0)
	{
		for (int i = 0; i < cmd_count; ++i)
		{
			if (! commands_[i]->EqualTo(*cmd_group.commands_[i])) { return false; }
		}
	}

	return true;
}

std::string CommandGroup::_StringOfLog(const std::string& strPrefix) const
{
	std::stringstream ss;
	ss << strPrefix;

	ss << " Commands ( " << commands_.size() << " ):\n";

	int i = 1;

	this->Foreach([&](const CommandSharedPtr& ptr){
		std::stringstream ssTmp;
		ssTmp << std::string(52 - ((i > 99) ? 3 : (i > 9 ? 2 : 1)), ' ') << i++ << ": ";
		ss << ptr->StringOfLog(ssTmp.str()) << " \n";
	});

	std::string str = ss.str();
	return str.substr(0, str.length() - 2); // remove the last "\n"
}

PROTO_NAMESPACE_END