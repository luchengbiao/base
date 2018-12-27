#include "cmd_student_clean.h"

PROTO_NAMESPACE_BEGIN

IMPLEMENT_COMMAND_RUNTIME_INFO(CommandType::StudentClean, CmdStudentClean, CommandCellWithDraw)

DrawOpInfo CmdStudentClean::GetDrawInfo() const
{
	DrawOpInfo draw;

	draw.draw_op_type_ = DrawOpType::DrawOpClearCb;

	return draw;
}

PROTO_NAMESPACE_END