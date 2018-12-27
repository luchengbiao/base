#include "command_handler.h"
#include <chrono>
#include <vector>
#include <deque>
#include <sstream>
#include "base/util/base64.h"
#include "base/util/string_util.h"
#include "base/util/string_number_conversions.h"
#include "base_control/painter/draw_board.h"
#include "base_control/painter/base_mark_widget.h"
#include "game_board_wrapper.h"
#include "game/model/base/action/game_action_result.h"
#include "game/model/base/board/game_board_base.h"
#include "game/model/base/proto_entities_view/new_game_board_ex_view.h"
#include "game/public/public_setting/public_setting.h"
#include "log/log.h"
#include "protocol/command/base/command_cell_with_draw.h"
#include "protocol/command/base/command_group_with_draw_list.h"
#include "protocol/command/commands/cmd_student_draw_end.h"
#include "protocol/command/commands/cmd_student_draw_move.h"
#include "protocol/command/commands/cmd_student_draw_start.h"
#include "protocol/command/commands/cmd_teacher_draw_end.h"
#include "protocol/command/commands/cmd_teacher_draw_move.h"
#include "protocol/command/commands/cmd_teacher_draw_start.h"
#include "protocol/command/commands/cmd_teacher_eraser_draw_down.h"
#include "protocol/command/commands/cmd_teacher_eraser_draw_move.h"
#include "protocol/command/commands/cmd_teacher_eraser_draw_up.h"
#include "protocol/command/commands/cmd_teacher_new_game_board_ex.h"
#include "protocol/command/commands/cmd_teacher_update_mark_on_board.h"
#include "protocol/command/commands/cmd_teacher_import_chess_manual.h"
#include "protocol/command/command_factory_default.h"
#include "protocol/command/entities/import_chess_manual.h"
#include "protocol/command/entities/new_game_board_ex.h"
#include "command_checker.h"
#include "md5_sync_helper.h"
#include "heartbeat_helper.h"

using namespace proto;

#ifdef _DEBUG
#define PRINT_MESSAGE_COMMAND_LOG 1
#endif

GAME_NAMESPACE_BEGIN

inline static bool CommandIsRelatedToChess(const CommandSharedPtr& cmd_ptr) { return CommandChecker::CommandIsRelatedToChess(cmd_ptr->Type()); }
inline static bool CommandNeedsToBeHandledInGameBoard(const CommandSharedPtr& cmd_ptr) { return CommandChecker::CommandNeedsToBeHandledInGameBoard(cmd_ptr->Type()); }
inline static bool CommandInfluencesDrawOrMarkBoard(const CommandSharedPtr& cmd_ptr) { return CommandChecker::CommandInfluencesDrawOrMarkBoard(cmd_ptr->Type()); }

struct GameBoardPair
{
	GameBoardType	board_type_{ GameBoardType::None };

	GameBoardPtr	master_model_;
	GameBoardPtr	local_model_;
};

class GameBoardPairs : public std::vector<GameBoardPair>
{
public:
	inline void PushMaster(const GameBoardPtr& game_board)
	{
		auto pair_ptr = PairByType(game_board->BoardType());

		if (pair_ptr)
		{
			pair_ptr->master_model_ = game_board;
		}
		else
		{
			GameBoardPair pair;
			pair.board_type_ = game_board->BoardType();
			pair.master_model_ = game_board;

			push_back(pair);
		}
	}

	inline void PushLocal(const GameBoardPtr& game_board)
	{
		auto pair_ptr = PairByType(game_board->BoardType());

		if (pair_ptr)
		{
			pair_ptr->local_model_ = game_board;
		}
		else
		{
			GameBoardPair pair;
			pair.board_type_ = game_board->BoardType();
			pair.local_model_ = game_board;

			push_back(pair);
		}
	}

	inline GameBoardPtr MasterModelOfGameBoard(GameBoardType board_type)
	{
		auto pair_ptr = PairByType(board_type);

		return pair_ptr ? pair_ptr->master_model_ : GameBoardPtr();
	}

private:
	inline GameBoardPair* PairByType(GameBoardType board_type)
	{
		for (auto& pair : *this)
		{
			if (pair.board_type_ == board_type)
			{
				return &pair;
			}
		}

		return nullptr;
	}
};

