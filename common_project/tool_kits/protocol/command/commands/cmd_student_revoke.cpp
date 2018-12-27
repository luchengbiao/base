#include "cmd_student_revoke.h"

PROTO_NAMESPACE_BEGIN

IMPLEMENT_COMMAND_RUNTIME_INFO(CommandType::StudentRevoke, CmdStudentRevoke, CommandCellWithDraw)

DrawOpInfo CmdStudentRevoke::GetDrawInfo() const
{
	DrawOpInfo draw;

	draw.draw_op_type_ = DrawOpType::DrawOpUndoCb;

	return draw;
}

PROTO_NAMESPACE_END