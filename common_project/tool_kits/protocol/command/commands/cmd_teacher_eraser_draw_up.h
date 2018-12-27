#ifndef __PROTO_COMMANDS_CMD_TEACHER_ERASER_DRAW_UP_H__
#define __PROTO_COMMANDS_CMD_TEACHER_ERASER_DRAW_UP_H__
#include "../base/command_cell_with_draw.h"

PROTO_NAMESPACE_BEGIN

class CmdTeacherEraserDrawUp : public CommandCellWithDraw
{
	DECLARE_COMMAND_RUNTIME_INFO(CmdTeacherEraserDrawUp)

public:
	virtual DrawOpInfo GetDrawInfo() const override;
};

PROTO_NAMESPACE_END

#endif