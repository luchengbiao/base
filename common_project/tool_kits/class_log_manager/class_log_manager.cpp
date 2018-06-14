#include "class_log_manager.h"
#include "base\util\string_util.h"
#include "log_manager\class_online_log.h"
#include "base\file\file_path.h"
#include "base\file\file_util.h"
#include "common\system\system_tool.h"
#include "http_request\api_result.h"
#include "http_request\api_caller.h"
#include "base\util\string_number_conversions.h"
#include <thread>
#include "base\memory\deleter.h"
#include "base\win32\platform_string_util.h"
#include "sys/stat.h"
#include "../Qiniu/qiniu_multi_upload.h"
#include "..\common\tool\common_tool.h"
#include "qthread_manager\closure.h"
#include "log_manager\log.h"

#define API_UPLOAD_LOGEX std::string("/xqs-member/external/log/upload")
#define API_UPLOAD_TOKENEX std::string("/xqs-third/external/qiniu/get-uploadToken")
#define TEST
LogManager::LogManager()
{

}

LogManager::~LogManager()
{

}

void LogManager::InitLogDir(std::wstring log_dir,std::wstring online_log_dir)
{
	log_dir_ = log_dir;
	online_log_dir_ = online_log_dir;

	nbase::CreateDirectory(log_dir);
	nbase::CreateDirectory(online_log_dir);

	InitNormalLog();
	LogNormalRobot();
}

void LogManager::OnSetClassOnlineFileName(std::string file_name)
{
	std::wstring full_file = online_log_dir_ + nbase::UTF8ToUTF16(file_name);
	QClassOnlineLogImpl::GetInstance()->SetLogFile(full_file);
	QClassOnlineLogImpl::GetInstance()->SetLogLevel(LV_BANKE);
}

void LogManager::OnUploadNormalLog(int64_t uid, std::string content)
{
	std::wstring user_log = log_dir_ + L"log.txt";

	if (!nbase::FilePathIsExist(user_log, false))
	{
		return;
	}

	uint64_t tmp_time = systembase::get_time_ms();
	std::wstring filename = nbase::StringPrintf(L"PCLOG_normal_%lld_%I64d.log", tmp_time, uid);
	std::wstring uer_tmp_dir = log_dir_ + filename;

	// 把声网，网易以及我们本地的日志上传到服务器
	std::string log_str = "";
	OnGetAgoraLog(log_str);
	OnGetNimLog(log_str);
	OnGetLocalLog(log_str);
	nbase::WriteFile(uer_tmp_dir, log_str);

	auto finish_cb = ToWeakCallback([=](bool ret, std::string& url)
	{
		if (ret)
		{
			auto cb = ToWeakCallback([=](ApiResult & res){
				if (res.Success())
				{
					//nbase::DeleteFile(user_log);
					nbase::DeleteFile(uer_tmp_dir);
				}
			});

			SS_MAP param;
			param["bizId"] = nbase::Uint64ToString(uid);
			param["content"] = content;
			param["type"] = "PC_EVENT";
			param["logUrls"] = url;
			param["source"] = "pc";
			param["courseId"] = "-1";
			ApiCaller().AsyncCallPost(API_UPLOAD_LOGEX, param, cb);
		}
	});

	if (nbase::FilePathIsExist(uer_tmp_dir, false))
	{
		UploadFile(nbase::UTF16ToUTF8(uer_tmp_dir), finish_cb);
	}
}

