#include "cmd_teacher_eraser_draw_move.h"

PROTO_NAMESPACE_BEGIN

IMPLEMENT_COMMAND_RUNTIME_INFO(CommandType::TeacherEraserDrawMove, CmdTeacherEraserDrawMove, CommandCellWithDraw)

DrawOpInfo CmdTeacherEraserDrawMove::GetDrawInfo() const
{
	DrawOpInfo draw;

	draw.draw_op_type_ = EraserOpMove;
	draw.x_ = this->FloatAt(0);
	draw.y_ = this->FloatAt(1);

	return draw;
}

PROTO_NAMESPACE_END