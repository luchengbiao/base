#pragma once
#include "base\memory\singleton.h"
#include "base\callback\callback.h"
#include <list>
#include "base\synchronization\lock.h"
/*
* function: 用来日志相关操作  上传 、获取网易 、声网日志 、更改日志内容
* author:zsj
*/
typedef std::function<void(bool, std::string)> upload_finished_cb;

class LogManager : public nbase::SupportWeakCallback
{
public:
	SINGLETON_DEFINE(LogManager);

	LogManager();
	~LogManager();

public:
	void InitLogDir(std::wstring log_dir,std::wstring online_log_dir);
	void OnSetClassOnlineFileName(std::string file_name);       //呼叫 班课重新设置文件名
	void OnUploadNormalLog(int64_t uid, std::string content);				//上传普通日志
	void OnUploadCrashLog(int64_t uid);
	void OnUploadAgoraLog();
	void OnUploadAllClassOnlineFiles();
	void OnPushClassOnlineLogFile();
	void OnHalfToNimLog();   //保留一半的记录
	void OnHalfToAgoraLog();

private:
	void OnGetLocalLog(std::string& content);
	void OnGetNimLog(std::string& content);
	void OnGetAgoraLog(std::string& content);

	void OnCheckOnlineLogUpload();
	void OnStartUploadThread();
	void OnUploadOnlineLog();
	void OnHalfTo(std::wstring file_path);
	void PushClassOnlineLogFileDelay();

	void InitNormalLog();
	void LogNormalRobot();

	bool UploadFile(std::string file, const upload_finished_cb& finished_cb = nullptr);
	static void DoUpload(std::string& token, std::string& base, std::string& name, std::string& file, const upload_finished_cb& cb = nullptr);
private:
	std::string nim_agora_tmp_;    //临时保存声网、网易自己的日志

	std::list<std::wstring> wb_file_path_list_;
	bool upload_thread_init_;
	nbase::NLock wb_file_upload_lock_;


	std::wstring  log_dir_;					//GetJYXBDataDir().append("NIM\\");
	std::wstring  online_log_dir_;			//呼叫 一对一 班课 日志 class_online_log
};