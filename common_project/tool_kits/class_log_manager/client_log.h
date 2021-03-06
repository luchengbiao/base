#include "base\memory\singleton.h"
#include "nim\nim_chatroom_c_sdk\util\nim_base_types.h"
#include <string>

struct ClientLogParam
{
	std::string log_type;
	std::string content;
	std::string source;
	int biz_id;
	int64_t course_id;
	std::string file;
	std::string log_urls;
	std::string dmp_urls;
	ClientLogParam()
	{
		log_type = "";
		content = "";
		source = "";
		biz_id = 0;
		course_id = 0;
		file = "";
		log_urls = "";
		dmp_urls = "";
	}
};


class ClientLog
{
public:
	SINGLETON_DEFINE(ClientLog);
	ClientLog(){};
	~ClientLog(){};
	void InitLog(std::string code_version, std::string api_path);
	void ApiSubmitClientLog(ClientLogParam param);
	void ApiSubmitLog(ClientLogParam param);

private:
	std::string code_version_;
	std::string api_path_;
};