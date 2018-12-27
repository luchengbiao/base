#ifndef __PROTO_BASE_COMMAND_GROUP_H__
#define __PROTO_BASE_COMMAND_GROUP_H__
#include <unordered_set>
#include <vector>
#include "command.h"

PROTO_NAMESPACE_BEGIN

class CommandFactory;

class CommandGroup : public Command
{
	DECLARE_COMMAND_RUNTIME_INFO(CommandGroup)

public:
	void				UnpackCommands(const std::string& str_raw, const CommandFactory& cmdFactory);

	void				MergeContinuousCommandsToGroup(const CommandFactory& cmdFactory, const std::unordered_set<CommandType>& setCmdToMerge, CommandType cmdGroupType);

	template<typename CmdPtrContainerT>
	void				AddCommands(CmdPtrContainerT&& cmd_ptrs);

	virtual	void		Add(const CommandSharedPtr& ptr);

	int					GetCommandCount() const;

	CommandSharedPtr	GetCommandAt(size_t index) const;

	void				EraseTargetCommandsIfContaining(const std::vector<Command*>& vecCmdToErase);
	
	template<typename Callable>
	void				Foreach(Callable&& callable) const;
	template<typename Callable>
	void				ForeachWithBreaker(Callable&& callable) const;

	// override
	virtual int			UnpackFromString(const std::string& str, int startIndex) override;
	virtual std::string PackToString() const override;

	virtual bool		EqualTo(const Command& rh) const override;

protected:
	virtual std::string	_StringOfLog(const std::string& strPrefix) const override;

	virtual void		MergeCommands(const CommandFactory& cmdFactory);

	std::vector<int>	GetContinousIndicesOfRelativeCommands(const std::unordered_set<CommandType>& setCmdType);

protected:
	std::vector<CommandSharedPtr> commands_;
};

template<typename CmdPtrContainerT>
void CommandGroup::AddCommands(CmdPtrContainerT&& cmd_ptrs)
{
	for (const auto& cmd_ptr : cmd_ptrs)
	{
		this->Add(cmd_ptr);
	}
}

template<typename Callable>
void CommandGroup::Foreach(Callable&& callable) const
{
	if (!commands_.empty())
	{
		for (const auto& cmd: commands_)
		{
			callable(cmd);
		}
	}
}

template<typename Callable>
void CommandGroup::ForeachWithBreaker(Callable&& callable) const
{
	if (!commands_.empty())
	{
		for (const auto& cmd : commands_)
		{
			if (callable(cmd))
			{
				break;
			}
		}
	}
}

PROTO_NAMESPACE_END

#endif