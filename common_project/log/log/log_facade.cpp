#include "log_facade.h"
#include <thread>
#include <mutex>
#include "base\util\string_number_conversions.h"
#include "base\win32\platform_string_util.h"
#include "common\system\system_tool.h"
#include "common\weak_callback\weak_callback.h"
#include "http_request\api_caller.h"
#include "http_request\api_result.h"
#include "qiniu_upload\qiniu_multi_upload.h"
#include "qthread_manager\closure.h"
#include "log_custom_item.h"
#include "singleton/log_agora.h"
#include "singleton/log_class_online.h"
#include "singleton/log_common.h"
#include "singleton/log_nim.h"
#include "util/log_output_util.h"
#include "util/log_upload_uitl.h"

class LogFacadeImpl : public wcb::SupportWeakCallback<LogFacadeImpl>
{
public:
	LogFacadeImpl()
	{
		is_uploading_class_online_log_.store(false);
	}

	inline void SetUploadApi(const std::string& api_upload_log, const std::string& api_get_upload_token)
	{
		log_upload_util::SetUploadApi(api_upload_log, api_get_upload_token);
	}

	inline void SetCommonLogDirectory(const std::wstring& common_log_dir)
	{
		common_log_dir_ = common_log_dir;

		nbase::CreateDirectory(common_log_dir);

		LogCommon::GetInstance()->SetLogLevel(LogCommonLevel::Info);

		LogCommon::GetInstance()->SetLogFilePath(common_log_dir + L"log.txt");

		std::vector<std::wstring> nim_files =
		{
			common_log_dir + L"nim.log",
			common_log_dir + L"nim_tcp.log",
			common_log_dir + L"nrtc_chat_session.log",
			common_log_dir + L"nrtc_vchat.log",
			common_log_dir + L"nrtc_http.log",
			common_log_dir + L"nim_chatroom.log",
		};

		LogNim::GetInstance()->SetLogFilePaths(std::move(nim_files));

		std::vector<std::wstring> agora_files =
		{
			common_log_dir + L"AgoraSDK.log",
		};

		LogAgora::GetInstance()->SetLogFilePaths(std::move(agora_files));

		qtbase::Post2Task(kThreadMoreTaskHelper, ToWeakCallback([=]{ this->MaintainLogFilesLength(); }));
	}

	inline void SetClassOnlineLogFilePath(const std::wstring& class_online_log_path)
	{
		std::wstring class_online_log_dir;
		nbase::FilePathApartDirectory(class_online_log_path, class_online_log_dir);

		nbase::CreateDirectory(class_online_log_dir);

		LogClassOnline::GetInstance()->SetLogFilePath(class_online_log_path);
	}

	void UploadClassOnlineLog()
	{
		if (is_uploading_class_online_log_.load())
		{
			return;
		}

		is_uploading_class_online_log_.store(true);

		std::thread task_thread([=]{ this->DoUploadClassOnlineLog(); });
		task_thread.detach();
	}

	void UploadCrashLog(int64_t biz_id, const std::string& content)
	{
		uint64_t tmp_time = systembase::get_time_ms();
		std::wstring filename = nbase::StringPrintf(L"PCLOG_crash_%lld_%I64d.log", tmp_time, biz_id);

		std::vector<LogSingletonInterface*> vec_log_singleton =
		{
			LogAgora::GetInstance(),
			LogNim::GetInstance(),
			LogCommon::GetInstance(),
		};

		log_upload_util::UploadLogsInSingleOneFile(vec_log_singleton, nbase::Uint64ToString(biz_id), content, common_log_dir_ + filename);
	}

	void UploadCustomLog(const CustomLogItem& log_item)
	{
		SS_MAP ss_param =
		{
			{ "bizId", nbase::IntToString(log_item.biz_id_) },
			{ "content", log_item.content_ },
			{ "couseId", nbase::Int64ToString(log_item.course_id_) },
			{ "type", log_item.log_type_ },
			{ "source", log_item.source_ },
			{ "logUrls", log_item.log_urls_ },
		};

		log_upload_util::UploadCustomLog(ss_param);
	}

