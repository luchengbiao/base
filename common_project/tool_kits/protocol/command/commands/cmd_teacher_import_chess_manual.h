#ifndef __PROTO_COMMANDS_CMD_TEACHER_IMPORT_CHESS_MANUAL_H__
#define __PROTO_COMMANDS_CMD_TEACHER_IMPORT_CHESS_MANUAL_H__
#include "../base/command_cell.h"

PROTO_NAMESPACE_BEGIN

class CommandGroup;
typedef std::shared_ptr<CommandGroup> CommandGroupPtr;

struct ImportChessManual;
typedef std::unique_ptr<ImportChessManual> ImportChessManualPtr;

class CommandFactory;

class CmdTeacherImportChessManual : public CommandCell
{
	DECLARE_COMMAND_RUNTIME_INFO(CmdTeacherImportChessManual)

	class MyObjectCounter : public nbase::ObjectCounter<MyObjectCounter> {};

public:
	~CmdTeacherImportChessManual();

	void						SetChessManualFilePath(const std::wstring& file_path);

	CommandGroupPtr				GetOrCreateCommandGroup(const CommandFactory& cmd_factory);
	CommandGroupPtr				GetCommandGroup() const;
	CommandSharedPtr			FindNewBoardCommand() const; // NOTICE: Please call GetOrCreateCommandGroup at least once after SetChessManualFilePath to trigger generating CommandGroup before FindNewBoardCommand

	const ImportChessManual*	EntityOfCommand();

private:
	std::wstring				local_file_path_;
	CommandGroupPtr				cmd_group_;
	ImportChessManualPtr		import_chess_manaul_;

	MyObjectCounter				counter_;
};

PROTO_NAMESPACE_END

#endif