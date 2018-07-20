#include "common_tool.h"
#include "base\win32\path_util.h"
#include "base\util\string_util.h"
#include "nim_sdk_manager\api\nim_cpp_tool.h"
#include "base\network\network_util.h"
#include <iptypes.h>
#include <iphlpapi.h>

std::string commontool::GetAppDataLocalDir()
{
	std::string p = nbase::UTF16ToUTF8(nbase::win32::GetLocalAppDataDir());
	return p;
}

std::wstring commontool::GetAppPath()
{
	return nbase::win32::GetCurrentModuleDirectory();
}

std::string commontool::GetGuid()
{
	return nim::Tool::GetUuid();
}


std::string commontool::GetUserAppdataDir(const std::string& app_account)
{
	return nim::Tool::GetUserAppdataDir(app_account);
}

bool commontool::GetMacAddress(std::string &address)
{
	return nbase::GetMacAddress(address);
}

bool commontool::UnicodeToMBCS(const std::wstring& input, std::string &output, int code_page)
{
	return nbase::win32::UnicodeToMBCS(input, output, code_page);
}

std::wstring commontool::GetGeneralGrade(std::wstring grade)
{
	if (grade == L"初一" || grade == L"初二" || grade == L"初三")
	{
		return L"初中";
	}
	else if (grade == L"高一" || grade == L"高二" || grade == L"高三")
	{
		return L"高中";
	}
	else if (grade == L"一年级" || grade == L"二年级" || grade == L"三年级" || grade == L"四年级" || grade == L"五年级" || grade == L"六年级")
	{
		return L"小学";
	}
	else if (grade == L"大学")
	{
		return L"大学";
	}
	return L"";
}

bool commontool::IsJsonString(std::string json_str)
{
	if (json_str.size() < 2 || json_str[0] != '{') {
		return false;
	}

	int num = 1;
	for (int i = 1; i < json_str.size(); ++i) {
		if (json_str[i] == '{') {
			++num;
		}
		else if (json_str[i] == '}') {
			--num;
		}

		if (num == 0) {
			return true;
		}
	}
	return false;
}

std::string commontool::GetMac()
{
	static std::string mac("");

	if (!mac.empty())
	{
		return mac;
	}
	IP_ADAPTER_INFO adapter[50];  //Maximum 5 adapters

	DWORD buflen = sizeof(adapter);

	DWORD status = GetAdaptersInfo(adapter, &buflen);

	BYTE s[8];

	if (status == ERROR_SUCCESS)
	{

		PIP_ADAPTER_INFO painfo = adapter;
		memcpy(s, painfo->Address, 6);

		mac = nbase::BinaryToHexString((char*)s, 6);
	}

	return mac;
}

