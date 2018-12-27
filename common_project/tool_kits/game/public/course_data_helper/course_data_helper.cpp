#include "course_data_helper.h"
#include <algorithm>
#include <iterator>
#include <thread>
#include "base/file/file_util.h"
#include "base/util/string_util.h"
#include "game/public/downloaded_files_recorder/downloaded_files_recorder.h"
#include "game/public/public_setting/public_setting.h"
#include "log/log.h"
#include "protocol/command/base/command_factory.h"
#include "protocol/command/commands/cmd_teacher_import_chess_manual.h"
#include "protocol/command/commands/cmd_teacher_new_game_board_ex.h"
#include "protocol/command/commands/cmd_upload_img.h"
#include "protocol/command/entities/import_chess_manual.h"
#include "protocol/command/entities/new_game_board_ex.h"
#include "qthread_manager/closure.h"
#include "zip/user_zip.h"

using namespace proto;

GAME_NAMESPACE_BEGIN

CourseDataHelper::CourseDataHelper()
{
	qRegisterMetaType<StdClosure>("StdClosure");
	connect(this, SIGNAL(SignalClosure(StdClosure)), this, SLOT(SlotClosure(StdClosure)));
}

CmdExecutor CourseDataHelper::ExecutorOfFile(const std::wstring& file_path) const
{
	std::wstring file_name = file_path;
	if (file_path.find(LR"(\)") != std::wstring::npos || file_path.find(LR"(/)") != std::wstring::npos)
	{
		nbase::FilePathApartFileName(file_path, file_name);
	}

	return file_name.find(L"tea") != std::wstring::npos ? CmdExecutor::Teacher : CmdExecutor::Student;
}

CmdGroupWithExecutors CourseDataHelper::ParseFromFiles(const std::unordered_map<std::wstring, CmdExecutor>& mapFileToExecutor,
													   const proto::CommandFactory& cmd_factory,
													   bool need_to_sort) const
{
	CmdGroupWithExecutors cmd_groups;

	for (auto& pair : mapFileToExecutor)
	{
		std::string content = zip::UnCompressGz(pair.first);

		CmdGroupWithExecutors cmd_groups_tmp = ParseFromString(content, pair.second, cmd_factory, false);
		std::copy(cmd_groups_tmp.cbegin(), cmd_groups_tmp.cend(), std::back_inserter(cmd_groups));
	}

	if (!cmd_groups.empty() && need_to_sort)
	{
		SortCmdGroups(cmd_groups);
	}

	return cmd_groups;
}

CmdGroupWithExecutors CourseDataHelper::ParseFromString(const std::string& str,
														const CmdExecutor executor,
														const proto::CommandFactory& cmd_factory,
														bool need_to_sort) const
{
	CmdGroupWithExecutors cmd_groups;

	int totalLen = str.length();
	for (int i = 0; i < totalLen;)
	{
		CmdGroupWithTimestampSharedPtr cmdGroupPtr(proto::CmdGroupWithTimestamp::Create());

		int cmdLen = cmdGroupPtr->UnpackFromString(str, i);

		if (cmdLen > 0)
		{
			cmd_groups.emplace_back(cmdGroupPtr, executor);

			cmdGroupPtr->UnpackGroup(CourseDataIsBase64Encoded(), cmd_factory);
		}
		else
		{
			LOG_NERR("Error ocurrs in CourseDataHelper::ParseFromString");

			break;
		}

		i += cmdLen;
	}

	if (!cmd_groups.empty() && need_to_sort)
	{
		SortCmdGroups(cmd_groups);
	}

	return cmd_groups;
}

void CourseDataHelper::SortCmdGroups(CmdGroupWithExecutors& cmd_groups) const
{
	std::stable_sort(cmd_groups.begin(), cmd_groups.end(),
		[](const CmdGroupWithExecutor& lhs, const CmdGroupWithExecutor& rhs)
	{
		if (lhs.cmd_group_->Timestamp() < rhs.cmd_group_->Timestamp())
		{ 
			return true; 
		}
		else if (lhs.cmd_group_->Timestamp() == rhs.cmd_group_->Timestamp())
		{
			if (lhs.executor_ != rhs.executor_)
			{
				return lhs.executor_ == CmdExecutor::Teacher;
			}
		}
		return false; 
	} );
}

