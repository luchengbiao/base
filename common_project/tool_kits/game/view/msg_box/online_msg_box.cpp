#include "online_msg_box.h"
#include <QTimer>
#include "common/uiutils/ui_utils.h"
#include "game/GeneratedFiles/ui_online_msg_box.h"

GAME_NAMESPACE_BEGIN

UIOnlineMsgBox::UIOnlineMsgBox(QWidget* parent)
: QDialog(parent)
, ui_(new Ui::OnlineMsgBox)
{
	ui_->setupUi(this);

	this->Init();
}

UIOnlineMsgBox::~UIOnlineMsgBox() = default;

void UIOnlineMsgBox::Init()
{
	this->setWindowFlags(Qt::FramelessWindowHint);
	this->setAttribute(Qt::WA_DeleteOnClose);

	this->setFixedSize(349, 212);
	ui_->btn_close->setFixedSize(15, 17);
	ui_->btn_confirm->setFixedSize(80, 26);

	this->setObjectName("game_wdt_online_msg_box");
	ui_->lb_title->setObjectName("game_lb_13_000000");
	ui_->lb_message->setObjectName("game_lb_13_000000");
	ui_->lb_countdown->setObjectName("game_lb_13_000000");
	ui_->btn_close->setObjectName("game_btn_close_online_msg_box");
	ui_->btn_confirm->setObjectName("game_btn_ok_online_msg_box");

	ui_->lb_countdown->hide();
	
	connect(ui_->btn_close, SIGNAL(clicked()), this, SLOT(OnCloseBtnClicked()));
	connect(ui_->btn_confirm, SIGNAL(clicked()), this, SLOT(OnConfirmBtnClicked()));
}

void UIOnlineMsgBox::OnCloseBtnClicked()
{
	this->close();
}

void UIOnlineMsgBox::OnConfirmBtnClicked()
{
	this->close();
}

void UIOnlineMsgBox::OnTimeoutToClose()
{
	milliseconds_remain_ -= timer_->interval();
	this->UpdateCountDown();

	if (milliseconds_remain_ <= 0)
	{
		this->close();
	}
	else
	{
		timer_->setInterval(std::min(milliseconds_remain_, 500));
		timer_->start();
	}
}

void UIOnlineMsgBox::SetMessage(const QString& msg)
{
	ui_->lb_message->setText(msg);
}

void UIOnlineMsgBox::SetTitle(const QString& title)
{
	ui_->lb_title->setText(title);
}

void UIOnlineMsgBox::DelayToClose(int milliseconds)
{
	if (timer_.isNull())
	{
		timer_ = new QTimer(this);

		connect(timer_, &QTimer::timeout, this, &UIOnlineMsgBox::OnTimeoutToClose);
	}
	else
	{
		timer_->stop();
	}

	if (milliseconds > 0)
	{
		milliseconds_remain_ = milliseconds;

		this->UpdateCountDown();
		ui_->lb_countdown->show();
		timer_->setInterval(std::min(milliseconds_remain_, 500));
		timer_->start();
	}
	else
	{
		ui_->lb_countdown->hide();
	}
}

void UIOnlineMsgBox::UpdateCountDown()
{
	ui_->lb_countdown->setText(QString::number(milliseconds_remain_ / 1000));
}

GAME_NAMESPACE_END