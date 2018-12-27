#include "command_group_with_draw_list.h"

PROTO_NAMESPACE_BEGIN

void CommandGroupWithDrawList::Add(const CommandSharedPtr& ptr)
{
	CommandGroup::Add(ptr);

	dirty_ = true;
}

const std::list<DrawOpInfo>& CommandGroupWithDrawList::GetDrawList()
{
	if (dirty_)
	{
		dirty_ = false;

		draw_list_.swap(this->_GetDrawList());
	}

	return draw_list_;
}

PROTO_NAMESPACE_END