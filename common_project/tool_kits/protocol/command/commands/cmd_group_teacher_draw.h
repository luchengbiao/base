#ifndef __CMD_GROUP_TEACHER_DRAW_H__
#define __CMD_GROUP_TEACHER_DRAW_H__
#include "protocol/command/base/command_group_with_draw_list.h"

PROTO_NAMESPACE_BEGIN

class CmdGroupTeacherDraw : public CommandGroupWithDrawList
{
	DECLARE_COMMAND_RUNTIME_INFO(CmdGroupTeacherDraw)

private:
	virtual std::list<DrawOpInfo>	_GetDrawList() override;
};

PROTO_NAMESPACE_END

#endif

