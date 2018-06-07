#ifndef COMMON_TOOL_H
#define COMMON_TOOL_H
#include <iostream>
#include "base\win32\platform_string_util.h"

namespace commontool
{
	/*	获取本地存储目录	*/
	std::string GetAppDataLocalDir();
	/*	获取程序运行目录	*/
	std::wstring GetAppPath();
	/*	获取GUID		*/
	std::string GetGuid();

	std::string GetUserAppdataDir(const std::string& app_account);
	/*获取本机的MAC地址*/
	bool GetMacAddress(std::string &address);

	std::string GetMac();

	bool UnicodeToMBCS(const std::wstring& input, std::string &output, int code_page = CP_ACP);
	std::wstring GetGeneralGrade(std::wstring grade);

	bool IsJsonString(std::string json_str);
}
#endif	//	COMMON_TOOL_H