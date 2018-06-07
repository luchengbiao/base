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

void ShowAutoCloseMsgBox(std::wstring msg, std::wstring title = L"��ʾ", int delay_time = 3);
void ShowMsgBox(std::wstring msg, MsgboxCallback cb=nullptr, std::wstring title = L"��ʾ", std::wstring ok_text = L"", std::wstring cancel_text = L"", bool b_close_show=true,bool modal = false);  //Ĭ�Ϲرհ�ť��ʾ 
void ShowMsgToast(std::wstring msg, int delay_time = 3);
void ShowMsgBox_TitleHint(int width, int height, std::wstring msg, MsgboxCallback cb = nullptr, std::wstring title = L"��ʾ", std::wstring ok_text = L"", std::wstring cancel_text = L"", std::wstring cancel_text2 = L"", std::wstring HighLightTitle = L"");
void ShowMsgBox_TitleHintVer2(int width, int height, std::wstring msg, MsgboxCallback cb = nullptr, std::wstring title = L"��ʾ", std::wstring ok_text = L"", std::wstring cancel_text = L"", std::wstring HighLightTitle = L"");//	�б��������ݣ���callback����
void ShowRenameMsgBox(std::wstring msg, MsgboxCallback cb = nullptr, std::wstring title = L"��ʾ", std::wstring ok_text = L"", std::wstring cancel_text = L"", std::wstring other_text = L"");