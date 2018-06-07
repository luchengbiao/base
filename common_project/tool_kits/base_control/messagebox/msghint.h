#pragma once
#include <string>
#include <functional>

enum MsgBoxRet
{
	MB_YES,
	MB_NO,
	MB_OTHER,
};

typedef std::function<void(MsgBoxRet)> MsgboxCallback;

void ShowAutoCloseMsgBox(std::wstring msg, std::wstring title = L"提示", int delay_time = 3);
void ShowMsgBox(std::wstring msg, MsgboxCallback cb=nullptr, std::wstring title = L"提示", std::wstring ok_text = L"", std::wstring cancel_text = L"", bool b_close_show=true,bool modal = false);  //默认关闭按钮显示 
void ShowMsgToast(std::wstring msg, int delay_time = 3);
void ShowMsgBox_TitleHint(int width, int height, std::wstring msg, MsgboxCallback cb = nullptr, std::wstring title = L"提示", std::wstring ok_text = L"", std::wstring cancel_text = L"", std::wstring cancel_text2 = L"", std::wstring HighLightTitle = L"");
void ShowMsgBox_TitleHintVer2(int width, int height, std::wstring msg, MsgboxCallback cb = nullptr, std::wstring title = L"提示", std::wstring ok_text = L"", std::wstring cancel_text = L"", std::wstring HighLightTitle = L"");//	有标题与内容，带callback功能
void ShowRenameMsgBox(std::wstring msg, MsgboxCallback cb = nullptr, std::wstring title = L"提示", std::wstring ok_text = L"", std::wstring cancel_text = L"", std::wstring other_text = L"");