class CommandCache : public std::deque<CommandSharedPtr>
{
public:
	inline void PushCommand(const CommandSharedPtr& cmd_ptr)
	{
		push_back(cmd_ptr);
	}

	inline void PopFrontCommand()
	{
		if (!empty())
		{
			pop_front();
		}
	}

	inline CommandSharedPtr FrontCommand() const { return empty() ? CommandSharedPtr() : front(); }
};

class CommandAndConflict
{
public:
	CommandAndConflict() = default;

	inline void	Reset(const CommandSharedPtr& cmd_ptr, const CmdConflictLevel conflict)
	{
		cmd_ptr_ = cmd_ptr;
		conflict_ = conflict;
	}

	inline void	Reset()
	{
		cmd_ptr_.reset();
		conflict_ = CmdConflictLevel::None;
	}

	inline bool IsConflict() const
	{
		return (conflict_ == CmdConflictLevel::InsideOfCurrentGame || conflict_ == CmdConflictLevel::SwitchedToAnotherGame);
	}

	CommandSharedPtr	cmd_ptr_;
	CmdConflictLevel	conflict_{ CmdConflictLevel::None };
};

class GameDrawBoardInfo
{
public:
	inline GameBoardType	BoardType() const { return board_type_; }
	inline void				SetDrawBoard(GameBoardType board_type, DrawBoard* draw_board)
	{
		board_type_ = board_type;
		draw_board_ = draw_board;
	}
	inline QPointer<DrawBoard> GetDrawBoard() const { return draw_board_; }

private:
	GameBoardType		board_type_{ GameBoardType::None };
	QPointer<DrawBoard> draw_board_;
};

CommandHandlerSharedPtr CommandHandler::Create()
{
	CommandHandlerSharedPtr ptr(new CommandHandler);

	if (ptr) 
	{ 
		auto wk_ptr = ptr->GetWeakPtr();
		ptr->async_download_flag_.SetWeakPtr(wk_ptr);
		ptr->md5_sync_helper_ = MD5SyncHelper::Create(wk_ptr);
		ptr->heartbeat_helper_ = HeartbeatHelper::Create(wk_ptr);
	}

	return ptr;
}

CommandHandler::CommandHandler()
: board_pairs_(new GameBoardPairs)
, master_chess_cmd_being_handled_(new CommandAndConflict)
, master_chess_cmd_blocked_reenter_(new CommandCache)
, local_chess_cmd_cache_(new CommandCache)
, draw_board_by_tea_(new GameDrawBoardInfo)
, draw_board_by_stu_(new GameDrawBoardInfo)
{}

CommandHandler::~CommandHandler() = default;

void CommandHandler::SendCommand(const CommandSharedPtr& cmd_ptr)
{
	if (CommandIsRelatedToChess(cmd_ptr))
	{
		if (md5_sync_helper_->IsRequestingRecovery())
		{
			return;
		}

		local_chess_cmd_cache_->PushCommand(cmd_ptr);
	}

	if (sender_) { sender_(cmd_ptr); }
}

void CommandHandler::SendDrawCommand(const DrawOpInfo& draw_info)
{
	CommandSharedPtr cmd_ptr;
	const bool is_tea = ClientTypeIsTeacher();

	switch (draw_info.draw_op_type_)
	{
	case DrawOpType::DrawOpStart:
		cmd_ptr = is_tea ? CmdTeacherDrawStart::CreateHomogeneously() : CmdStudentDrawStart::CreateHomogeneously();
		break;

	case DrawOpType::DrawOpMove:
		cmd_ptr = is_tea ? CmdTeacherDrawMove::CreateHomogeneously() : CmdStudentDrawMove::CreateHomogeneously();
		break;

	case DrawOpType::DrawOpEnd:
		cmd_ptr = is_tea ? CmdTeacherDrawEnd::CreateHomogeneously() : CmdStudentDrawEnd::CreateHomogeneously();
		break;

	case DrawOpType::EraserOpStart:
		cmd_ptr = CmdTeacherEraserDrawDown::CreateHomogeneously();
		break;

	case DrawOpType::EraserOpMove:
		cmd_ptr = CmdTeacherEraserDrawMove::CreateHomogeneously();
		break;

	case DrawOpType::EraserOpEnd:
		cmd_ptr = CmdTeacherEraserDrawUp::CreateHomogeneously();
		break;

	default:
		break;
	}

	if (cmd_ptr)
	{
		cmd_ptr->PushVariants(float(draw_info.x_), float(draw_info.y_));

		this->SendCommand(cmd_ptr);
	}
}