void LogManager::OnUploadCrashLog(int64_t uid)
{
	std::wstring user_log_ = log_dir_ + L"log.txt";

	if (!nbase::FilePathIsExist(user_log_, false))
	{
		return;
	}

	std::string service_log;
	OnGetNimLog(service_log);
	OnGetAgoraLog(service_log);

	std::string log_str;
	nbase::ReadFileToString(user_log_, log_str);
	log_str.append(service_log);


	uint64_t tmp_time = systembase::get_time_ms();
	std::wstring filename = nbase::StringPrintf(L"PCLOG_normal_%lld_%I64d.log", tmp_time, uid);
	std::wstring uer_tmp_dir = log_dir_ + filename;
	nbase::WriteFile(uer_tmp_dir, log_str);

	auto finish_cb = ToWeakCallback([=](bool ret, std::string& url)
	{
		if (ret)
		{
			auto cb = ToWeakCallback([=](ApiResult & res){
				if (res.Success())
				{
					//nbase::DeleteFile(user_log_);
					nbase::DeleteFile(uer_tmp_dir);
				}
			});

			SS_MAP param;
			param["bizId"] = nbase::Uint64ToString(uid);
			param["content"] = "pc_crash_log";
			param["type"] = "PC_EVENT";
#ifdef TEST
			param["logUrls"] = url;
			param["courseId"] = "-1";
#endif // TEST

#ifndef TEST
			param["logUrls"] = url;
			param["courseId"] = "-1";
#endif // !TEST

			
			param["source"] = "pc";
			ApiCaller().AsyncCallPost(API_UPLOAD_LOGEX, param, cb);
		}
	});

	if (nbase::FilePathIsExist(uer_tmp_dir, false))
	{
		UploadFile(nbase::UTF16ToUTF8(uer_tmp_dir), finish_cb);
	}
}

void LogManager::OnUploadAgoraLog()
{
	std::wstring log_path = QClassOnlineLogImpl::GetInstance()->GetLogFile();
	std::wstring file_dir = online_log_dir_;
	uint64_t tmp_time = systembase::get_time_ms();
	std::wstring filename = nbase::StringPrintf(L"PCLOG_agora_signalling_%lld.log", tmp_time);
	std::wstring uer_tmp_dir = file_dir + filename;

	nbase::CopyFile(log_path, uer_tmp_dir);

	auto finish_cb = ToWeakCallback([=](bool ret, std::string& url)
	{
		if (ret)
		{
			auto cb = ToWeakCallback([=](ApiResult & res){
				if (res.Success())
				{
					nbase::DeleteFile(log_path);
					nbase::DeleteFile(uer_tmp_dir);
				}
			});

			SS_MAP param;
			param["bizId"] = "201";
			param["content"] = "pc_signalling_log";
			param["type"] = "PC_EVENT";
#ifdef TEST
			param["logUrls"] = url;
			param["courseId"] = "-1";
#endif // TEST

#ifndef TEST
			param["logUrls"] = url;
			param["courseId"] = "-1";
#endif // !TEST
			param["source"] = "pc";
			ApiCaller().AsyncCallPost(API_UPLOAD_LOGEX, param, cb);
		}
	});

	if (nbase::FilePathIsExist(uer_tmp_dir, false))
	{
		UploadFile(nbase::UTF16ToUTF8(uer_tmp_dir), finish_cb);
	}
}

void LogManager::OnUploadAllClassOnlineFiles()
{
	OnCheckOnlineLogUpload();
}

void LogManager::OnPushClassOnlineLogFile()
{
	PushClassOnlineLogFileDelay();
}

void LogManager::OnHalfToNimLog()
{
	std::vector<std::wstring> vec_files;
	vec_files.push_back(std::wstring(log_dir_).append(L"nim.log"));
	vec_files.push_back(std::wstring(log_dir_).append(L"nim_tcp.log"));
	vec_files.push_back(std::wstring(log_dir_).append(L"nrtc_chat_session.log"));
	vec_files.push_back(std::wstring(log_dir_).append(L"nrtc_vchat.log"));
	vec_files.push_back(std::wstring(log_dir_).append(L"nrtc_http.log"));
	vec_files.push_back(std::wstring(log_dir_).append(L"nim_chatroom.log"));

	int ret;
	struct _stat buf;
	for (auto i = 2; i < vec_files.size(); i++)
	{
		std::string log_file = "";
		nbase::win32::UnicodeToMBCS(vec_files[i], log_file);
		ret = _stat(log_file.c_str(), &buf);
		if (ret == 0)
		{
			time_t loc_time = 0;
			time(&loc_time);
			if (buf.st_ctime + 3600 * 24 * 5 < loc_time)
			{
				OnHalfTo(vec_files[i]);
			}
		}
	}
}

