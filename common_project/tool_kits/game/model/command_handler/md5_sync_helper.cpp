#include "md5_sync_helper.h"
#include <QThread>
#include "base/util/string_util.h"
#include "common/system/system_tool.h"
#include "crypto/include/md5.h"
#include "crypto/include/cryptlib.h"
#include "crypto/include/config.h"
#include "game/public/course_data_helper/course_data_helper.h"
#include "game/public/public_setting/public_setting.h"
#include "http_request/file_fetch.h"
#include "log/log.h"
#include "protocol/command/base/command_group.h"
#include "protocol/command/commands/cmd_replay_ask.h"
#include "protocol/command/commands/cmd_teacher_start_sync_current_chess_board.h"
#include "protocol/command/commands/cmd_teacher_sync_current_chess_board.h"
#include "protocol/command/command_factory_default.h"
#include "qthread_manager/closure.h"
#include "game_board_wrapper.h"
#include "command_checker.h"
#include "command_handler.h"

#ifdef _DEBUG
#pragma comment(lib, "cryptlib_d.lib")
#else
#pragma comment(lib, "cryptlib.lib")
#endif

using namespace proto;

#define  TRIGGER_CONFLICT_FOR_TESTING_RECOVERY 0

GAME_NAMESPACE_BEGIN

std::shared_ptr<MD5SyncHelper> MD5SyncHelper::Create(const CommandHandlerWeakPtr& command_handler)
{
	std::shared_ptr<MD5SyncHelper> ptr(new MD5SyncHelper(command_handler));

	if (ptr)
	{
		ptr->recovery_flag_.SetWeakPtr(ptr->GetWeakPtr());
	}

	return (ptr);
}

MD5SyncHelper::MD5SyncHelper(const CommandHandlerWeakPtr& command_handler)
: command_handler_(command_handler)
{
	qRegisterMetaType<StdClosure>("StdClosure");
	connect(this, SIGNAL(SignalClosure(StdClosure)), this, SLOT(SlotClosure(StdClosure)));
}

bool MD5SyncHelper::TryToHandleMasterSyncCommand(const CommandSharedPtr& cmd_ptr)
{
	bool ret = true;

	const auto cmd_type = cmd_ptr->Type();
	if (cmd_type == CommandType::TeacherStartSyncCurrentChessBoard)
	{
		if (!UsageTypeIsPlayback())
		{
			if (ClientTypeIsTeacher())
			{
				auto handler = command_handler_.lock();
				if (handler) 
				{ 
					handler->SendCommand(CmdTeacherSyncCurrentChessBoard::CreateWithVariants(md5_temp_to_sync_));
				}
			}
			else
			{
				md5_temp_to_sync_ = md5_of_all_master_chess_cmd_;
			}
		}
	}
	else if (cmd_type == CommandType::TeacherSyncCurrentChessBoard)
	{
		if (!UsageTypeIsPlayback() && ClientTypeIsStudent())
		{
			const auto md5_from_tea = cmd_ptr->StringAt(0);

			LOG_O2O_OR_O2M_WITH_ARGS(L"Tea-MD5: {0}, Stu-MD5: {1}", md5_from_tea << md5_temp_to_sync_);

#if (TRIGGER_CONFLICT_FOR_TESTING_RECOVERY > 0)
			static int TimesToTriggerConflict = 0;
			if (md5_from_tea != md5_temp_to_sync_ || (++TimesToTriggerConflict % TRIGGER_CONFLICT_FOR_TESTING_RECOVERY) == 0)
#else
			if (md5_from_tea != md5_temp_to_sync_)
#endif
			{
				DBG_WRAPPER(LOG_NWAR(L"MD5 Sync -- Conflict"));

				do
				{
					if (this->IsRequestingRecovery()) { break; }

					DBG_WRAPPER(LOG_NWAR(L"MD5 Sync -- Requesting recovery..."));

					auto handler = command_handler_.lock();
					if (!handler) { break; }

					const auto sign_key = nbase::StringPrintf("%lld", systembase::get_time_ms());
					handler->SendCommand(CmdReplayAsk::CreateWithVariants(sign_key));

					this->RequestWithToken(sign_key);

					if (handler->game_board_wrapper_) { handler->game_board_wrapper_->OnWaitingRecovery(); }

				} while (false);
			}
		}
	}
	else if (cmd_type == CommandType::ReplayAsk)
	{
		if (!UsageTypeIsPlayback() && ClientTypeIsTeacher())
		{
		}
	}
	else if (cmd_type == CommandType::ReplayReceive)
	{
		if (!UsageTypeIsPlayback() && ClientTypeIsStudent())
		{
			if (this->Token() == cmd_ptr->StringAt(0))
			{
				this->Download(cmd_ptr->StringAt(1));
			}
		}
	}
	else
	{
		ret = false;
	}

	return ret;
}

