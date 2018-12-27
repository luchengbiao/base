#include "command_factory_default.h"
#include "protocol/command/base/command_class_runtime_info.h"
#include "base/command_cell.h"
#include "base/command_group.h"
#include "common/macros/assert.h"
#include "commands/cmd_teacher_draw_start.h"
#include "commands/cmd_teacher_draw_move.h"
#include "commands/cmd_teacher_draw_end.h"
#include "commands/cmd_teacher_change_color.h"
#include "commands/cmd_teacher_start_course.h"
#include "commands/cmd_end_course.h"
#include "commands/cmd_teacher_move_chess.h"
#include "commands/cmd_upload_img.h"
#include "commands/cmd_teacher_change_price.h"
#include "commands/cmd_teacher_change_game_board.h"
#include "commands/cmd_teacher_check_manual_chess.h"
#include "commands/cmd_student_charge_balance.h"
#include "commands/cmd_teacher_change_photo_page.h"
#include "commands/cmd_student_recv_course_start.h"
#include "commands/cmd_exchange_data.h"
#include "commands/cmd_student_revoke.h"
#include "commands/cmd_teacher_revoke.h"
#include "commands/cmd_student_clean.h"
#include "commands/cmd_teacher_clean.h"
#include "commands/cmd_audio_start.h"
#include "commands/cmd_times_sign.h"
#include "commands/cmd_teacher_upload_ppt_files.h"
#include "commands/cmd_student_change_board_pager.h"
#include "commands/cmd_student_change_photo_pager.h"
#include "commands/cmd_student_change_ppt_pager.h"
#include "commands/cmd_student_draw_start.h"
#include "commands/cmd_student_draw_move.h"
#include "commands/cmd_student_draw_end.h"
#include "commands/cmd_student_audio_start.h"
#include "commands/cmd_teacher_reset_ppt.h"
#include "commands/cmd_start_observe.h"
#include "commands/cmd_observe_data.h"
#include "commands/cmd_exchange_user_info.h"
#include "commands/cmd_teacher_class_upload_ppt_files.h"
#include "commands/cmd_teacher_class_change_ppt_pager.h"
#include "commands/cmd_teacher_class_clean_ppt.h"
#include "commands/cmd_teacher_class_begin_upload.h"
#include "commands/cmd_teacher_class_end_upload.h"
#include "commands/cmd_teacher_forbidden_student_oper.h"
#include "commands/cmd_teacher_show_board_rule.h"
#include "commands/cmd_teacher_eraser_draw_down.h"
#include "commands/cmd_teacher_eraser_draw_move.h"
#include "commands/cmd_teacher_eraser_draw_up.h"
#include "commands/cmd_student_change_game_board.h"
#include "commands/cmd_student_move_chess.h"
#include "commands/cmd_teacher_new_game_board.h"
#include "commands/cmd_teacher_mark_manual_chess.h"
#include "commands/cmd_teacher_change_board_pager.h"
#include "commands/cmd_teacher_change_to_ppt.h"
#include "commands/cmd_clear_game_board.h"
#include "commands/cmd_replay_ask.h"
#include "commands/cmd_replay_receive.h"
#include "commands/cmd_check_heartbeat_from_peer.h"
#include "commands/cmd_teacher_start_sync_current_chess_board.h"
#include "commands/cmd_teacher_sync_current_chess_board.h"
#include "commands/cmd_student_hangsup_speak.h"
#include "commands/cmd_teacher_allow_forbid_stu_speak.h"
#include "commands/cmd_teacher_add_cancel_stu_chess.h"
#include "commands/cmd_teacher_start_pause_stu_chess.h"
#include "commands/cmd_teacher_exchange_stu_chess.h"
#include "commands/cmd_teacher_new_game_board_ex.h"
#include "commands/cmd_teacher_back_forward.h"
#include "commands/cmd_teacher_import_chess_manual.h"
#include "commands/cmd_teacher_update_mark_on_board.h"
#include "commands/cmd_teacher_update_auto_taking_pieces.h"
#include "commands/cmd_teacher_switch_to_history_board.h"
#include "commands/cmd_group_student_draw.h"
#include "commands/cmd_group_teacher_draw.h"
#include "commands/cmd_group_teacher_eraser.h"
#include "commands/cmd_group_with_timestamp.h"

