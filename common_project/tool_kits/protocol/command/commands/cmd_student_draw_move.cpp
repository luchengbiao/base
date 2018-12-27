#include "cmd_student_draw_move.h"

PROTO_NAMESPACE_BEGIN

IMPLEMENT_COMMAND_RUNTIME_INFO(CommandType::StudentDrawMove, CmdStudentDrawMove, CommandCellWithDraw)

DrawOpInfo CmdStudentDrawMove::GetDrawInfo() const
{
	DrawOpInfo draw;

	draw.draw_op_type_ = DrawOpType::DrawOpMove;
	draw.x_ = this->FloatAt(0);
	draw.y_ = this->FloatAt(1);

	return draw;
}

PROTO_NAMESPACE_END