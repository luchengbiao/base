#ifndef __GAME_MODEL_COMMAND_HANDLER_COMMAND_HANDLER_FWD_H__
#define __GAME_MODEL_COMMAND_HANDLER_COMMAND_HANDLER_FWD_H__
#include <memory>
#include "game/macros.h"

GAME_NAMESPACE_BEGIN

class CommandHandler;

typedef std::shared_ptr<CommandHandler> CommandHandlerSharedPtr;
typedef std::weak_ptr<CommandHandler> CommandHandlerWeakPtr;

GAME_NAMESPACE_END

#endif