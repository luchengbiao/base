#ifndef __PUBLIC_LITERAL_STOCK_LITERAL_STOCK_H__
#define __PUBLIC_LITERAL_STOCK_LITERAL_STOCK_H__
#include <string>
#include <unordered_map>
#include <QString>
#include "base/memory/singleton.h"
#include "base/util/string_util.h"
#include "literal_keys.h"

GAME_NAMESPACE_BEGIN

class LiteralStock
{
	typedef std::unordered_map<std::string, std::string> LiteralMap;

public:
	SINGLETON_DEFINE(LiteralStock);

	std::string	StringByKey(const std::string& key) const;

private:
	LiteralStock();

private:
	LiteralMap literal_map_;
};

inline std::string StdStringByKey(const std::string& key)
{
	return LiteralStock::GetInstance()->StringByKey(key);
}

template<typename... Args>
inline std::string StdStringByKeyWithArgs(const std::string& key, Args&& ... args)
{
	return nbase::StringPrintf(LiteralStock::GetInstance()->StringByKey(key).c_str(), std::forward<Args>(args)...);
}

inline QString QStringByKey(const std::string& key)
{
	return QString::fromLocal8Bit(LiteralStock::GetInstance()->StringByKey(key).c_str());
}

template<typename... Args>
inline QString QStringByKeyWithArgs(const std::string& key, Args&& ... args)
{
	return QString::fromLocal8Bit(StdStringByKeyWithArgs(key, std::forward<Args>(args)...).c_str());
}

GAME_NAMESPACE_END

#endif