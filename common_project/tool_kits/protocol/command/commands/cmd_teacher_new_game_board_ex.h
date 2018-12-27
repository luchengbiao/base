#ifndef __PROTO_COMMANDS_CMD_TEACHER_NEW_GAME_BOARD_EX_H__
#define __PROTO_COMMANDS_CMD_TEACHER_NEW_GAME_BOARD_EX_H__
#include "../base/command_cell.h"

PROTO_NAMESPACE_BEGIN

struct NewGameBoardEx;
typedef std::unique_ptr<NewGameBoardEx> NewGameBoardExPtr;

class CmdTeacherNewGameBoardEx : public CommandCell
{
	DECLARE_COMMAND_RUNTIME_INFO(CmdTeacherNewGameBoardEx)

public:
	const NewGameBoardEx*	EntityOfCommand();

private:
	NewGameBoardExPtr		new_game_board_ex_;
};

PROTO_NAMESPACE_END

#endif