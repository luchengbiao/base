#pragma once
#include "base\memory\singleton.h"
#include "base\callback\callback.h"
#include <list>
#include "base\synchronization\lock.h"
/*
* function: ������־��ز���  �ϴ� ����ȡ���� ��������־ ��������־����
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
	void OnSetClassOnlineFileName(std::string file_name);       //���� ������������ļ���
	void OnUploadNormalLog(int64_t uid, std::string content);				//�ϴ���ͨ��־
	void OnUploadCrashLog(int64_t uid);
	void OnUploadAgoraLog();
	void OnUploadAllClassOnlineFiles();
	void OnPushClassOnlineLogFile();
	void OnHalfToNimLog();   //����һ��ļ�¼
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
	std::string nim_agora_tmp_;    //��ʱ���������������Լ�����־

	std::list<std::wstring> wb_file_path_list_;
	bool upload_thread_init_;
	nbase::NLock wb_file_upload_lock_;


	std::wstring  log_dir_;					//GetJYXBDataDir().append("NIM\\");
	std::wstring  online_log_dir_;			//���� һ��һ ��� ��־ class_online_log
};