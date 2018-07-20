#ifndef __LOG_SINGLETON_INTERFACE_H__
#define __LOG_SINGLETON_INTERFACE_H__
#include <string>

class LogSingletonInterface
{
public:
	virtual  void	GetLogContent(std::string& content) = 0;
};

#endif