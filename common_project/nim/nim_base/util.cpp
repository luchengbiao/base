#include "util.h"
#include <assert.h>
#include <vector>
#include "base\util\string_util.h"
#include "nim_sdk_manager\api\nim_cpp_tool.h"
#include "nim_sdk_manager\api\nim_cpp_global.h"
#include "base\win32\path_util.h"

std::string QNimString::GetGUID()
{
	return nim::Tool::GetUuid();
}

std::string QNimString::GetMd5(const std::string& input)
{
	return nim::Tool::GetMd5(input);
}

void QNimString::NIMFreeBuf(void *data)
{
	return nim::Global::FreeBuf(data);
}

std::wstring QPath::GetAppPath()
{
	return nbase::win32::GetCurrentModuleDirectory();
}

std::wstring QPath::GetUserAppDataDir(const std::string& app_account)
{
	return nbase::UTF8ToUTF16(nim::Tool::GetUserAppdataDir(app_account));
}

std::wstring QPath::GetLocalAppDataDir()
{
	return nbase::UTF8ToUTF16(nim::Tool::GetLocalAppdataDir());
}

std::wstring QPath::GetNimAppDataDir()
{
	std::wstring dir = QPath::GetLocalAppDataDir();
#ifdef _DEBUG
	dir.append(L"Netease\\NIM_Debug\\");
#else
	dir.append(L"Netease\\NIM\\");
#endif

	return dir;
}