#ifndef __GAME_MODEL_COMMAND_HANDLER_COMMAND_HANDLER_H__
#define __GAME_MODEL_COMMAND_HANDLER_COMMAND_HANDLER_H__
#include <functional>
#include <list>
#include <QPointer>
#include "base_control/painter/paint_data.h"
#include "common/weak_callback/weak_callback.h"
#include "game/model/base/board/game_board_base_fwd.h"
#include "game/model/command_handler/command_conflict_level.h"
#include "game/model/command_handler/command_handler_fwd.h"
#include "game/public/enums/game_board_type.h"
#include "game/public/structs/cmd_group_with_executor.h"
#include "protocol/command/base/command.h"

class DrawBoard;
struct DrawOpInfo;

GAME_NAMESPACE_BEGIN

class GameBoardWrapper;
//typedef QPointer<GameBoardWrapper> GameBoardWrapperPtr;
typedef GameBoardWrapper* GameBoardWrapperPtr;

class CommandCache;
class CommandAndConflict;
class GameBoardPairs;
class GameActionResult;
class GameDrawBoardInfo;
class HeartbeatHelper;
class MD5SyncHelper;

class CommandHandler : public wcb::SupportWeakCallback<CommandHandler>
{
	typedef std::function<void(const CommandSharedPtr&)> CommandSender;
	typedef std::unique_ptr<CommandCache> CommandCachePtr;
	typedef std::unique_ptr<CommandAndConflict> CommandAndConflictPtr;
	typedef std::unique_ptr<GameBoardPairs> GameBoardPairsPtr;
	typedef std::unique_ptr<GameDrawBoardInfo> GameDrawBoardInfoPtr;
	typedef std::shared_ptr<MD5SyncHelper> MD5SyncHelperPtr;
	typedef std::shared_ptr<HeartbeatHelper> HeartbeatHelperPtr;
	typedef wcb::WeakCallbackFlagCancelable<CommandHandler> MyWeakCallbackFlag;

	friend class HeartbeatHelper;
	friend class MD5SyncHelper;
	
public:
	static CommandHandlerSharedPtr Create();

	~CommandHandler();

	CommandHandler(const CommandHandler&) = delete;
	CommandHandler& operator=(const CommandHandler&) = delete;

	void					SetCommandSender(const CommandSender& sender) { sender_ = sender; }

	void					SendCommand(const CommandSharedPtr& cmd_ptr);
	void					SendDrawCommand(const DrawOpInfo& draw_info);
	void					SendMarkCommand(const MarkOpInfo& mark_info);

	void					SetGameBoardWrapper(GameBoardWrapper* game_board_wrapper);

	void					HandleChannelMessgae(const std::string& msg, bool sender_is_self);
	void					HandleMasterCommand(const CommandSharedPtr& cmd_ptr);

	bool					TestSyncLocalWithMasterWhenMasterDone(const GameBoardType master_board_type, const GameActionResult& result_on_master);

	void					DelegateMasterGameBoard(const GameBoardPtr& game_board);
	void					DelegateLocalGameBoard(const GameBoardPtr& game_board);

	void					SwitchMasterToGameBoard(GameBoardType board_type);

	void					SetDrawBoardByTeacher(GameBoardType board_type);
	void					SetDrawBoardByStudent(GameBoardType board_type);

	bool					IsRequestingRecovery() const;
	bool					IsWaitingRecovery() const;

private:
	CommandHandler();

	CommandGroupSharedPtr	UnpackMessageIntoCmdGroup(const std::string& msg);

	bool					HandleMasterSyncCommand(const CommandSharedPtr& cmd_ptr);

	void					HandleMasterCommandOnGameBoard(const CommandSharedPtr& cmd_ptr, bool is_reenter, bool is_from_blocked_cache, bool is_recovery = false);
	bool					TryToAccumulateMD5OfMasterChessCommand(const CommandSharedPtr& cmd_ptr);
	CmdConflictLevel		CheckCommandConflictBetweenMasterAndLocal(const CommandSharedPtr& master_cmd_ptr);
	bool					IsAnyMasterCommandWaitingReenter() const;
	bool					CheckMasterCommandNeedingReenter(const CommandSharedPtr& cmd_ptr);
	void					ReenterMasterCommand(const CommandSharedPtr& cmd_ptr);

	void					TryToSwitchMasterGameBoardFirstly(const CommandSharedPtr& cmd_ptr);

	void					HandleMasterCommandOnDrawOrMarkBoard(const CommandSharedPtr& cmd_ptr);
	void					HandleMasterCommandSpecially(const CommandSharedPtr& cmd_ptr);

	void					RefreshGameBoardWithMaster(GameBoardType board_type, bool switch_to_game_board = true);

	DrawBoard*				DrawBoardOnGameBoard(GameBoardType board_type);

	void					TeacherDraw(const std::list<DrawOpInfo>&);
	void					StudentDraw(const std::list<DrawOpInfo>&);
	void					TeacherMark(int mark_icon, const QPoint& pos);

	void					StartRecovery(const CmdGroupWithExecutors& cmd_groups);
	void					BeforeRecovery();
	void					AfterRecovery();
	
private:
	CommandSender			sender_;

	GameBoardPairsPtr		board_pairs_{ nullptr };

	CommandAndConflictPtr	master_chess_cmd_being_handled_;
	CommandSharedPtr		master_chess_cmd_waiting_reenter_{ nullptr };
	CommandCachePtr			master_chess_cmd_blocked_reenter_{ nullptr }; // master chess commands blocked after last command waiting reenter

	CommandCachePtr			local_chess_cmd_cache_{ nullptr };

	GameBoardType			current_master_board_type_{ GameBoardType::None };

	GameBoardWrapperPtr		game_board_wrapper_{ nullptr }; // passed from the outside
	GameDrawBoardInfoPtr	draw_board_by_tea_{ nullptr };
	GameDrawBoardInfoPtr	draw_board_by_stu_{ nullptr };

	MyWeakCallbackFlag		async_download_flag_;

	MD5SyncHelperPtr		md5_sync_helper_;
	HeartbeatHelperPtr		heartbeat_helper_;
};

GAME_NAMESPACE_END

#endif