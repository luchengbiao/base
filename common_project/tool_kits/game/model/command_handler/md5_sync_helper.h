#ifndef __GAME_MODEL_COMMAND_HANDLER_MD5_SYNC_HELPER_H__
#define __GAME_MODEL_COMMAND_HANDLER_MD5_SYNC_HELPER_H__
#include <string>
#include <unordered_map>
#include <vector>
#include <QObject>
#include "base/callback/callback.h" // for StdClosure
#include "common/weak_callback/weak_callback.h"
#include "game/model/command_handler/command_handler_fwd.h"
#include "game/public/structs/cmd_group_with_executor.h"
#include "protocol/command/base/command_fwd.h"

GAME_NAMESPACE_BEGIN

class MD5SyncHelper : public QObject, public wcb::SupportWeakCallback<MD5SyncHelper>
{
	Q_OBJECT

	typedef wcb::WeakCallbackFlagCancelable<MD5SyncHelper> MyWeakCallbackFlag;
	typedef std::unordered_map<std::string, std::wstring> Url2FileMap;
	typedef std::vector<CommandGroupSharedPtr> CommandGroups;
	typedef std::vector<std::string> Strings;
	
	friend class CommandHandler;

	enum class Step
	{
		Idle,
		Requesting,
		Downloading,
		Waiting,
	};

public:
	static std::shared_ptr<MD5SyncHelper> Create(const CommandHandlerWeakPtr& command_handler);

	bool					TryToHandleMasterSyncCommand(const CommandSharedPtr& cmd_ptr);
	bool					TryToAccumulateMD5OfMasterChessCommand(const CommandSharedPtr& cmd_ptr);

	inline bool				IsRequestingRecovery() const { return step_ >= Step::Requesting; }
	inline bool				IsWaitingRecovery() const { return step_ >= Step::Waiting; }

	void					BlockMessageAndCmdGroup(const std::string& msg, bool sender_is_self, const CommandGroupSharedPtr& cmd_group);

	void					Cancel();

signals:
	void					SignalClosure(StdClosure);

private slots:
	void					SlotClosure(StdClosure);

private:
	MD5SyncHelper(const CommandHandlerWeakPtr& command_handler);

	void					EmitSync();

	inline std::string		Token() const { return token_; }
	void					RequestWithToken(const std::string& token);
	void					Download(const std::string& file_url);

	void					ResetMD5();

	void					OnFileDownloaded(const std::string& file_url, const std::wstring& file_path);
	void					OnFileParsed(const CmdGroupWithExecutors& cmd_groups);
	void					StartRecovery(const CmdGroupWithExecutors& cmd_groups);

private:
	std::string				md5_of_all_master_chess_cmd_;
	std::string				md5_temp_to_sync_;

	std::string				token_;
	Url2FileMap				url_2_file_;
	Step					step_{ Step::Idle };
	CommandGroups			cmd_groups_blocked_;
	Strings					tea_msg_blocked_;
	Strings					stu_msg_blocked_;

	MyWeakCallbackFlag		recovery_flag_;

	CommandHandlerWeakPtr	command_handler_; // use weak_ptr to avoid circular reference between objects of MD5SyncHelper and CommandHandler
};

GAME_NAMESPACE_END

#endif