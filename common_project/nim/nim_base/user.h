#pragma once
#include <string>

namespace nim_comp
{
/**
* 获取用户数据目录位置，"\\"结尾
* @return wstring 用户数据目录
*/
std::wstring GetUserDataPath(std::string account);

/**
* 获取用户图片数据目录位置，"\\"结尾
* @return wstring 用户图片数据目录
*/
std::wstring GetUserImagePath(std::string account);

/**
* 获取用户语音数据目录位置，"\\"结尾
* @return wstring 用户语音数据目录
*/
std::wstring GetUserAudioPath(std::string account);
}