void LogManager::OnHalfToAgoraLog()
{
	std::vector<std::wstring> vec_files;
	vec_files.push_back(std::wstring(log_dir_).append(L"AgoraSDK.log"));

	int ret;
	struct _stat buf;
	for (auto i = 1; i < vec_files.size(); i++)
	{
		std::string log_file = "";
		nbase::win32::UnicodeToMBCS(vec_files[i], log_file);
		ret = _stat(log_file.c_str(), &buf);
		if (ret == 0)
		{
			time_t loc_time = 0;
			time(&loc_time);
			if (buf.st_ctime + 3600 * 24 * 5 < loc_time)
			{
				OnHalfTo(vec_files[i]);
			}
		}
	}
}

void LogManager::OnGetLocalLog(std::string& content)
{
	std::wstring user_log = log_dir_ + L"log.txt";

	if (!nbase::FilePathIsExist(user_log, false))
	{
		return;
	}

	std::wstring fn_w;
	nbase::FilePathApartFileName(user_log, fn_w);
	content.append("\r\n=======").append(nbase::UTF16ToUTF8(fn_w)).append("========\r\n");
	std::string logStr = "";
	nbase::ReadFileToString(user_log, logStr);
	content.append(logStr);
}

void LogManager::OnGetNimLog(std::string& content)
{
	std::vector<std::wstring> vec_files;
	vec_files.push_back(std::wstring(log_dir_).append(L"nim.log"));
	vec_files.push_back(std::wstring(log_dir_).append(L"nim_tcp.log"));
	vec_files.push_back(std::wstring(log_dir_).append(L"nrtc_chat_session.log"));
	vec_files.push_back(std::wstring(log_dir_).append(L"nrtc_vchat.log"));
	vec_files.push_back(std::wstring(log_dir_).append(L"nrtc_http.log"));
	vec_files.push_back(std::wstring(log_dir_).append(L"nim_chatroom.log"));

	for (auto i = 0; i < vec_files.size(); i++)
	{
		std::wstring fn_w;
		nbase::FilePathApartFileName(vec_files[i], fn_w);
		content.append("\r\n=======").append(nbase::UTF16ToUTF8(fn_w)).append("========\r\n");
		std::string logStr = "";
		nbase::ReadFileToString(vec_files[i], logStr);
		content.append(logStr);
	}

	content.append("\r\n=======").append("NIM Log ").append("========\r\n");
}

void LogManager::OnGetAgoraLog(std::string& content)
{
	std::vector<std::wstring> vec_files;
	vec_files.push_back(std::wstring(log_dir_).append(L"AgoraSDK.log"));

	for (auto i = 0; i < vec_files.size(); i++)
	{
		std::wstring fn_w;
		nbase::FilePathApartFileName(vec_files[i], fn_w);
		content.append("\r\n=======").append(nbase::UTF16ToUTF8(fn_w)).append("========\r\n");
		std::string logStr = "";
		nbase::ReadFileToString(vec_files[i], logStr);
		content.append(logStr);
	}

	content.append("\r\n=======").append("Agora Log ").append("========\r\n");
}

void LogManager::OnCheckOnlineLogUpload()
{
	//每次重新获取一遍已有文件并上传，成功后删除
	std::wstring file = online_log_dir_;

	WIN32_FIND_DATAW finder;
	HANDLE hfd;
	std::wstring s = file + L"*.log";	// nbase::UTF8ToUTF16(s_file);
	hfd = FindFirstFileW(s.c_str(), &finder);
	if (hfd == INVALID_HANDLE_VALUE)
	{
		return;
	}
	std::wstring file_name = finder.cFileName;
	wb_file_path_list_.push_back(file_name);

	while (FindNextFileW(hfd, &finder))
	{
		std::wstring file_name_1 = finder.cFileName;
		wb_file_path_list_.push_back(file_name_1);
	}
	FindClose(hfd);

	OnStartUploadThread();
}

void LogManager::OnStartUploadThread()
{
	upload_thread_init_ = true;
	std::thread task_upload(std::bind(&LogManager::OnUploadOnlineLog, this));
	task_upload.detach();
}

