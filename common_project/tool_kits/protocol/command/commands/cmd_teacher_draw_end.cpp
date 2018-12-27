#include "cmd_teacher_draw_end.h"

PROTO_NAMESPACE_BEGIN

IMPLEMENT_COMMAND_RUNTIME_INFO(CommandType::TeacherDrawEnd, CmdTeacherDrawEnd, CommandCellWithDraw)

DrawOpInfo CmdTeacherDrawEnd::GetDrawInfo() const
{
	DrawOpInfo draw;

	draw.draw_op_type_ = DrawOpType::DrawOpEnd;
	draw.x_ = this->FloatAt(0);
	draw.y_ = this->FloatAt(1);

	return draw;
}

PROTO_NAMESPACE_END