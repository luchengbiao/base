#ifndef __PROTO_BASE_COMMAND_GROUP_WITH_DRAW_LIST_H__
#define __PROTO_BASE_COMMAND_GROUP_WITH_DRAW_LIST_H__
#include "base_control/painter/paint_data.h"
#include "protocol/command/base/command_group.h"

PROTO_NAMESPACE_BEGIN

class CommandGroupWithDrawList : public CommandGroup
{
public:
	virtual	void					Add(const CommandSharedPtr& ptr) override;

	const std::list<DrawOpInfo>&	GetDrawList();

protected:
	virtual std::list<DrawOpInfo>	_GetDrawList() = 0;

private:
	std::list<DrawOpInfo>			draw_list_;
	bool							dirty_{ false };
};

PROTO_NAMESPACE_END

#endif