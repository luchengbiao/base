#ifndef __PROTO_COMMAND_ENTITIES_IMPORT_MANUAL_H__
#define __PROTO_COMMAND_ENTITIES_IMPORT_MANUAL_H__
#include <string>
#include "protocol/macros.h"

PROTO_NAMESPACE_BEGIN

struct ImportChessManual
{
	std::string file_url_;
	int			game_mode_{ 0 };

	int			role_{ 0 };
	std::string white_name_;
	std::string white_url_;
	std::string black_name_;
	std::string black_url_;

	ImportChessManual() = default;
};

PROTO_NAMESPACE_END

#endif