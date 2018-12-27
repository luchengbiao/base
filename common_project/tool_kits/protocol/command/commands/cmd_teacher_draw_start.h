#ifndef __PROTO_COMMANDS_CMD_TEACHER_DRAW_START_H__
#define __PROTO_COMMANDS_CMD_TEACHER_DRAW_START_H__
#include "../base/command_cell_with_draw.h"

PROTO_NAMESPACE_BEGIN

class CmdTeacherDrawStart : public CommandCellWithDraw
{
	DECLARE_COMMAND_RUNTIME_INFO(CmdTeacherDrawStart)

public:
	virtual DrawOpInfo GetDrawInfo() const override;
};

PROTO_NAMESPACE_END

#endif