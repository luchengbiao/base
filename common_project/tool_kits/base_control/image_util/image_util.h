#pragma once
#include "QImage"
#include "string"

namespace ImageUtil
{
	QImage MakeThumbnail(std::wstring file_path, int width, int height);

}