#include "cmd_group_teacher_eraser.h"
#include "../base/command_cell_with_draw.h"

PROTO_NAMESPACE_BEGIN

IMPLEMENT_COMMAND_RUNTIME_INFO(CommandType::TeacherEraserGroup, CmdGroupTeacherEraser, CmdGroupWithDrawList)

std::list<DrawOpInfo> CmdGroupTeacherEraser::_GetDrawList()
{
	std::list<DrawOpInfo> draw_list;

	for (const auto& cmdPtr : commands_)
	{
		switch (cmdPtr->Type())
		{
		case CommandType::TeacherEraserDrawDown:
		case CommandType::TeacherEraserDrawMove:
		case CommandType::TeacherEraserDrawUp:
			draw_list.emplace_back(std::static_pointer_cast<CommandCellWithDraw>(cmdPtr)->GetDrawInfo());
			break;
		}
	}

	return draw_list;
}

PROTO_NAMESPACE_END