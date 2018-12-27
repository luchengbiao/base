#ifndef __CMD_GROUP_TEACHER_ERASER_H__
#define __CMD_GROUP_TEACHER_ERASER_H__
#include "protocol/command/base/command_group_with_draw_list.h"

PROTO_NAMESPACE_BEGIN

class CmdGroupTeacherEraser : public CommandGroupWithDrawList
{
	DECLARE_COMMAND_RUNTIME_INFO(CmdGroupTeacherEraser)

private:
	virtual std::list<DrawOpInfo>	_GetDrawList() override;
};

PROTO_NAMESPACE_END

#endif