void  CommandHandler::SendMarkCommand(const MarkOpInfo& mark_info)
{
	if (ClientTypeIsTeacher())
	{
		this->SendCommand(CmdTeacherUpdateMarkOnBoard::CreateWithVariants(nbase::Int64ToString(UserId()), int(mark_info.draw_op_type_), int(mark_info.x_), int(mark_info.y_)));
	}
}

void CommandHandler::SetGameBoardWrapper(GameBoardWrapper* game_board_wrapper)
{
	game_board_wrapper_ = game_board_wrapper;
}

void CommandHandler::HandleChannelMessgae(const std::string& msg, bool sender_is_self)
{
	auto cmd_group = UnpackMessageIntoCmdGroup(msg);

	do 
	{
		bool heartbeat = heartbeat_helper_->TryToHandleMasterHeartbeatCommands(cmd_group, sender_is_self);

		if (heartbeat) { break; }

		if (md5_sync_helper_->IsWaitingRecovery())
		{
			md5_sync_helper_->BlockMessageAndCmdGroup(msg, sender_is_self, cmd_group);
			break;
		}

		cmd_group->Foreach([this, &heartbeat](const CommandSharedPtr& cmd_ptr){
#if PRINT_MESSAGE_COMMAND_LOG == 1
			LOG_NFLT("{0}") << cmd_ptr->StringOfLog("");
#endif

			this->HandleMasterCommand(cmd_ptr);
		});

	} while (false);
}

CommandGroupSharedPtr CommandHandler::UnpackMessageIntoCmdGroup(const std::string& msg)
{
	std::string msg_decoded = msg;
	if (ChannelMessageIsEncoded()) { nbase::Base64Decode(msg, &msg_decoded); }

	CommandGroupSharedPtr cmd_group = proto::CommandGroup::Create();
	cmd_group->UnpackCommands(msg_decoded, DefaultCommandFactory());

	return cmd_group;
}

void CommandHandler::HandleMasterCommand(const CommandSharedPtr& cmd_ptr)
{
	do 
	{
		if (HandleMasterSyncCommand(cmd_ptr)) { break; }

		this->HandleMasterCommandOnGameBoard(cmd_ptr, false, false);

	} while (false);
}

bool CommandHandler::HandleMasterSyncCommand(const CommandSharedPtr& cmd_ptr)
{
	return md5_sync_helper_->TryToHandleMasterSyncCommand(cmd_ptr);
}

void CommandHandler::HandleMasterCommandOnGameBoard(const CommandSharedPtr& cmd_ptr, bool is_reenter, bool is_from_blocked_cache, bool is_recovery)
{
	do
	{
		if (CommandIsRelatedToChess(cmd_ptr))
		{
			if (!UsageTypeIsPlayback())
			{
				if (!is_reenter)
				{
					if (!is_from_blocked_cache)
					{
						this->TryToAccumulateMD5OfMasterChessCommand(cmd_ptr);

						if (IsAnyMasterCommandWaitingReenter())
						{
							master_chess_cmd_blocked_reenter_->PushCommand(cmd_ptr);

							break;
						}
					}

					if (CheckMasterCommandNeedingReenter(cmd_ptr))
					{
						break;
					}
				}

				//check the conflict level between local and the master command which will be handed right now
				auto conflict = this->CheckCommandConflictBetweenMasterAndLocal(cmd_ptr);
				master_chess_cmd_being_handled_->Reset(cmd_ptr, conflict);

				if (master_chess_cmd_being_handled_->IsConflict())
				{
					if (game_board_wrapper_) { game_board_wrapper_->OnLocalCommandConflictWithMaster(); }

					this->RefreshGameBoardWithMaster(current_master_board_type_, true);
				}
			}

			if (CommandNeedsToBeHandledInGameBoard(cmd_ptr))
			{
				// first switch to target game board before new
				this->TryToSwitchMasterGameBoardFirstly(cmd_ptr);

				auto master_board = board_pairs_->MasterModelOfGameBoard(current_master_board_type_);
				if (master_board) { master_board->DoCommand(cmd_ptr, !is_recovery); }
			}
		}

		if (CommandInfluencesDrawOrMarkBoard(cmd_ptr))
		{
			this->HandleMasterCommandOnDrawOrMarkBoard(cmd_ptr);
		}

		this->HandleMasterCommandSpecially(cmd_ptr);

		master_chess_cmd_being_handled_->Reset();
	} while (false);
}

