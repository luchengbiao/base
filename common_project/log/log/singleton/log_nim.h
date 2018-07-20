#ifndef __LOG_NIM_H__
#define __LOG_NIM_H__
#include "../base/log_singleton_third_party.h"

class LogNim : public LogSingletonThirdParty<LogNim>
{
public:
	virtual void	GetLogContent(std::string& content) override
	{
		content.append("\n================").append(" nim Log begin ").append("================\n");

		LogSingletonThirdParty::GetLogContent(content);

		content.append("\n================").append(" nim Log End ").append("==================\n");
	}
};

#endif