#include "user_zip.h"
#include "zip\zlib.h"
#include "zip\zip.h"
#include "zip\unzip.h"
#include "base\util\string_util.h"
#include "common\tool\common_tool.h"
#include "base\file\file_util.h"

std::wstring zip::UnCompressZIP(const std::string& path, const std::string& file_dir, const std::wstring& match_str)
{
	std::wstring install_path = L"";
	std::wstring file_path = nbase::UTF8ToUTF16(file_dir);
	std::wstring zip_file = nbase::UTF8ToUTF16(path);
	if (!nbase::FilePathIsExist(file_path, true)){
		nbase::CreateDirectory(file_path);
	}
	HZIP hz = OpenZip(zip_file.c_str(), 0);
	SetUnzipBaseDir(hz, file_path.c_str());
	ZIPENTRY ze;
	GetZipItem(hz, -1, &ze);
	int numitems = ze.index;
	
	for (int i = 0; i < numitems; i++)
	{
		GetZipItem(hz, i, &ze);
		UnzipItem(hz, i, ze.name);
		std::wstring w_str(ze.name);
		if (w_str != L"" && match_str != L"" && w_str.find(match_str) != -1)
		{
			install_path = file_path + ze.name;
		}
	}
	if (hz)
	{
		CloseZip(hz);
	}
	return install_path;
}

std::string zip::UnCompressGz(const std::wstring& wpath)
{
	std::string path = "";
	commontool::UnicodeToMBCS(wpath, path);
	gzFile gzfp = gzopen(path.c_str(), "rb");
	if (!gzfp)
	{
		return "";
	}
	std::string out = "";
	char buf[1024];
	int have;
	while ((have = gzread(gzfp, buf, sizeof(buf))) > 0)
	{
		out.append(buf, have);
	}
	gzclose(gzfp);
	return out;
}

bool zip::CompressGz(const std::wstring& wpath, const std::string& data_str)
{
	std::string path = "";
	commontool::UnicodeToMBCS(wpath, path);
	gzFile gzf = gzopen(path.c_str(), "wb");
	if (!gzf)
	{
		gzclose(gzf);
		return false;
	}
	int size = gzwrite(gzf, data_str.c_str(), data_str.length());
	if (size != data_str.length())
	{
		gzclose(gzf);
		return false;
	}

	gzclose(gzf);
	return true;
}