bool CommandHandler::TryToAccumulateMD5OfMasterChessCommand(const CommandSharedPtr& cmd_ptr)
{
	return md5_sync_helper_->TryToAccumulateMD5OfMasterChessCommand(cmd_ptr);
}

CmdConflictLevel  CommandHandler::CheckCommandConflictBetweenMasterAndLocal(const CommandSharedPtr& master_cmd_ptr)
{
	auto conflict = CmdConflictLevel::NoConflict_NoLocalCmdCached;

	// core thought:
	// check whether the master command is equal to first local command(if exist)
	// if equal: command sequence is OK
	// if not equal: command sequence is in conflict
	do
	{
		if (local_chess_cmd_cache_->empty()) { break; }

		auto front_local_command = local_chess_cmd_cache_->FrontCommand();

		if (front_local_command->EqualTo(*master_cmd_ptr)) 
		{ 
			conflict = CmdConflictLevel::NoConflict_SameWithTheFirstLocalCmdCached;
			local_chess_cmd_cache_->PopFrontCommand();

			break; 
		}

		auto conflict_master = ConflictLevelOfCmdType(master_cmd_ptr->Type(), CmdConflictLevel::InsideOfCurrentGame);
		auto conflict_local = ConflictLevelOfCmdType(front_local_command->Type(), CmdConflictLevel::InsideOfCurrentGame);

		conflict = conflict_master >= conflict_local ? conflict_master : conflict_local;

	} while (false);

	if (conflict != CmdConflictLevel::NoConflict_SameWithTheFirstLocalCmdCached)
	{
		if (!local_chess_cmd_cache_->empty())
		{
			local_chess_cmd_cache_->clear();
		}
	}

	return conflict;
}

bool CommandHandler::IsAnyMasterCommandWaitingReenter() const
{
	return master_chess_cmd_waiting_reenter_.use_count();
}

bool CommandHandler::CheckMasterCommandNeedingReenter(const CommandSharedPtr& cmd_ptr)
{
	bool need_reenter = false;

	const auto cmd_type = cmd_ptr->Type();
	if (cmd_type == CommandType::TeacherImportChessManual)
	{
		auto import_manual_cmd = std::static_pointer_cast<proto::CmdTeacherImportChessManual>(cmd_ptr);
		if (!import_manual_cmd->GetOrCreateCommandGroup(proto::DefaultCommandFactory()))
		{
			need_reenter = true;
			
			D_ASSERT(master_chess_cmd_waiting_reenter_ == nullptr, "master_chess_cmd_waiting_reenter_ should be null now");
			master_chess_cmd_waiting_reenter_ = import_manual_cmd;

			if (game_board_wrapper_)
			{
				game_board_wrapper_->DownloadChessManual(cmd_ptr->StringAt(0), async_download_flag_.ToWeakCallback([=](const std::string&, const std::wstring& file_path){
					import_manual_cmd->SetChessManualFilePath(file_path);
					import_manual_cmd->GetOrCreateCommandGroup(proto::DefaultCommandFactory());

					this->ReenterMasterCommand(import_manual_cmd);
				}));
			}
		}
	}

	return need_reenter;
}

void CommandHandler::ReenterMasterCommand(const CommandSharedPtr& cmd_ptr)
{
	D_ASSERT(master_chess_cmd_waiting_reenter_ == cmd_ptr, "master_chess_cmd_waiting_reenter_ should be same with cmd_ptr");
	master_chess_cmd_waiting_reenter_.reset();

	this->HandleMasterCommandOnGameBoard(cmd_ptr, true, false);

	while (!master_chess_cmd_blocked_reenter_->empty())
	{
		if (!IsAnyMasterCommandWaitingReenter())
		{
			auto front_cmd = master_chess_cmd_blocked_reenter_->FrontCommand();
			master_chess_cmd_blocked_reenter_->PopFrontCommand();

			this->HandleMasterCommandOnGameBoard(front_cmd, false, true);
		}
	}
}

