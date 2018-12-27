#ifndef __PROTO_COMMANDS_CMD_TEACHER_CHANGE_COLOR_H__
#define __PROTO_COMMANDS_CMD_TEACHER_CHANGE_COLOR_H__
#include "../base/command_cell_with_draw.h"

PROTO_NAMESPACE_BEGIN

class CmdTeacherChangeColor : public CommandCellWithDraw
{
	DECLARE_COMMAND_RUNTIME_INFO(CmdTeacherChangeColor)

public:
	virtual DrawOpInfo GetDrawInfo() const override;
};

PROTO_NAMESPACE_END

#endif