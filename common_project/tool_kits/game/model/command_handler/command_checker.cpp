#include "command_checker.h"
#include <vector>
#include "game/public/unsigned_includer/unsigned_includer.h"

using namespace proto;

GAME_NAMESPACE_BEGIN

typedef UnsignedIncluder<proto::CommandType, static_cast<unsigned int>(proto::CommandType::MaxCountFlag)> CommandIncluder;

bool CommandChecker::CommandIsRelatedToChess(const CommandType cmd_type)
{
	static CommandIncluder includer(
		std::vector<CommandType>{
			CommandType::TeacherStartCourse,
			CommandType::TeacherChangeGameBoard,
			CommandType::TeacherMoveChess,
			CommandType::StudentMoveChess,
			CommandType::TeacherNewGameBoard,
			CommandType::TeacherNewGameBoardEx,
			CommandType::TeacherMarkManualChess,
			CommandType::ClearGameBoard,
			CommandType::TeacherBackForward,
			CommandType::TeacherImportChessManual,
			CommandType::TeacherSwitchToHistoryBoard,
			CommandType::TeacherUpdateAutoTakingPieces,
			CommandType::TeacherStartPauseStuChess, }
	);

	return includer.Includes(cmd_type);
}

bool CommandChecker::CommandIsRelatedToHeartbeat(const proto::CommandType cmd_type)
{
	return cmd_type == CommandType::CheckHeartbeatFromPeer;
}

bool CommandChecker::CommandIsRelatedToMD5Sync(const proto::CommandType cmd_type)
{
	static CommandIncluder includer(
		std::vector<CommandType>{
			CommandType::TeacherStartSyncCurrentChessBoard,
			CommandType::TeacherSyncCurrentChessBoard,
			CommandType::ReplayAsk,
			CommandType::ReplayReceive, }
	);

	return includer.Includes(cmd_type);
}

bool CommandChecker::CommandInfluencesMD5(const proto::CommandType cmd_type) // subset of CommandIsRelatedToChess
{
	static CommandIncluder includer(
		std::vector<CommandType>{
			CommandType::TeacherChangeGameBoard,
			CommandType::TeacherMoveChess,
			CommandType::StudentMoveChess,
			CommandType::TeacherNewGameBoard,
			CommandType::TeacherNewGameBoardEx,
			CommandType::TeacherMarkManualChess,
			CommandType::ClearGameBoard,
			CommandType::TeacherBackForward,
			CommandType::TeacherImportChessManual,
			CommandType::TeacherSwitchToHistoryBoard,
			CommandType::TeacherUpdateAutoTakingPieces, }
	);

	return includer.Includes(cmd_type);
}

bool CommandChecker::CommandNeedsToBeHandledInGameBoard(const CommandType cmd_type) // subset of CommandIsRelatedToChess
{
	static CommandIncluder includer(
		std::vector<CommandType>{
			CommandType::TeacherMoveChess,
			CommandType::StudentMoveChess,
			CommandType::TeacherNewGameBoard,
			CommandType::TeacherNewGameBoardEx,
			CommandType::TeacherMarkManualChess,
			CommandType::ClearGameBoard,
			CommandType::TeacherBackForward,
			CommandType::TeacherImportChessManual,
			CommandType::TeacherSwitchToHistoryBoard,
			CommandType::TeacherUpdateAutoTakingPieces, }
	);

	return includer.Includes(cmd_type);
}

bool CommandChecker::CommandInfluencesDrawOrMarkBoard(const CommandType cmd_type)
{
	switch (cmd_type)
	{
	case CommandType::TeacherChangeGameBoard:
	case CommandType::TeacherChangeBoardPager:
	case CommandType::TeacherChangePhotoPage:
	case CommandType::TeacherChangeToPpt:

	case CommandType::StudentChangeGameBoard:
	case CommandType::StudentChangeBoardPager:
	case CommandType::StudentChangePhotoPager:
	case CommandType::StudentChangePptPager:

	case CommandType::TeacherDrawGroup:  // too big to use CommandIncluder
	case CommandType::TeacherDrawStart:
	case CommandType::TeacherDrawMove:
	case CommandType::TeacherDrawEnd:

	case CommandType::TeacherEraserGroup:  // too big to use CommandIncluder
	case CommandType::TeacherEraserDrawDown:
	case CommandType::TeacherEraserDrawMove:
	case CommandType::TeacherEraserDrawUp:

	case CommandType::StudentDrawGroup:  // too big to use CommandIncluder
	case CommandType::StudentDrawStart:
	case CommandType::StudentDrawMove:
	case CommandType::StudentDrawEnd:

	case CommandType::TeacherUpdateMarkOnBoard:
		return true;

	default:
		break;
	}

	return false;
}

bool CommandChecker::CommandNeedsRecovery(const proto::CommandType cmd_type)
{
	return !CommandIsRelatedToHeartbeat(cmd_type) && !CommandIsRelatedToMD5Sync(cmd_type);
}

GAME_NAMESPACE_END
