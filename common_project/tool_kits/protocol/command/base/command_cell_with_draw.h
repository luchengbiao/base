#ifndef __PROTO_BASE_COMMAND_CELL_WITH_DRAW_H__
#define __PROTO_BASE_COMMAND_CELL_WITH_DRAW_H__
#include <vector>
#include "base_control/painter/paint_data.h"
#include "command_cell.h"

PROTO_NAMESPACE_BEGIN

class CommandCellWithDraw : public CommandCell
{
public:
	virtual DrawOpInfo	GetDrawInfo() const = 0;
};

PROTO_NAMESPACE_END

#endif