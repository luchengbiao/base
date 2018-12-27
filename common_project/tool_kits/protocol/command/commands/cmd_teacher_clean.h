#ifndef __PROTO_COMMANDS_CMD_TEACHER_CLEAN_H__
#define __PROTO_COMMANDS_CMD_TEACHER_CLEAN_H__
#include "../base/command_cell_with_draw.h"

PROTO_NAMESPACE_BEGIN

class CmdTeacherClean : public CommandCellWithDraw
{
	DECLARE_COMMAND_RUNTIME_INFO(CmdTeacherClean)

public:
	virtual DrawOpInfo GetDrawInfo() const override;
};

PROTO_NAMESPACE_END

#endif