void CommandHandler::TryToSwitchMasterGameBoardFirstly(const CommandSharedPtr& cmd_ptr)
{
	auto game_board_type = current_master_board_type_;

	const auto cmd_type = cmd_ptr->Type();
	if (cmd_type == CommandType::TeacherNewGameBoard)
	{
		game_board_type = static_cast<GameBoardType>(cmd_ptr->IntAt(0));
	}
	else if (cmd_type == CommandType::TeacherNewGameBoardEx)
	{
		const auto new_board_ex_cmd = std::static_pointer_cast<CmdTeacherNewGameBoardEx>(cmd_ptr);
		const auto entity = new_board_ex_cmd->EntityOfCommand();
		const auto entity_view = NewGameBoardExView(*entity);
		game_board_type = entity_view.BoardType();

		if (entity_view.IsCreatingWithCurrentBoard())
		{
			game_board_type = current_master_board_type_;
		}
		else if (entity_view.IsCreatingWithHistoryBoard())
		{
			auto master_board = board_pairs_->MasterModelOfGameBoard(current_master_board_type_);
			if (master_board)
			{
				game_board_type = master_board->BoardTypeOfHistoryGame(entity_view.IndexOfHistoryBoard());
			}
		}
	}
	else if (cmd_type == CommandType::TeacherSwitchToHistoryBoard)
	{
		auto master_board = board_pairs_->MasterModelOfGameBoard(current_master_board_type_);
		if (master_board)
		{
			game_board_type = master_board->BoardTypeOfHistoryGame(cmd_ptr->IntAt(0));
		}
	}
	else if (cmd_type == CommandType::TeacherImportChessManual)
	{
		const auto import_manual_cmd = std::static_pointer_cast<CmdTeacherImportChessManual>(cmd_ptr);
		import_manual_cmd->GetOrCreateCommandGroup(proto::DefaultCommandFactory());

		const auto new_board_cmd = import_manual_cmd->FindNewBoardCommand();
		if (new_board_cmd)
		{
			this->TryToSwitchMasterGameBoardFirstly(new_board_cmd); // recursive
		}

		return; // return to avoid reseting to back after recursive
	}
	else
	{
		if (master_chess_cmd_being_handled_->conflict_ == CmdConflictLevel::SwitchedToAnotherGame)
		{
			this->SwitchMasterToGameBoard(current_master_board_type_);
			if (game_board_wrapper_) { game_board_wrapper_->SwitchMasterToGameBoard(current_master_board_type_); }
		}
	}

	if (game_board_type != current_master_board_type_ && game_board_type != GameBoardType::None)
	{
		this->SwitchMasterToGameBoard(game_board_type);
		if (game_board_wrapper_) { game_board_wrapper_->SwitchMasterToGameBoard(game_board_type); }
	}
}

