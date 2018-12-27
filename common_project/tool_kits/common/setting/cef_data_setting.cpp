#include "cef_data_setting.h"
#include "base\util\string_util.h"
#include "base\win32\scoped_win_handle.h"
#include "base\win32\path_util.h"
#include "base\file\file_util.h"
#include "json\json.h"

CefDataCache::CefDataCache()
{
	data_dir_ = nbase::win32::GetLocalAppDataDir();
}

CefDataCache::~CefDataCache()
{

}

void CefDataCache::Set(std::string key, std::string val)
{
	//CSIDL_LOCAL_APPDATA
	std::string path = nbase::UTF16ToUTF8(data_dir_);
	path.append("\\_db_cef_data.dat");

	std::wstring wpath = nbase::UTF8ToUTF16(path);

	std::string data_str = "";
	nbase::ReadFileToString(wpath, data_str);
	Json::Reader j_reader;
	Json::Value js_value;
	if (j_reader.parse(data_str.c_str(), js_value))
	{
		js_value[key] = val;
	}
	else
	{
		js_value[key] = val;
	}

	nbase::WriteFile(wpath, js_value.toStyledString());
}

std::string CefDataCache::Get(std::string key)
{
	std::string path = nbase::UTF16ToUTF8(data_dir_);
	path.append("\\_db_cef_data.dat");

	std::wstring wpath = nbase::UTF8ToUTF16(path);

	std::string data_str = "";
	nbase::ReadFileToString(wpath, data_str);
	Json::Reader j_reader;
	Json::Value js_value;
	std::string val = "";
	if (j_reader.parse(data_str.c_str(), js_value))
	{
		val = js_value[key].asString();
	}

	return val;
}

void CefDataCache::Test(std::string val)
{
	std::string path = nbase::UTF16ToUTF8(nbase::win32::GetLocalAppDataDir());
	path.append("\\_cef_test.dat");

	std::wstring wpath = nbase::UTF8ToUTF16(path);
	std::string vv = "";
	if (nbase::FilePathIsExist(wpath, false))
	{
		nbase::ReadFileToString(wpath, vv);
	}
	val = vv + val;
	nbase::WriteFile(wpath, val);
}

void CefDataCache::SetDataDir(std::wstring dir)
{
	data_dir_ = dir;
}