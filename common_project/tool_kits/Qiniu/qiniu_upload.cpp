#include "qiniu_upload.h"
#include "io.h"
#include "qiniu/resumable_io.h"
#include "qiniu/rs.h"
//#include "network/api_caller.h"
#include <thread>
#include "qiniu\http.h"

void QiniuTool::Init()
{
	Qiniu_Global_Init(-1);
}

void QiniuTool::Clean()
{
	Qiniu_Global_Cleanup();
}
 
/*debug 函数*/
void debuginfo(Qiniu_Client* client, Qiniu_Error err)
{
	printf("\nerror code: %d, message: %s\n", err.code, err.message);
	printf("response header:\n%s", Qiniu_Buffer_CStr(&client->respHeader));
	printf("response body:\n%s", Qiniu_Buffer_CStr(&client->b));
	printf("\n\n\n");
}


bool QiniuTool::UploadFile(std::string file, const upload_finished_cb& finished_cb)
{
	//wstring filePath = nbase::UTF8ToUTF16(file);
	//wstring name;
	//nbase::FilePathApartFileName(filePath, name);
	//string upload_name = nbase::UTF16ToUTF8(name);
	//if (upload_name.empty())
	//{
	//	upload_name = file;
	//}
	//string mbcsPath = "";
	//nbase::win32::UnicodeToMBCS(filePath, mbcsPath);

	//auto cb = [=](ApiResult& api_result)
	//{
	//	bool ret = false;
	//	string msg;
	//	if (api_result.Success())
	//	{
	//		string token = api_result.GetDataString("token");
	//		string base = api_result.GetDataString("baseUrl");
	//		std::thread task(nbase::Bind(QiniuTool::DoUpload, token, base, upload_name, mbcsPath, finished_cb));
	//		task.detach();
	//		//StdClosure task = nbase::Bind(QiniuTool::DoUpload, token, base, upload_name, file, finished_cb);
	//		//nbase::ThreadManager::PostTask(kThreadApiTask, task);
	//	}
	//	else
	//	{
	//		ret = false;
	//		msg = "get upload token failed!";
	//		if (finished_cb)
	//		{
	//			finished_cb(ret, msg);
	//		}
	//	}

	//};
	////从服务器获取token
	//SS_MAP param;
	//param["filename"] = upload_name;
	//param["type"] = "course-note";
	//ApiCaller().AsyncCallPost(API_UPLOAD_TOKENEX, param, cb, true);
	return true;
}

void QiniuTool::DoUpload(std::string& token, std::string& base, std::string& name, std::string& file, const upload_finished_cb& cb)
{
	Qiniu_Client client;
	Qiniu_Zero(client);
	Qiniu_Servend_Init(-1);
	Qiniu_Rgn_Disable();
	Qiniu_Client_InitNoAuth(&client, 1024);

	bool ret = false;
	std::string msg;
	Qiniu_Io_PutRet putRet;
	//
	Qiniu_Error error = Qiniu_Io_PutFile(&client, &putRet, token.c_str(), name.c_str(), file.c_str(), NULL);
	if (error.code != 200)
	{
		//QLOG_ERR(L"Upload File {0}, error code: {1}, message: {2}") << file << error.code << error.message;
		//printf("Upload File %s error.\n", file.c_str());
		debuginfo(&client, error);
		ret = false;
		msg = "upload error";
	}
	else
	{
		printf("Upload File %s success.\n", file.c_str());
		ret = true;
		msg = base + name;
	}
	if (cb)
	{
		cb(ret, msg);
	}

	Qiniu_Client_Cleanup(&client);
}

