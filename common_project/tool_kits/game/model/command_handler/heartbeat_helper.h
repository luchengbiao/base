#ifndef __GAME_MODEL_COMMAND_HANDLER_HERATBEAT_HELPER_H__
#define __GAME_MODEL_COMMAND_HANDLER_HERATBEAT_HELPER_H__
#include <QObject>
#include "base/callback/callback.h" // for StdClosure
#include "common/weak_callback/weak_callback.h"
#include "game/model/command_handler/command_handler_fwd.h"
#include "protocol/command/base/command_fwd.h"

GAME_NAMESPACE_BEGIN

class HeartbeatHelper : public QObject, public wcb::SupportWeakCallback<HeartbeatHelper>
{
	Q_OBJECT

	typedef wcb::WeakCallbackFlagCancelable<HeartbeatHelper> MyWeakCallbackFlag;

	friend class CommandHandler;

public:
	static std::shared_ptr<HeartbeatHelper> Create(const CommandHandlerWeakPtr& command_handler);

	/**
	* true: all of the commands in group are Heartbeat
	* false: at least one of the commands in group is not Heartbeat
	*/
	bool					TryToHandleMasterHeartbeatCommands(const CommandGroupSharedPtr& cmd_group, bool sender_is_self);

	void					Stop();

signals:
	void					SignalClosure(StdClosure);

private slots:
	void					SlotClosure(StdClosure);

private:
	HeartbeatHelper(const CommandHandlerWeakPtr& command_handler);

	void					HandleMasterHeartbeatCommand(const CommandSharedPtr& cmd_ptr, bool sender_is_self);

private:
	MyWeakCallbackFlag		heartbeat_flag_;

	CommandHandlerWeakPtr	command_handler_; // use weak_ptr to avoid circular reference between objects of HeartbeatHelper and CommandHandler
};


GAME_NAMESPACE_END

#endif