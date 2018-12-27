#include "cmd_teacher_revoke.h"

PROTO_NAMESPACE_BEGIN

IMPLEMENT_COMMAND_RUNTIME_INFO(CommandType::TeacherRevoke, CmdTeacherRevoke, CommandCellWithDraw)

DrawOpInfo CmdTeacherRevoke::GetDrawInfo() const
{
	DrawOpInfo draw;

	draw.draw_op_type_ = DrawOpType::DrawOpUndoCb;

	return draw;
}

PROTO_NAMESPACE_END