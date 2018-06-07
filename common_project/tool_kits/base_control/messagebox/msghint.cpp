#include "msghint.h"
#include "..\toast\stoast.h"
#include "msgbox.h"

void ShowAutoCloseMsgBox(std::wstring msg, std::wstring title /*= L""*/, int delay_time /*= 3*/)
{
	MsgBox *msg_box = new MsgBox();
	msg_box->SetInfo(QString::fromStdWString(title), QString::fromStdWString(msg), QString::fromStdWString(L""), QString::fromStdWString(L""));
	msg_box->SetCallback(nullptr);
	msg_box->show();
	msg_box->SetDelayClose(delay_time);
}

void ShowMsgBox(std::wstring msg, MsgboxCallback cb, std::wstring title /*= L""*/, std::wstring ok_text /*= L""*/, std::wstring cancel_text /*= L""*/, bool b_close_show,bool b_modal)
{
	MsgBox *msg_box = new MsgBox();
	msg_box->SetInfo(QString::fromStdWString(title), QString::fromStdWString(msg), QString::fromStdWString(ok_text), QString::fromStdWString(cancel_text));
	msg_box->SetCallback(cb);
	if (b_modal)
	{
		msg_box->setWindowModality(Qt::ApplicationModal);
		msg_box->exec();
	}
	else
	{
		msg_box->show();
	}
}

void ShowMsgToast(std::wstring msg, int delay_time /*= 3*/)
{
	SingleTonSToast::getInstance().setMessageVDuration(QString::fromStdWString(msg), delay_time * 1000);
}

void ShowMsgBox_TitleHint(int width, int height, std::wstring msg, MsgboxCallback cb, std::wstring title /*= L""*/, std::wstring ok_text /*= L""*/, std::wstring cancel_text /*= L""*/, std::wstring cancel_text2 /*= L""*/, std::wstring HighLightTitle /*= L""*/)
{
	MsgBox *msg_box = new MsgBox();
	msg_box->SetInfo(width,height,QString::fromStdWString(title), QString::fromStdWString(msg), QString::fromStdWString(ok_text), QString::fromStdWString(cancel_text),QString::fromStdWString(cancel_text2),QString::fromStdWString(HighLightTitle));
	msg_box->SetCallback(cb);
	msg_box->show();
}

void ShowMsgBox_TitleHintVer2(int width, int height, std::wstring msg, MsgboxCallback cb, std::wstring title /*= L""*/, std::wstring ok_text /*= L""*/, std::wstring cancel_text /*= L""*/, std::wstring HighLightTitle /*= L""*/)
{
	MsgBox *msg_box = new MsgBox();
	msg_box->SetInfo(width, height, QString::fromStdWString(title), QString::fromStdWString(msg), QString::fromStdWString(ok_text), QString::fromStdWString(cancel_text), QString::fromStdWString(HighLightTitle));
	msg_box->SetCallback(cb);
	msg_box->show();
}

void ShowRenameMsgBox(std::wstring msg, MsgboxCallback cb /*= nullptr*/, std::wstring title /*= L""*/, std::wstring ok_text /*= L""*/, std::wstring cancel_text /*= L""*/, std::wstring other_text /*= L""*/)
{
	MsgBox *msg_box = new MsgBox();
	msg_box->SetInfo(QString::fromStdWString(title), QString::fromStdWString(msg), QString::fromStdWString(ok_text), QString::fromStdWString(cancel_text), QString::fromStdWString(other_text));
	msg_box->SetCallback(cb);
	msg_box->show();
}
