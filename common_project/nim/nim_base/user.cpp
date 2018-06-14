﻿#include "user.h"
#include <assert.h>
#include "util.h"

namespace
{
	static const std::wstring kResImage = L"image\\";
	static const std::wstring kResAudio = L"audio\\";
}

namespace nim_comp
{
std::wstring GetUserDataPath(std::string account)
{
	assert( !account.empty() );
	std::wstring user_app_data_dir = QPath::GetUserAppDataDir( account );
	return user_app_data_dir;
}

std::wstring GetUserImagePath(std::string account)
{
	std::wstring dir = GetUserDataPath(account);
	dir.append(kResImage);
	return dir;
}

std::wstring GetUserAudioPath(std::string account)
{
	std::wstring dir = GetUserDataPath(account);
	dir.append(kResAudio);
	return dir;
}
}