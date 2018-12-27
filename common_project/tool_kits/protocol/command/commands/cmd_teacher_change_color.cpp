#include "cmd_teacher_change_color.h"

PROTO_NAMESPACE_BEGIN

IMPLEMENT_COMMAND_RUNTIME_INFO(CommandType::TeacherChangeColor, CmdTeacherChangeColor, CommandCellWithDraw)

DrawOpInfo CmdTeacherChangeColor::GetDrawInfo() const
{
	DrawOpInfo draw;

	draw.draw_op_type_ = DrawOpType::DrawOpPenColor;
	draw.pen_color_ = QColor(QString("#") + QString::fromStdString(this->StringAt(0)));

	return draw;
}

PROTO_NAMESPACE_END