static void RegisterCommandsClassRuntimeInfo()
{
	REGISTER_COMMAND_RUNTIME_INFO(proto::CmdTeacherDrawStart);
	REGISTER_COMMAND_RUNTIME_INFO(proto::CmdTeacherDrawMove);
	REGISTER_COMMAND_RUNTIME_INFO(proto::CmdTeacherDrawEnd);
	REGISTER_COMMAND_RUNTIME_INFO(proto::CmdTeacherChangeColor);
	REGISTER_COMMAND_RUNTIME_INFO(proto::CmdTeacherStartCourse);
	REGISTER_COMMAND_RUNTIME_INFO(proto::CmdEndCourse);
	REGISTER_COMMAND_RUNTIME_INFO(proto::CmdTeacherMoveChess);
	REGISTER_COMMAND_RUNTIME_INFO(proto::CmdUploadImg);
	REGISTER_COMMAND_RUNTIME_INFO(proto::CmdTeacherChangePrice);
	REGISTER_COMMAND_RUNTIME_INFO(proto::CmdTeacherChangeGameBoard);
	REGISTER_COMMAND_RUNTIME_INFO(proto::CmdTeacherCheckManualChess);
	REGISTER_COMMAND_RUNTIME_INFO(proto::CmdStudentChargeBalance);
	REGISTER_COMMAND_RUNTIME_INFO(proto::CmdTeacherChangePhotoPage);
	REGISTER_COMMAND_RUNTIME_INFO(proto::CmdStudentRecvCourseStart);
	REGISTER_COMMAND_RUNTIME_INFO(proto::CmdExchangeData);
	REGISTER_COMMAND_RUNTIME_INFO(proto::CmdStudentRevoke);
	REGISTER_COMMAND_RUNTIME_INFO(proto::CmdTeacherRevoke);
	REGISTER_COMMAND_RUNTIME_INFO(proto::CmdStudentClean);
	REGISTER_COMMAND_RUNTIME_INFO(proto::CmdTeacherClean);
	REGISTER_COMMAND_RUNTIME_INFO(proto::CmdAudioStart);
	REGISTER_COMMAND_RUNTIME_INFO(proto::CmdTimesSign);
	REGISTER_COMMAND_RUNTIME_INFO(proto::CmdTeacherUploadPptFiles);
	REGISTER_COMMAND_RUNTIME_INFO(proto::CmdStudentChangeBoardPager);
	REGISTER_COMMAND_RUNTIME_INFO(proto::CmdStudentChangePhotoPager);
	REGISTER_COMMAND_RUNTIME_INFO(proto::CmdStudentChangePptPager);
	REGISTER_COMMAND_RUNTIME_INFO(proto::CmdStudentDrawStart);
	REGISTER_COMMAND_RUNTIME_INFO(proto::CmdStudentDrawMove);
	REGISTER_COMMAND_RUNTIME_INFO(proto::CmdStudentDrawEnd);
	REGISTER_COMMAND_RUNTIME_INFO(proto::CmdStudentAudioStart);
	REGISTER_COMMAND_RUNTIME_INFO(proto::CmdTeacherResetPpt);
	REGISTER_COMMAND_RUNTIME_INFO(proto::CmdStartObserve);
	REGISTER_COMMAND_RUNTIME_INFO(proto::CmdObserveData);
	REGISTER_COMMAND_RUNTIME_INFO(proto::CmdExchangeUserInfo);
	REGISTER_COMMAND_RUNTIME_INFO(proto::CmdTeacherClassUploadPptFiles);
	REGISTER_COMMAND_RUNTIME_INFO(proto::CmdTeacherClassChangePptPager);
	REGISTER_COMMAND_RUNTIME_INFO(proto::CmdTeacherClassCleanPpt);
	REGISTER_COMMAND_RUNTIME_INFO(proto::CmdTeacherClassBeginUpload);
	REGISTER_COMMAND_RUNTIME_INFO(proto::CmdTeacherClassEndUpload);
	REGISTER_COMMAND_RUNTIME_INFO(proto::CmdTeacherForbiddenStudentOper);
	REGISTER_COMMAND_RUNTIME_INFO(proto::CmdTeacherShowBoardRule);
	REGISTER_COMMAND_RUNTIME_INFO(proto::CmdTeacherEraserDrawDown);
	REGISTER_COMMAND_RUNTIME_INFO(proto::CmdTeacherEraserDrawMove);
	REGISTER_COMMAND_RUNTIME_INFO(proto::CmdTeacherEraserDrawUp);
	REGISTER_COMMAND_RUNTIME_INFO(proto::CmdStudentChangeGameBoard);
	REGISTER_COMMAND_RUNTIME_INFO(proto::CmdStudentMoveChess);
	REGISTER_COMMAND_RUNTIME_INFO(proto::CmdTeacherNewGameBoard);
	REGISTER_COMMAND_RUNTIME_INFO(proto::CmdTeacherMarkManualChess);
	REGISTER_COMMAND_RUNTIME_INFO(proto::CmdTeacherChangeBoardPager);
	REGISTER_COMMAND_RUNTIME_INFO(proto::CmdTeacherChangeToPpt);
	REGISTER_COMMAND_RUNTIME_INFO(proto::CmdClearGameBoard);
	REGISTER_COMMAND_RUNTIME_INFO(proto::CmdReplayAsk);
	REGISTER_COMMAND_RUNTIME_INFO(proto::CmdReplayReceive);
	REGISTER_COMMAND_RUNTIME_INFO(proto::CmdCheckHeartbeatFromPeer);
	REGISTER_COMMAND_RUNTIME_INFO(proto::CmdTeacherStartSyncCurrentChessBoard);
	REGISTER_COMMAND_RUNTIME_INFO(proto::CmdTeacherSyncCurrentChessBoard);
	REGISTER_COMMAND_RUNTIME_INFO(proto::CmdStudentHangsupSpeak);
	REGISTER_COMMAND_RUNTIME_INFO(proto::CmdTeacherAllowForbidStuSpeak);
	REGISTER_COMMAND_RUNTIME_INFO(proto::CmdTeacherAddCancelStuChess);
	REGISTER_COMMAND_RUNTIME_INFO(proto::CmdTeacherStartPauseStuChess);
	REGISTER_COMMAND_RUNTIME_INFO(proto::CmdTeacherExchangeStuChess);
	REGISTER_COMMAND_RUNTIME_INFO(proto::CmdTeacherNewGameBoardEx);
	REGISTER_COMMAND_RUNTIME_INFO(proto::CmdTeacherBackForward);
	REGISTER_COMMAND_RUNTIME_INFO(proto::CmdTeacherImportChessManual);
	REGISTER_COMMAND_RUNTIME_INFO(proto::CmdTeacherUpdateMarkOnBoard);
	REGISTER_COMMAND_RUNTIME_INFO(proto::CmdTeacherUpdateAutoTakingPieces);
	REGISTER_COMMAND_RUNTIME_INFO(proto::CmdTeacherSwitchToHistoryBoard);

	REGISTER_COMMAND_RUNTIME_INFO(proto::CommandCell);
	REGISTER_COMMAND_RUNTIME_INFO(proto::CommandGroup);
	REGISTER_COMMAND_RUNTIME_INFO(proto::CmdGroupWithTimestamp);
	REGISTER_COMMAND_RUNTIME_INFO(proto::CmdGroupStudentDraw);
	REGISTER_COMMAND_RUNTIME_INFO(proto::CmdGroupTeacherDraw);
	REGISTER_COMMAND_RUNTIME_INFO(proto::CmdGroupTeacherEraser);
}

