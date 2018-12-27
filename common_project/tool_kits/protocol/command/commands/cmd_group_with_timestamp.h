#ifndef __PROTO_COMMANDS_CMD_GROUP_WITH_TIEMSTAMP_H__
#define __PROTO_COMMANDS_CMD_GROUP_WITH_TIEMSTAMP_H__
#include "protocol/command/base/command_group.h"

PROTO_NAMESPACE_BEGIN

class CmdGroupWithTimestamp : public CommandGroup
{
	DECLARE_COMMAND_RUNTIME_INFO(CmdGroupWithTimestamp)

public:
	~CmdGroupWithTimestamp();

	virtual int			UnpackFromString(const std::string& str, int startIndex) override;
	virtual std::string PackToString() const override;

	virtual	void		UnpackGroup(bool needDecodeBase64, const CommandFactory& cmdFactory);

	unsigned int		Timestamp() const;
	void				SetTimestamp(unsigned int);

	bool				IsBeginningOfSection() const;
	bool				IsEndOfSection() const;

	virtual std::string	_StringOfLog(const std::string& strPrefix) const override;

private:
	friend class CmdGroupWithTimestampPrivate;
	std::unique_ptr<CmdGroupWithTimestampPrivate> impl_;
};

PROTO_NAMESPACE_END

#endif