	void HandleConsoleCommand(const std::string& command)
	{
		log_output_util::HandleConsoleCommand(nbase::UTF8ToUTF16(command));
	}

protected:
	void MaintainLogFilesLength()
	{
		const long m2 = 2 * 1024 * 1024; // cut if longer than 2M

		LogCommon::GetInstance()->RemoveHalfIfLongerThan(m2);
		LogNim::GetInstance()->RemoveHalfIfLongerThan(m2);
		LogAgora::GetInstance()->RemoveHalfIfLongerThan(m2);

		qtbase::Post2DelayedTask(kThreadMoreTaskHelper, ToWeakCallback([=]{ this->MaintainLogFilesLength(); }), nbase::TimeDelta::FromMinutes(10));
	}

	void DoUploadClassOnlineLog()
	{
		bool failed = false;

		do 
		{
			std::wstring file_path = LogClassOnline::GetInstance()->GetLogFilePath();
			std::wstring file_name;
			nbase::FilePathApartFileName(file_path, file_name);

			auto index = file_name.find_last_of(L".log");
			if (index == file_name.length() - 1)
			{
				file_name = file_name.substr(0, index - 3);
			}

			std::vector<std::wstring> name_vec;
			nbase::WstringSplit(file_name, L"_", name_vec);

			if (name_vec.size() < 5) 
			{ 
				failed = true;  
				break; 
			}

			auto im_id = name_vec[0];
			auto service_type = name_vec[1];
			auto uid = name_vec[3];
			auto class_type = name_vec[4];

			LogSingletonInterface* log_third_party = nullptr;
			if (service_type == L"Nim")
			{
				log_third_party = LogNim::GetInstance();
			}
			else if (service_type == L"Agora")
			{
				log_third_party = LogAgora::GetInstance();
			}

			std::wstring file_dir;
			nbase::FilePathApartDirectory(file_path, file_dir);

			uint64_t tmp_time = systembase::get_time_ms();
			std::wstring tmp_file_name = L"PCLOG_" + im_id + L"_" + service_type + L"_" + uid + L"_" + class_type + nbase::UTF8ToUTF16(nbase::StringPrintf("_%lld.log", tmp_time));
			std::wstring tmp_file_path = file_dir + tmp_file_name;

			std::vector<LogSingletonInterface*> vec_log_singleton = {  LogClassOnline::GetInstance() };
			if (log_third_party) 
			{ 
				vec_log_singleton.emplace(std::begin(vec_log_singleton), log_third_party);
			}

			std::string biz_id = nbase::UTF16ToUTF8(im_id);
			std::string content = (service_type == L"Nim") ? "PC_Nim_log" : "PC_Agora_log";
			
			auto task_cb = ToWeakCallback([=](bool, const std::string&){
				LogClassOnline::GetInstance()->SetLogFilePath(L"");
				is_uploading_class_online_log_.store(false);
			});

			log_upload_util::UploadLogsInSingleOneFile(vec_log_singleton, biz_id, content, tmp_file_path, task_cb);

		} while (false);

		if (failed)
		{
			LogClassOnline::GetInstance()->SetLogFilePath(L"");

			is_uploading_class_online_log_.store(false);
		}
	}

private:
	std::wstring		common_log_dir_;

	std::atomic_bool	is_uploading_class_online_log_;
};

LogFacade::LogFacade()
	: impl_(std::make_shared<LogFacadeImpl>())
{}

void LogFacade::SetUploadApi(const std::string& api_upload_log, const std::string& api_get_upload_token)
{
	impl_->SetUploadApi(api_upload_log, api_get_upload_token);
}

void LogFacade::SetCommonLogDirectory(const std::wstring& common_log_dir)
{
	impl_->SetCommonLogDirectory(common_log_dir);
}

void LogFacade::SetClassOnlineLogFilePath(const std::wstring& class_online_log_path)
{
	impl_->SetClassOnlineLogFilePath(class_online_log_path);
}

void LogFacade::UploadClassOnlineLog()
{
	impl_->UploadClassOnlineLog();
}

void LogFacade::UploadCrashLog(int64_t upload_id, const std::string& upload_str_id)
{
	impl_->UploadCrashLog(upload_id, upload_str_id);
}

void LogFacade::UploadCustomLog(const CustomLogItem& log_item)
{
	impl_->UploadCustomLog(log_item);
}

void LogFacade::HandleConsoleCommand(const std::string& command)
{
	impl_->HandleConsoleCommand(command);
}