bool MD5SyncHelper::TryToAccumulateMD5OfMasterChessCommand(const CommandSharedPtr& cmd_ptr)
{
	const auto cmd_type = cmd_ptr->Type();
	if (!CommandChecker::CommandInfluencesMD5(cmd_type)) { return false; }

	std::stringstream ss;
	ss << static_cast<int>(cmd_type);

	cmd_ptr->ForeachVariant([&ss](const Variant& var){
		const auto var_type = var.GetType();
		switch (var_type)
		{
		case Variant::Type::Int:
			ss << var.ToInt();
			break;

		case Variant::Type::Float:
			ss << var.ToFloat(); // precision?
			break;

		case Variant::Type::LongLong:
			ss << var.ToLongLong();
			break;

		case Variant::Type::String:
			ss << var.ToString();
			break;

		default:
			break;
		}
	});

	std::string cmd_str = ss.str();
	std::string str = md5_of_all_master_chess_cmd_ + cmd_str;

	byte arr_byte[16];
	memset(arr_byte, 0x00, sizeof(arr_byte));

	CryptoPP::Weak1::MD5 md5;
	md5.CalculateDigest(arr_byte, (byte*)(str.c_str()), str.size());

	md5_of_all_master_chess_cmd_ = nbase::BinaryToHexString((void*)arr_byte, sizeof(arr_byte));

	LOG_O2O_OR_O2M_WITH_ARGS(L"Cmd_Type: {0}, Cmd_Str: {1}, MD5: {2}", static_cast<int>(cmd_type) << cmd_str << md5_of_all_master_chess_cmd_);

	return true;
}


void MD5SyncHelper::Download(const std::string& file_url)
{
	switch (step_)
	{
	case Step::Requesting:
		step_ = Step::Downloading;
		DBG_WRAPPER(LOG_NWAR(L"MD5 Sync -- Downloading recovery files..."));
		break;

	case Step::Downloading:
		step_ = Step::Waiting;
		break;

	default:
		break;
	}

	auto cbCompleted = recovery_flag_.ToWeakCallback([=](const std::string& file_path){
		auto file_path_w = nbase::UTF8ToUTF16(file_path);
		if (this->thread() == QThread::currentThread())
		{
			OnFileDownloaded(file_url, file_path_w);
		}
		else
		{
			emit SignalClosure([=]{ OnFileDownloaded(file_url, file_path_w); });
		}
	});

	auto cbProgress = recovery_flag_.ToWeakCallback([=](double d1, double d2, double d3, double d4)
	{
		double progress = (d4 * 100 / d3);
	});

	FileFetcher ffetcher;
	ffetcher.AsyncFetchFile(std::string(file_url), nbase::UTF16ToUTF8(DefaultDirctoryToStoreFiles()), cbCompleted, true, cbProgress, "", "");
}

void MD5SyncHelper::RequestWithToken(const std::string& token)
{
	this->Cancel();

	token_ = token;
	step_ = Step::Requesting;
}

void MD5SyncHelper::Cancel()
{
	recovery_flag_.Cancel();
	
	token_.clear();
	url_2_file_.clear();
	step_ = Step::Idle;
	cmd_groups_blocked_.clear();
	tea_msg_blocked_.clear();
	stu_msg_blocked_.clear();
}

void MD5SyncHelper::SlotClosure(StdClosure closure)
{
	if (closure) { closure(); }
}

void MD5SyncHelper::OnFileDownloaded(const std::string& file_url, const std::wstring& file_path)
{
	url_2_file_.emplace(file_url, file_path);

	if (url_2_file_.size() == 2)
	{
		DBG_WRAPPER(LOG_NWAR(L"MD5 Sync -- Parsing recovery files..."));

		const auto file0 = url_2_file_.cbegin()->second;
		const auto file1 = (++url_2_file_.cbegin())->second;

		std::unordered_map<std::wstring, CmdExecutor> files
		{
			{ file0, COURSE_DATD_HELPER->ExecutorOfFile(file0) },
			{ file1, COURSE_DATD_HELPER->ExecutorOfFile(file1) },
		};
		
		qtbase::Post2Task(kThreadUIHelper, recovery_flag_.ToWeakCallback([=]{
			auto cmd_groups = COURSE_DATD_HELPER->ParseFromFiles(files, DefaultCommandFactory(), true);
			emit SignalClosure([=]{ OnFileParsed(cmd_groups); });
		}));
	}
}

void MD5SyncHelper::OnFileParsed(const CmdGroupWithExecutors& cmd_groups)
{
	DBG_WRAPPER(LOG_NWAR(L"MD5 Sync -- Downloading resource files refered..."));

	std::vector<CommandGroupSharedPtr> groups;
	for (const auto& pair : cmd_groups)
	{
		groups.emplace_back(pair.cmd_group_);
	}
	for (const auto& cmd_group : cmd_groups_blocked_)
	{
		groups.emplace_back(cmd_group);
	}

	COURSE_DATD_HELPER->DownloadResourcesReferedByCmdGroups(groups, recovery_flag_.ToWeakCallback([=]{

		if (this->thread() == QThread::currentThread())
		{
			StartRecovery(cmd_groups);
		}
		else
		{
			emit SignalClosure([=]{ StartRecovery(cmd_groups); });
		}
	}));
}

void  MD5SyncHelper::StartRecovery(const CmdGroupWithExecutors& cmd_groups)
{
	auto handler = command_handler_.lock();
	if (handler)
	{
		handler->StartRecovery(cmd_groups);
	}
}

void MD5SyncHelper::BlockMessageAndCmdGroup(const std::string& msg, bool sender_is_self, const CommandGroupSharedPtr& cmd_group)
{
	cmd_groups_blocked_.emplace_back(cmd_group);

	Strings& strings = (sender_is_self == ClientTypeIsStudent()) ? stu_msg_blocked_ : tea_msg_blocked_;
	strings.emplace_back(msg);
}

void MD5SyncHelper::ResetMD5()
{
	md5_of_all_master_chess_cmd_.clear();
	md5_temp_to_sync_.clear();
}

void MD5SyncHelper::EmitSync()
{
	auto handler = command_handler_.lock();
	if (handler)
	{
		md5_temp_to_sync_ = md5_of_all_master_chess_cmd_;

		handler->SendCommand(CmdTeacherStartSyncCurrentChessBoard::Create());
	}
}

GAME_NAMESPACE_END