static CommandClassRuntimeInfo*& FristCommandClassRuntimeInfoPointer()
{
	static CommandClassRuntimeInfo* fristCommandClassRuntimeInfoPointer = nullptr;

	return fristCommandClassRuntimeInfoPointer;
}

CommandClassRuntimeInfo::ClassRuntimeInfoWithType(proto::CommandType type, const std::string& className, PFuncCommandCreator creator)
	: _type(type), _className(className), _creator(creator), _next(FristCommandClassRuntimeInfoPointer())
{
	CommandClassRuntimeInfo*& firstPtr = FristCommandClassRuntimeInfoPointer();

	D_ASSERT(!(firstPtr && firstPtr->Type() == this->Type()), L"The type has already been registered.");
	
	firstPtr = this;
}

PROTO_NAMESPACE_BEGIN

DefaultCommandFactory::DefaultCommandFactory()
{
	RegisterCommandsClassRuntimeInfo();
}

CommandUniquePtr DefaultCommandFactory::CreateCommandWithType(CommandType type) const
{
	CommandUniquePtr uniquePtr;

	CommandClassRuntimeInfo* ptr = FristCommandClassRuntimeInfoPointer();

	while (ptr)
	{
		if (ptr->Type() == type)
		{
			uniquePtr = ptr->Create();
			break;
		}

		ptr = ptr->Next();
	}

	return uniquePtr;
}

PROTO_NAMESPACE_END