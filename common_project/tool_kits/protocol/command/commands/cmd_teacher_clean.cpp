#include "cmd_teacher_clean.h"

PROTO_NAMESPACE_BEGIN

IMPLEMENT_COMMAND_RUNTIME_INFO(CommandType::TeacherClean, CmdTeacherClean, CommandCellWithDraw)

DrawOpInfo CmdTeacherClean::GetDrawInfo() const
{
	DrawOpInfo draw;

	draw.draw_op_type_ = DrawOpType::DrawOpClearCb;

	return draw;
}

PROTO_NAMESPACE_END