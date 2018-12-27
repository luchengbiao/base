#include "cmd_teacher_eraser_draw_up.h"

PROTO_NAMESPACE_BEGIN

IMPLEMENT_COMMAND_RUNTIME_INFO(CommandType::TeacherEraserDrawUp, CmdTeacherEraserDrawUp, CommandCellWithDraw)

DrawOpInfo CmdTeacherEraserDrawUp::GetDrawInfo() const
{
	DrawOpInfo draw;

	draw.draw_op_type_ = EraserOpEnd;
	draw.x_ = this->FloatAt(0);
	draw.y_ = this->FloatAt(1);

	return draw;
}

PROTO_NAMESPACE_END