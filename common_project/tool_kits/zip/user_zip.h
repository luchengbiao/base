#pragma once
#include <string>

namespace zip
{
	std::string UnCompressGz(const std::wstring& wpath);
	std::wstring UnCompressZIP(const std::string& path, const std::string& file_dir, const std::wstring& match_str);
	bool CompressGz(const std::wstring& wpath, const std::string& data_str);
}

