#pragma once
#include <functional>
typedef std::function<void(bool, std::string)> upload_finished_cb;

class QiniuTool
{
public:
	static void Init();
	static void Clean();

	static bool UploadFile(std::string file, const upload_finished_cb& cb = nullptr);

private:
	
	static void DoUpload(std::string& token, std::string& base, std::string& name, std::string& file, const upload_finished_cb& cb = nullptr);
};