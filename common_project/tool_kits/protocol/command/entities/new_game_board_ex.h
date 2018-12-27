#ifndef __PROTO_COMMAND_ENTITIES_NEW_GAME_BOARD_EX_H__
#define __PROTO_COMMAND_ENTITIES_NEW_GAME_BOARD_EX_H__
#include <string>
#include "protocol/macros.h"

PROTO_NAMESPACE_BEGIN

struct NewGameBoardEx
{
	int			board_type_{ 0 };
	std::string game_name_;
	int			game_mode_{ 0 };

	int			role_{ 0 };
	int			order_{ 0 };
	int			layout_{ 0 };
	int			layout_index_{ 0 };
	std::string white_name_;
	std::string white_url_;
	std::string black_name_;
	std::string	black_url_;

	NewGameBoardEx() = default;
};

PROTO_NAMESPACE_END

#endif