void CommandHandler::HandleMasterCommandOnDrawOrMarkBoard(const CommandSharedPtr& cmd_ptr)
{
	const auto cmd_tpye = cmd_ptr->Type();
	switch (cmd_tpye)
	{
	case CommandType::TeacherChangeGameBoard:
	{
		const auto game_board_type = static_cast<GameBoardType>(cmd_ptr->IntAt(0));
		this->SwitchMasterToGameBoard(game_board_type);
		if (game_board_wrapper_) { game_board_wrapper_->SwitchMasterToGameBoard(game_board_type); }

		DrawBoard* draw_board = this->DrawBoardOnGameBoard(game_board_type);

		draw_board_by_tea_->SetDrawBoard(game_board_type, draw_board);
		draw_board_by_stu_->SetDrawBoard(game_board_type, draw_board);

		break;
	}

	case CommandType::TeacherChangeBoardPager:
	case CommandType::TeacherChangePhotoPage:
	case CommandType::TeacherChangeToPpt:
		draw_board_by_tea_->SetDrawBoard(GameBoardType::None, nullptr);
		draw_board_by_stu_->SetDrawBoard(GameBoardType::None, nullptr);
		break;

	case CommandType::StudentChangeGameBoard:
	{
		const auto game_board_type = static_cast<GameBoardType>(cmd_ptr->IntAt(0));
		DrawBoard* draw_board = this->DrawBoardOnGameBoard(game_board_type);
		draw_board_by_stu_->SetDrawBoard(game_board_type, draw_board);
		
		break;
	}
		
	case CommandType::StudentChangeBoardPager:
	case CommandType::StudentChangePhotoPager:
	case CommandType::StudentChangePptPager:
		draw_board_by_stu_->SetDrawBoard(GameBoardType::None, nullptr);
		break;

	case CommandType::TeacherDrawGroup:
	case CommandType::TeacherEraserGroup:
		this->TeacherDraw(std::static_pointer_cast<CommandGroupWithDrawList>(cmd_ptr)->GetDrawList());
		break;

	case CommandType::TeacherDrawStart:
	case CommandType::TeacherDrawMove:
	case CommandType::TeacherDrawEnd:
	case CommandType::TeacherEraserDrawDown:
	case CommandType::TeacherEraserDrawMove:
	case CommandType::TeacherEraserDrawUp:
		this->TeacherDraw({ std::static_pointer_cast<CommandCellWithDraw>(cmd_ptr)->GetDrawInfo() });
		break;

	case CommandType::StudentDrawGroup:
		this->StudentDraw(std::static_pointer_cast<CommandGroupWithDrawList>(cmd_ptr)->GetDrawList());
		break;

	case CommandType::StudentDrawStart:
	case CommandType::StudentDrawMove:
	case CommandType::StudentDrawEnd:
		this->StudentDraw({ std::static_pointer_cast<CommandCellWithDraw>(cmd_ptr)->GetDrawInfo() });
		break;

	case CommandType::TeacherUpdateMarkOnBoard:
		this->TeacherMark(cmd_ptr->IntAt(1), QPoint(cmd_ptr->IntAt(2), cmd_ptr->IntAt(3)));
		break;

	default:
		break;
	}
}

void CommandHandler::HandleMasterCommandSpecially(const CommandSharedPtr& cmd_ptr)
{
	if (game_board_wrapper_) { game_board_wrapper_->HandleMasterCommandSpecially(cmd_ptr); }
}

void CommandHandler::TeacherDraw(const std::list<DrawOpInfo>& draw_list)
{
	if (!ClientTypeIsTeacher() || UsageTypeIsPlayback())
	{
		auto draw_board = draw_board_by_tea_->GetDrawBoard();

		if (!draw_board && draw_board_by_tea_->BoardType() != GameBoardType::None && game_board_wrapper_) // maybe not initialized if TeacherChangeGameBoard command lost
		{
			draw_board = game_board_wrapper_->DrawBoardOnGameBoard(draw_board_by_tea_->BoardType());
			draw_board_by_tea_->SetDrawBoard(draw_board_by_tea_->BoardType(), draw_board);
		}

		if (draw_board)
		{
			draw_board->OnTeaDrawInfos(draw_list);
		}
	}
}

void CommandHandler::StudentDraw(const std::list<DrawOpInfo>& draw_list)
{
	if (!ClientTypeIsStudent() || UsageTypeIsPlayback())
	{
		auto draw_board = draw_board_by_stu_->GetDrawBoard();

		if (!draw_board && draw_board_by_stu_->BoardType() != GameBoardType::None && game_board_wrapper_) // maybe not initialized if TeacherChangeGameBoard command lost
		{
			draw_board = game_board_wrapper_->DrawBoardOnGameBoard(draw_board_by_stu_->BoardType());
			draw_board_by_stu_->SetDrawBoard(draw_board_by_stu_->BoardType(), draw_board);
		}

		if (draw_board)
		{
			draw_board->OnStuDrawInfos(draw_list);
		}
	}
}

void CommandHandler::TeacherMark(int mark_icon, const QPoint& pos)
{
	if (!ClientTypeIsTeacher() || UsageTypeIsPlayback())
	{
		if (draw_board_by_stu_->BoardType() != GameBoardType::None && game_board_wrapper_)
		{
			auto mark_board = game_board_wrapper_->MarkBoardOnGameBoard(draw_board_by_stu_->BoardType());
			if (mark_board)
			{
				mark_board->OnUpdateMarkIcon(mark_icon, pos);
			}
		}
	}
}

DrawBoard* CommandHandler::DrawBoardOnGameBoard(GameBoardType board_type)
{
	return game_board_wrapper_ ? game_board_wrapper_->DrawBoardOnGameBoard(board_type) : nullptr;
}

