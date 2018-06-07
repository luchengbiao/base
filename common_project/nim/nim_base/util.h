#pragma once
#include <string>
#include <map>

class QNimString
{
public:
	static std::string GetGUID();
	static std::string GetMd5(const std::string& input);
	static void NIMFreeBuf(void *data);
};

class QPath
{
public:
	static std::wstring GetAppPath(); //获取exe所在目录，最后有"\\"
	static std::wstring GetUserAppDataDir(const std::string& app_account);
	static std::wstring GetLocalAppDataDir();
	static std::wstring GetNimAppDataDir(); // "...Local\\Netease\\Nim\\"
};