void CourseDataHelper::DownloadResourcesReferedByCmdGroups(const CmdGroups& cmd_groups, const StdClosure& callback)
{
	typedef std::vector<std::shared_ptr<CmdTeacherImportChessManual>> ImportManualCmdPtrs;
	typedef std::vector<std::shared_ptr<CmdUploadImg>> UploadImgCmdPtrs;

	std::unordered_map<std::string, ImportManualCmdPtrs> url_2_import_manual_cmds; // import manual commands
	std::unordered_map<std::string, UploadImgCmdPtrs> url_2_upload_img_cmds; // upload image commands
	std::unordered_map<std::string, std::wstring> file_url_2_dir;

	for (auto& cmd_group : cmd_groups)
	{
		cmd_group->Foreach([&](const CommandSharedPtr& cmd_ptr) {
			const auto cmd_type = cmd_ptr->Type();
			if (cmd_type == CommandType::TeacherImportChessManual)
			{
				auto import_manual_cmd = std::static_pointer_cast<CmdTeacherImportChessManual>(cmd_ptr);
				const auto entity = import_manual_cmd->EntityOfCommand();

				std::wstring file_path;
				if (ExistingFileOfUrl(entity->file_url_, &file_path))
				{
					import_manual_cmd->SetChessManualFilePath(file_path);
				}
				else
				{
					file_url_2_dir.emplace(entity->file_url_, DirctoryToStoreChessManual());
					url_2_import_manual_cmds[entity->file_url_].emplace_back(import_manual_cmd);
				}

				if (!entity->white_url_.empty())
				{
					auto file_url = PlayerPortraitUrl(entity->white_url_);
					if (!ExistingFileOfUrl(file_url))
					{
						file_url_2_dir.emplace(std::move(file_url), DirctoryToStorePlayerPortrait());
					}
				}

				if (!entity->black_url_.empty())
				{
					auto file_url = PlayerPortraitUrl(entity->black_url_);
					if (!ExistingFileOfUrl(file_url))
					{
						file_url_2_dir.emplace(std::move(file_url), DirctoryToStorePlayerPortrait());
					}
				}
			}
			else if (cmd_type == CommandType::TeacherNewGameBoardEx)
			{
				auto new_board_ex_cmd = std::static_pointer_cast<CmdTeacherNewGameBoardEx>(cmd_ptr);
				const auto entity = new_board_ex_cmd->EntityOfCommand();

				if (!entity->white_url_.empty())
				{
					auto file_url = PlayerPortraitUrl(entity->white_url_);
					if (!ExistingFileOfUrl(file_url))
					{
						file_url_2_dir.emplace(std::move(file_url), DirctoryToStorePlayerPortrait());
					}
				}

				if (!entity->black_url_.empty())
				{
					auto file_url = PlayerPortraitUrl(entity->black_url_);
					if (!ExistingFileOfUrl(file_url))
					{
						file_url_2_dir.emplace(std::move(file_url), DirctoryToStorePlayerPortrait());
					}
				}
			}
			else if (cmd_type == CommandType::TeacherAddCancelStuChess)
			{
				auto raw_url = cmd_ptr->StringAt(4);
				if (!raw_url.empty())
				{
					auto file_url = PlayerPortraitUrl(raw_url);
					if (!ExistingFileOfUrl(file_url))
					{
						file_url_2_dir.emplace(std::move(file_url), DirctoryToStorePlayerPortrait());
					}
				}
			}
			else if (cmd_type == CommandType::UploadImg)
			{
				auto upload_img_cmd = std::static_pointer_cast<CmdUploadImg>(cmd_ptr);
				auto file_url = upload_img_cmd->StringAt(0);

				std::wstring file_path;
				if (ExistingFileOfUrl(file_url, &file_path))
				{
					upload_img_cmd->SetImageFilePath(file_path);
				}
				else
				{
					file_url_2_dir.emplace(file_url, DirctoryToStoreImage());
					url_2_upload_img_cmds[file_url].emplace_back(upload_img_cmd);
				}

			}
		});
	}

	if (file_url_2_dir.empty())
	{
		if (callback) { callback(); }
	}
	else
	{
		auto files_fetcher = this->CreateFetcher();

		auto cb_downloaded = this->ToWeakCallback([=](){

			emit SignalClosure([=]{
				auto url_2_file = files_fetcher->GetUrlToFilePathMap();
				for (const auto& pair : url_2_file)
				{
					DOWNLOADED_FILES_RECORDER->Record(pair.first, pair.second);

					auto it_manual = url_2_import_manual_cmds.find(pair.first);
					if (it_manual != url_2_import_manual_cmds.cend())
					{
						for (const auto& import_manual_cmd : it_manual->second)
						{
							import_manual_cmd->SetChessManualFilePath(pair.second);
						}
					}

					auto it_image = url_2_upload_img_cmds.find(pair.first);
					if (it_image != url_2_upload_img_cmds.cend())
					{
						for (const auto& upload_img_cmd : it_image->second)
						{
							upload_img_cmd->SetImageFilePath(pair.second);
						}
					}
				}

				this->DeleteFetcher(files_fetcher);

				if (callback) { callback(); }
			});

		});

		auto progress = this->ToWeakCallback([=](double loaded, double total){
		});

		files_fetcher->Fetch(file_url_2_dir, cb_downloaded, progress);
	}
}

void CourseDataHelper::SlotClosure(StdClosure closure)
{
	if (closure) { closure(); }
}

FilesFetcherSharedPtr CourseDataHelper::CreateFetcher()
{
	std::lock_guard<std::recursive_mutex> lock(mutex_);

	auto files_fetcher = FilesFetcher::Create();

	files_fetchers_.emplace(files_fetcher);

	return files_fetcher;
}

void CourseDataHelper::DeleteFetcher(const FilesFetcherSharedPtr& fetcher)
{
	std::lock_guard<std::recursive_mutex> lock(mutex_);

	files_fetchers_.erase(fetcher);
}

GAME_NAMESPACE_END