void LogManager::OnUploadOnlineLog()
{
	nbase::NAutoLock auto_lock(&wb_file_upload_lock_);
	if (wb_file_path_list_.size() <= 0)
	{
		upload_thread_init_ = false;
		return;
	}
	std::wstring file_dir_ = online_log_dir_;
	std::wstring file_name = wb_file_path_list_.front();
	wb_file_path_list_.pop_front();

	if (file_name.find(L".log") == std::wstring::npos)
	{
		upload_thread_init_ = false;
		return;
	}

	std::wstring tmp_full_path = file_dir_ + file_name;

	std::string tem_str = "";
	bool read_succ = nbase::ReadFileToString(tmp_full_path, tem_str);
	if (!read_succ)
	{
		OnUploadOnlineLog();
		return;
	}

	//解析文件名 TODO
	std::string file_name_str = nbase::UTF16ToUTF8(file_name);
	int last_index = file_name_str.find_last_of('.log');
	std::string real_file_name = file_name_str.substr(0, last_index - 3);
	std::vector<std::string> name_vec;
	nbase::StringSplit(real_file_name, "_", name_vec);
	if (name_vec.size() < 5)
	{
		upload_thread_init_ = false;
		return;
	}

	std::string im_id = name_vec[0];
	std::string service_type = name_vec[1];
	std::string uid = name_vec[3];
	std::string class_type = name_vec[4];

	std::string service_log;
	if (service_type == "Nim")
	{
		OnGetNimLog(service_log);
	}
	else if (service_type == "Agora")
	{
		OnGetAgoraLog(service_log);
	}

	service_log.append(tem_str);

	uint64_t tmp_time = systembase::get_time_ms();
	std::string tmp_file = "PCLOG_" + im_id + "_" + service_type + "_" + uid + "_" + class_type + nbase::StringPrintf("_%lld.log", tmp_time);
	std::wstring new_file_log_path = file_dir_ + nbase::UTF8ToUTF16(tmp_file);

	if (nbase::WriteFile(new_file_log_path, service_log) == service_log.size())
	{
		//上传TODO
		auto finish_cb = ToWeakCallback([=](bool ret, std::string& url)
		{
			if (ret)
			{
				auto cb = ToWeakCallback([=](ApiResult & res){
					if (res.Success())
					{
						nbase::DeleteFile(tmp_full_path);
						nbase::DeleteFile(new_file_log_path);
						OnStartUploadThread();
					}
				});

				SS_MAP param;
				param["bizId"] = im_id;
				param["content"] = (service_type == "Nim") ? "PC_Nim_log" : "PC_Agora_log";
				param["type"] = "PC_EVENT";
#ifdef TEST
				param["logUrls"] = url;
				param["courseId"] = "-1";
#endif // TEST

#ifndef TEST
				param["logUrls"] = url;
				param["courseId"] = "-1";
#endif // !TEST
				param["source"] = "pc";
				ApiCaller().AsyncCallPost(API_UPLOAD_LOGEX, param, cb);
			}
		});

		if (nbase::FilePathIsExist(new_file_log_path, false))
		{
			UploadFile(nbase::UTF16ToUTF8(new_file_log_path), finish_cb);
		}
		else
		{
			OnUploadOnlineLog();
		}
	}
	else
	{
		OnUploadOnlineLog();
	}
}