void CommandHandler::DelegateMasterGameBoard(const GameBoardPtr& game_board)
{
	if (!game_board) { return; }

	board_pairs_->PushMaster(game_board);
}

void CommandHandler::DelegateLocalGameBoard(const GameBoardPtr& game_board)
{
	if (!game_board) { return; }

	board_pairs_->PushLocal(game_board);
}

void CommandHandler::SwitchMasterToGameBoard(GameBoardType board_type)
{
	if (current_master_board_type_ != board_type)
	{
		current_master_board_type_ = board_type;
	}
}

void CommandHandler::SetDrawBoardByTeacher(GameBoardType board_type)
{
	draw_board_by_tea_->SetDrawBoard(board_type, DrawBoardOnGameBoard(board_type));
}

void CommandHandler::SetDrawBoardByStudent(GameBoardType board_type)
{
	draw_board_by_stu_->SetDrawBoard(board_type, DrawBoardOnGameBoard(board_type));
}

bool CommandHandler::TestSyncLocalWithMasterWhenMasterDone(const GameBoardType master_board_type, const GameActionResult& result_on_master)
{
	D_ASSERT(master_board_type == current_master_board_type_, "the master_board_type passed in should be same with my current_master_board_type_");
	D_ASSERT(result_on_master.GetCommandPtr() == master_chess_cmd_being_handled_->cmd_ptr_, "the command if result_on_master should be same with that in master_chess_cmd_being_handled_");

	return UsageTypeIsPlayback() || master_chess_cmd_being_handled_->conflict_ != CmdConflictLevel::NoConflict_SameWithTheFirstLocalCmdCached;
}

bool CommandHandler::IsRequestingRecovery() const
{
	return md5_sync_helper_->IsRequestingRecovery();
}

bool CommandHandler::IsWaitingRecovery() const
{
	return md5_sync_helper_->IsWaitingRecovery();
}

void CommandHandler::StartRecovery(const CmdGroupWithExecutors& cmd_groups)
{
	DBG_WRAPPER(LOG_NWAR(L"MD5 Sync -- Starting recovery..."));

	if (game_board_wrapper_) { game_board_wrapper_->OnStartingRecovery(); }

	this->BeforeRecovery();

	for (const auto& cmd_group : cmd_groups)
	{
		cmd_group.cmd_group_->Foreach([this](const CommandSharedPtr& cmd_ptr){
			if (CommandChecker::CommandNeedsRecovery(cmd_ptr->Type()))
			{
				this->HandleMasterCommandOnGameBoard(cmd_ptr, false, false, true);
			}
		});
	}

	for (const auto& cmd_group : md5_sync_helper_->cmd_groups_blocked_)
	{
		cmd_group->Foreach([this](const CommandSharedPtr& cmd_ptr){
			if (CommandChecker::CommandNeedsRecovery(cmd_ptr->Type()))
			{
				this->HandleMasterCommandOnGameBoard(cmd_ptr, false, false, true);
			}
		});
	}

	this->AfterRecovery();

	for (const auto& pair : *board_pairs_)
	{
		this->RefreshGameBoardWithMaster(pair.board_type_, false);
	}

	if (game_board_wrapper_) { game_board_wrapper_->OnRecoveryDone(); }

	DBG_WRAPPER(LOG_NWAR(L"MD5 Sync -- Recovery done"));
}

void CommandHandler::BeforeRecovery()
{
	md5_sync_helper_->ResetMD5();

	master_chess_cmd_being_handled_->Reset();
	master_chess_cmd_waiting_reenter_.reset();
	master_chess_cmd_blocked_reenter_->clear();
	local_chess_cmd_cache_->clear();
	current_master_board_type_ = GameBoardType::None;
	async_download_flag_.Cancel();
}

void CommandHandler::AfterRecovery()
{
	md5_sync_helper_->Cancel();
}

void CommandHandler::RefreshGameBoardWithMaster(GameBoardType board_type, bool switch_to_game_board)
{
	if (switch_to_game_board)
	{
		this->SwitchMasterToGameBoard(board_type);
		if (game_board_wrapper_)
		{
			game_board_wrapper_->SwitchMasterToGameBoard(board_type);
		}
	}

	if (game_board_wrapper_)
	{
		game_board_wrapper_->RefreshGameBoardWithMaster(board_type);
	}
}

GAME_NAMESPACE_END