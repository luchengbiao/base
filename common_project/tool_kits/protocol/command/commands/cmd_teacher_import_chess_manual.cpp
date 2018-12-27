#include "cmd_teacher_import_chess_manual.h"
#include <map>
#include "cmd_group_with_timestamp.h"
#include "common/macros/json_def.h"
#include <mutex>
#include "json/json.h"
#include "zip/user_zip.h"
#include "protocol/command/entities/import_chess_manual.h"

PROTO_NAMESPACE_BEGIN

IMPLEMENT_COMMAND_RUNTIME_INFO(CommandType::TeacherImportChessManual, CmdTeacherImportChessManual, CommandCell)

static std::mutex s_mutex;
static std::unique_ptr<std::map<std::string, CommandGroupPtr>> s_cache{ nullptr }; //<file_url, CommandGroupPtr>

static void CacheCmdGroup(const std::string& file_url, const CommandGroupPtr& cmd_group)
{
	if (cmd_group)
	{
		std::lock_guard<std::mutex> lock(s_mutex);

		if (!s_cache)
		{
			s_cache.reset(new std::map<std::string, CommandGroupPtr>);
		}

		if (s_cache)
		{
			s_cache->emplace(file_url, cmd_group);
		}
	}
}

static CommandGroupPtr CmdGroupByUrl(const std::string& file_url)
{
	CommandGroupPtr cmd_group;

	{
		std::lock_guard<std::mutex> lock(s_mutex);

		if (s_cache)
		{
			auto it = s_cache->find(file_url);
			if (s_cache->cend() != it)
			{
				cmd_group = it->second;
			}
		}
	}


	return cmd_group;
}

static void DeleteCmdGroups()
{
	std::lock_guard<std::mutex> lock(s_mutex);

	s_cache.reset();
}

CmdTeacherImportChessManual::~CmdTeacherImportChessManual()
{
	if (MyObjectCounter::live() == 1)
	{
		DeleteCmdGroups();
	}
}

void CmdTeacherImportChessManual::SetChessManualFilePath(const std::wstring& file_path)
{
	if (local_file_path_ != file_path)
	{
		local_file_path_ = file_path;
	}
}

CommandGroupPtr CmdTeacherImportChessManual::GetOrCreateCommandGroup(const CommandFactory& cmd_factory)
{
	if (cmd_group_) { return cmd_group_; }

	do
	{
		auto file_url = StringAt(0);
		cmd_group_ = CmdGroupByUrl(file_url);

		if (cmd_group_) { break; }

		if (local_file_path_.empty()) { break; }

		std::string str = zip::UnCompressGz(local_file_path_);
		if (str.empty()) { break; }

		cmd_group_ = CmdGroupWithTimestamp::Create();

		int totalLen = str.length();
		for (int i = 0; i < totalLen;)
		{
			auto cmdGroupPtr = CmdGroupWithTimestamp::Create();

			int cmdLen = cmdGroupPtr->UnpackFromString(str, i);

			if (cmdLen > 0)
			{
				cmdGroupPtr->UnpackGroup(true, cmd_factory);
			}
			else
			{
				break;
			}

			cmdGroupPtr->Foreach([=](const CommandSharedPtr& cmd_ptr){
				cmd_group_->Add(cmd_ptr);
			});

			i += cmdLen;
		}

		CacheCmdGroup(file_url, cmd_group_);

	} while (false);

	return cmd_group_;
}

CommandGroupPtr CmdTeacherImportChessManual::GetCommandGroup() const
{
	return cmd_group_;
}

CommandSharedPtr CmdTeacherImportChessManual::FindNewBoardCommand() const
{
	CommandSharedPtr cmd_ptr;

	if (cmd_group_)
	{
		cmd_group_->ForeachWithBreaker([&cmd_ptr](const CommandSharedPtr& cmd_tmp){
			const auto cmd_type = cmd_tmp->Type();

			if (cmd_type == CommandType::TeacherNewGameBoardEx || cmd_type == CommandType::TeacherNewGameBoard)
			{
				cmd_ptr = cmd_tmp;
				return true;
			}

			return false;
		});
	}

	return cmd_ptr;
}

const ImportChessManual* CmdTeacherImportChessManual::EntityOfCommand()
{
	if (!import_chess_manaul_)
	{
		import_chess_manaul_.reset(new ImportChessManual);

		import_chess_manaul_->file_url_ = this->StringAt(0);
		import_chess_manaul_->game_mode_ = this->IntAt(1);

		auto ext_str = this->StringAt(2);
		Json::Value value;
		Json::Reader reader;
		if (reader.parse(ext_str, value))
		{
			JSON_FIELD_AS_INT(import_chess_manaul_->role_, role, value);
			JSON_FIELD_AS_STRING(import_chess_manaul_->white_name_, whiteName, value);
			JSON_FIELD_AS_STRING(import_chess_manaul_->white_url_, whiteUrl, value);
			JSON_FIELD_AS_STRING(import_chess_manaul_->black_name_, blackName, value);
			JSON_FIELD_AS_STRING(import_chess_manaul_->black_url_, blackUrl, value);
		}
	}

	return import_chess_manaul_.get();
}

PROTO_NAMESPACE_END