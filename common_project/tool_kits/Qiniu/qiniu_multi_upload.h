#pragma once
#include <atomic>
#include <functional>
#include <string.h>

typedef std::function<void(bool, std::string)> qiniu_upload_finished_cb;
typedef std::function<void(double)> qiniu_upload_progress_cb;

struct QiNiuUploadInfo
{
	std::wstring file_name_;
	std::wstring file_path_;		//目标文件地址
	std::wstring temp_path_;		//原始文件地址
	std::string token_;
	std::string base_url_;

	QiNiuUploadInfo() : file_name_(L""), file_path_(L""), token_(""), base_url_(""), temp_path_(L"")
	{

	}
};

class QiniuMultiUpload
{
public:
	bool UploadFile(QiNiuUploadInfo info, const qiniu_upload_finished_cb& finished_cb);
	bool UploadFile(QiNiuUploadInfo info, const qiniu_upload_progress_cb& progress_cb, const qiniu_upload_finished_cb& finished_cb);
	//bool ResumableUploadFile(UploadVideoFile videofile, const qiniu_upload_progress_cb& progress_cb, const qiniu_upload_finished_cb& cb);

private:
	void DoUpload(std::string& token, std::string& base, std::string& name, std::string& file, const qiniu_upload_finished_cb& cb);
	void DoUploadFile(std::string& token, std::string& base, std::string& name, std::string& file, 
							const qiniu_upload_progress_cb& progress_cb, const qiniu_upload_finished_cb& finished_cb);
	void DoResumableUpload(std::string token, std::string name, std::string file, 
								const qiniu_upload_progress_cb& progress_cb, const qiniu_upload_finished_cb& cb);
	__int64 getFileSzie(const char *localFile);
	char* GetProgressFilePath(const char* bucket, const char* key, const char * localFilePath);

public:
	static void Init();
	static void Clean();


};