#include "client_log.h"
#include "base\util\string_number_conversions.h"
#include "base\util\string_util.h"
#include "base\file\file_util.h"
#include "http_request\api_caller.h"
#include "nim_sdk_manager\api\nim_tools_http_cpp.h"
#include "log_manager\log.h"
#include "common\tool\common_tool.h"
#include "..\common\system\system_tool.h"
#include "base\util\base64.h"
#define TEST
void ClientLog::InitLog(std::string code_version, std::string api_path)
{
	code_version_ = code_version;
	api_path_ = api_path;
}

void ClientLog::ApiSubmitClientLog(ClientLogParam param)
{
#ifdef TEST
	ApiSubmitLog(param);
	return;
#endif // TEST

	//bizId    错误码
	//type  类型
	//content   内容
	//logUrls    .dump
	//source    来源
	//courseId 课程id

	const static std::string BOUNDARY = "BOUNDARYfareoigj9032490aggsrths";

	std::string multipart_form_body;
	//log_type
	multipart_form_body.append("--").append(BOUNDARY).append("\r\n")
		.append("Content-Disposition: form-data; name=\"type\"\r\n\r\n")
		.append(param.log_type).append("\r\n");

	//content
	multipart_form_body.append("--").append(BOUNDARY).append("\r\n")
		.append("Content-Disposition: form-data; name=\"content\"\r\n\r\n")
		.append(param.content).append("\r\n");

	//biz-id
	multipart_form_body.append("--").append(BOUNDARY).append("\r\n")
		.append("Content-Disposition: form-data; name=\"bizId\"\r\n\r\n")
		.append(nbase::IntToString(param.biz_id)).append("\r\n");

	//course_id
	multipart_form_body.append("--").append(BOUNDARY).append("\r\n")
		.append("Content-Disposition: form-data; name=\"courseId\"\r\n\r\n")
		.append(nbase::Int64ToString(param.course_id)).append("\r\n");

	//source
	multipart_form_body.append("--").append(BOUNDARY).append("\r\n")
		.append("Content-Disposition: form-data; name=\"source\"\r\n\r\n")
		.append(param.source).append("\r\n");

	if (!param.file.empty())
	{
		std::string suffix = "";
		if (param.content == "pc_crash_stu" || param.content == "pc_crash")
		{
			suffix = ".dmp";
		}
		std::string file_name = nbase::StringPrintf("PC_LOGFILE_%s_%s_%lld%s", systembase::GetMac().c_str(), code_version_.c_str(), systembase::get_time_ms(), suffix.c_str());
		std::string file_content;
		nbase::ReadFileToString(nbase::UTF8ToUTF16(param.file), file_content);
		multipart_form_body.append("--").append(BOUNDARY).append("\r\n")
			.append("Content-Disposition: form-data; name=\"file\"; filename=\"").append(file_name).append("\"\r\n")
			.append("Content-Type: application/octet-stream\r\n\r\n")
			.append(file_content).append("\r\n");
	}

	multipart_form_body.append("--").append(BOUNDARY).append("--\r\n");


	ApiCaller api_caller;
	SS_MAP param_map_;
	param_map_["_body__"] = multipart_form_body;

	std::string url = api_caller.assemble_post_url_(api_path_, param_map_);
 	std::string ua = api_caller.build_user_agent_();

	auto comp_cb = [=](bool b_success, int code, const std::string& s)
	{
		//cout << b_success << " " << code << " " << s << endl;
		QLOG_APP(L"async upload log {0} {1} {2}") << b_success << code << s;
	};
	auto prog_cb = [=](double d1, double d2, double d3, double d4)
	{
		//cout << d1 << " " << d2 << " " << d3 << " " << d4 << endl;
	};
	//需要一个Coockie
	nim_http::HttpRequest request(url, multipart_form_body.c_str(), multipart_form_body.size(), comp_cb, prog_cb);

	request.AddHeader("User-Agent", ua);
	request.AddHeader("Content-Type", std::string("multipart/form-data; boundary=").append(BOUNDARY));
	request.AddHeader("Cookie", "");
	request.AddHeader("xy-nonce", param_map_["ts"]);
	request.AddHeader("xy-sign", param_map_["sign"]);
	request.AddHeader("xy-access-key", "jyxb");

	request.SetMethodAsPost();
	request.SetTimeout(20000);

	PostRequest(request);
}

void ClientLog::ApiSubmitLog(ClientLogParam param)
{
	ApiCaller api_caller;
	SS_MAP ss_param;
	ss_param["bizId"] = param.biz_id;
	ss_param["content"] = param.content;
	ss_param["couseId"] = param.course_id;
	ss_param["type"] = param.log_type;
	ss_param["source"] = param.source;
	ss_param["logUrls"] = param.log_urls;
	api_caller.AsyncCallPost(api_path_, ss_param, nullptr);
}
