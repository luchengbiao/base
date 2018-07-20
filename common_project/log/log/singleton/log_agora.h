#ifndef __LOG_AGORA_H__
#define __LOG_AGORA_H__
#include "../base/log_singleton_third_party.h"

class LogAgora : public LogSingletonThirdParty<LogAgora>
{
public:
	virtual void	GetLogContent(std::string& content) override
	{
		content.append("\n================").append(" agora log begin ").append("================\n");

		LogSingletonThirdParty::GetLogContent(content);

		content.append("\n================").append(" agora log end ").append("==================\n");
	}
};

#endif