void LogManager::OnHalfTo(std::wstring file_path)
{
	long max = 2*1024 * 1024;
	//打开文件
	std::unique_ptr<FILE, nbase::DeleterFileClose> fp;
	FILE* fp_file = nullptr;
	_wfopen_s(&fp_file, file_path.c_str(), L"r");
	fp.reset(fp_file);
	if (!fp)
	{
		fp.reset(NULL);
		return;
	}

	//获取长度
	int ret = fseek(fp.get(), 0L, SEEK_END);
	if (ret != 0)
	{
		return;
	}

	//小于max则直接返回
	long len = ftell(fp.get());
	if (len <= max)
	{
		return;
	}

	//大于max，只留下最后to
	len = max * (-1);
	ret = fseek(fp.get(), len, SEEK_END);
	if (ret != 0)
	{
		return;
	}

	//创建新文件
	nbase::PathString new_file = file_path + L".old";
	std::unique_ptr<FILE, nbase::DeleterFileClose> fp2;
	FILE* fp_file2 = nullptr;
	_wfopen_s(&fp_file2, new_file.c_str(), L"w");
	fp2.reset(fp_file2);
	if (!fp2)
	{
		return;
	}

	//写入新文件
	char cbuf[12 * 1024] = { 0 };
	int cn = sizeof(cbuf), n = 0;
	while (!feof(fp.get()))
	{
		n = fread_s(cbuf, cn, sizeof(char), cn, fp.get());
		if (n > 0)
		{
			fwrite(cbuf, sizeof(char), n, fp2.get());
		}
		else
		{
			break;
		}
	}
	fp.reset(NULL);
	fp2.reset(NULL);

	//文件替换
	bool del = nbase::DeleteFileW(file_path);
	if (del)
	{
		::_wrename(new_file.c_str(), file_path.c_str());
	}
	else
	{
		nbase::DeleteFileW(new_file);
	}
}

void LogManager::PushClassOnlineLogFileDelay()
{
	nbase::NAutoLock auto_lock(&wb_file_upload_lock_);
	std::wstring class_log = QClassOnlineLogImpl::GetInstance()->GetLogFile();
	OnSetClassOnlineFileName("");
	std::wstring file_name;
	nbase::FilePathApartFileName(class_log, file_name);
	wb_file_path_list_.push_back(file_name);

	if (!upload_thread_init_)
	{
		OnStartUploadThread();
	}
}

bool LogManager::UploadFile(std::string file, const upload_finished_cb& finished_cb /*= nullptr*/)
{
	std::wstring filePath = nbase::UTF8ToUTF16(file);
	std::wstring name;
	nbase::FilePathApartFileName(filePath, name);
	std::string upload_name = nbase::UTF16ToUTF8(name);
	if (upload_name.empty())
	{
		upload_name = file;
	}
	std::string mbcsPath = "";
	nbase::win32::UnicodeToMBCS(filePath, mbcsPath);

	auto cb = ToWeakCallback([=](ApiResult& api_result)
	{
		bool ret = false;
		std::string msg;
		if (api_result.Success())
		{
			std::string token = api_result.GetDataString("token");
			std::string base = api_result.GetDataString("baseUrl");
			std::thread task(nbase::Bind(LogManager::DoUpload, token, base, upload_name, mbcsPath, finished_cb));
			task.detach();
		}
		else
		{
			ret = false;
			msg = "get upload token failed!";
			if (finished_cb)
			{
				finished_cb(ret, msg);
			}
		}

	});
	//从服务器获取token
	SS_MAP param;
	param["fileName"] = upload_name;
	//param["type"] = "course-note";
	ApiCaller().AsyncCallGet(API_UPLOAD_TOKENEX, param, cb, true);
	return true;
}

void LogManager::DoUpload(std::string& token, std::string& base, std::string& name, std::string& file, const upload_finished_cb& cb /*= nullptr*/)
{
	QiniuMultiUpload upload_note;
	QiNiuUploadInfo qiniu;
	qiniu.base_url_ = base;
	qiniu.token_ = token;
	qiniu.file_path_ = nbase::UTF8ToUTF16(file);
	qiniu.file_name_ = nbase::UTF8ToUTF16(name);
	upload_note.UploadFile(qiniu,cb);
}

void LogManager::InitNormalLog()
{
#ifdef _DEBUG
	QLogImpl::GetInstance()->SetLogLevel(LV_PRO);
#else
	QLogImpl::GetInstance()->SetLogLevel(LV_APP);
#endif
	QLogImpl::GetInstance()->SetLogFile(log_dir_ + L"log.txt");
}

void LogManager::LogNormalRobot()
{
	const long m2 = 2 * 1024 * 1024;
	QLogImpl::GetInstance()->HalfTo(m2);
	OnHalfToNimLog();
	OnHalfToAgoraLog();

	qtbase::Post2DelayedTask(kThreadMoreTaskHelper, nbase::Bind(&LogManager::LogNormalRobot, this), nbase::TimeDelta::FromMinutes(10));
}
