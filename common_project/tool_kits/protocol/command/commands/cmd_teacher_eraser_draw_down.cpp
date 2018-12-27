#include "cmd_teacher_eraser_draw_down.h"

PROTO_NAMESPACE_BEGIN

IMPLEMENT_COMMAND_RUNTIME_INFO(CommandType::TeacherEraserDrawDown, CmdTeacherEraserDrawDown, CommandCellWithDraw)

DrawOpInfo CmdTeacherEraserDrawDown::GetDrawInfo() const
{
	DrawOpInfo draw;

	draw.draw_op_type_ = EraserOpStart;
	draw.x_ = this->FloatAt(0);
	draw.y_ = this->FloatAt(1);

	return draw;
}

PROTO_NAMESPACE_END