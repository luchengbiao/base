#include "msghint.h"
#include "msgbox.h"
#include "..\toast\stoast.h"
#include "..\common\uiutils\ui_utils.h"

static const QWidget* GetCurrentTopWidget()
{
	QWidget* parent_widget = nullptr;
	auto top_widget = qApp->focusWidget();

	while (top_widget && (parent_widget = top_widget->parentWidget()))
	{
		top_widget = parent_widget;
	}

	return top_widget;
}

void ShowAutoCloseMsgBox(std::wstring msg, std::wstring title /*= L""*/, int delay_time /*= 3*/)
{
	auto top_widget = GetCurrentTopWidget();

	MsgBox *msg_box = new MsgBox();
	msg_box->SetInfo(QString::fromStdWString(title), QString::fromStdWString(msg), QString::fromStdWString(L""), QString::fromStdWString(L""));
	msg_box->SetCallback(nullptr);

	if (top_widget)
	{
		UiUtils::AlignWidgetOnCenter(msg_box, top_widget);
	}

	msg_box->show();
	msg_box->SetDelayClose(delay_time);
}

void ShowMsgBox(std::wstring msg, MsgboxCallback cb, std::wstring title /*= L""*/, std::wstring ok_text /*= L""*/, std::wstring cancel_text /*= L""*/, bool b_close_show,bool b_modal)
{
	auto top_widget = GetCurrentTopWidget();

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
		if (top_widget)
		{
			UiUtils::AlignWidgetOnCenter(msg_box, top_widget);
		}

		msg_box->show();
	}
}

void ShowMsgToast(std::wstring msg, int delay_time)
{
	QWidget* parent_widget = nullptr;
	auto top_widget = qApp->focusWidget();

	while (top_widget && (parent_widget = top_widget->parentWidget()))
	{
		top_widget = parent_widget;
	}

	SingleTonSToast::getInstance().setMessageVDuration(QString::fromStdWString(msg), delay_time * 1000, top_widget);
}

void ShowMsgBox_TitleHint(int width, int height, std::wstring msg, MsgboxCallback cb, std::wstring title /*= L""*/, std::wstring ok_text /*= L""*/, std::wstring cancel_text /*= L""*/, std::wstring cancel_text2 /*= L""*/, std::wstring HighLightTitle /*= L""*/)
{
	auto top_widget = GetCurrentTopWidget();

	MsgBox *msg_box = new MsgBox();
	msg_box->SetInfo(width,height,QString::fromStdWString(title), QString::fromStdWString(msg), QString::fromStdWString(ok_text), QString::fromStdWString(cancel_text),QString::fromStdWString(cancel_text2),QString::fromStdWString(HighLightTitle));
	msg_box->SetCallback(cb);

	if (top_widget)
	{
		UiUtils::AlignWidgetOnCenter(msg_box, top_widget);
	}

	msg_box->show();
}

void ShowMsgBox_TitleHintVer2(int width, int height, std::wstring msg, MsgboxCallback cb, std::wstring title /*= L""*/, std::wstring ok_text /*= L""*/, std::wstring cancel_text /*= L""*/, std::wstring HighLightTitle /*= L""*/)
{
	auto top_widget = GetCurrentTopWidget();

	MsgBox *msg_box = new MsgBox();
	msg_box->SetInfo(width, height, QString::fromStdWString(title), QString::fromStdWString(msg), QString::fromStdWString(ok_text), QString::fromStdWString(cancel_text), QString::fromStdWString(HighLightTitle));
	msg_box->SetCallback(cb);

	if (top_widget)
	{
		UiUtils::AlignWidgetOnCenter(msg_box, top_widget);
	}

	msg_box->show();
}

void ShowRenameMsgBox(std::wstring msg, MsgboxCallback cb /*= nullptr*/, std::wstring title /*= L""*/, std::wstring ok_text /*= L""*/, std::wstring cancel_text /*= L""*/, std::wstring other_text /*= L""*/)
{
	auto top_widget = GetCurrentTopWidget();

	MsgBox *msg_box = new MsgBox();
	msg_box->SetInfo(QString::fromStdWString(title), QString::fromStdWString(msg), QString::fromStdWString(ok_text), QString::fromStdWString(cancel_text), QString::fromStdWString(other_text));
	msg_box->SetCallback(cb);

	if (top_widget)
	{
		UiUtils::AlignWidgetOnCenter(msg_box, top_widget);
	}

	msg_box->show();
}
