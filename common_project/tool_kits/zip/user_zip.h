#pragma once
#include <iostream>

namespace zip
{
	std::string UnCompressGz(std::wstring wpath);
	std::wstring UnCompressZIP(std::string path, std::string file_dir, std::wstring match_str);
	bool CompressGz(std::wstring wpath,std::string data_str);
}

