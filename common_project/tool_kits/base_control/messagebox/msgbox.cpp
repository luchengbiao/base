#include "msgbox.h"
#include "ui_msgbox.h"
#include <QGraphicsDropShadowEffect>
#include "QWidget"
#include "qevent.h"
#include "QPainter"
#include "base\macros.h"
#include "qmath.h"
#include <QApplication>
#include <QDesktopWidget>

#define SHADOW_SIZE 10

MsgBox::MsgBox(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MsgBox)
{
    ui->setupUi(this);

	ret_ = MB_NO;
	
	//设置标题栏隐藏
	setWindowTitle(QString::fromLocal8Bit("小棋神"));
	setWindowIcon(QIcon(":/icon/images/icon/jyxb-logo.ico"));
	this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
	this->setAttribute(Qt::WA_DeleteOnClose);
	setAttribute(Qt::WA_TranslucentBackground);
	ui->btn_cancel->setCursor(Qt::PointingHandCursor);
	ui->btn_ok->setCursor(Qt::PointingHandCursor);
	ui->btn_close->setCursor(Qt::PointingHandCursor);

	QGraphicsDropShadowEffect *shadow_effect = new QGraphicsDropShadowEffect(this);
	shadow_effect->setOffset(0, 0);
	shadow_effect->setBlurRadius(16);
	shadow_effect->setColor(QColor(0, 0, 0, 255 * 0.36));
	ui->frame->setGraphicsEffect(shadow_effect);

//	ui->lab_msg->setFixedWidth(450);
	ui->lab_msg->setMaximumHeight(190);
	ui->lab_msg->setWordWrap(true);   //自动折行
//	ui->lab_msg->setMargin(4);        //边距调整
	ui->lab_msg->setText(QString::fromStdWString(L""));
	ui->btn_cancel->setFixedSize(76, 27);
	ui->btn_ok->setFixedSize(76, 27);
	ui->btn_close->setFixedSize(19, 15);
	ui->wid_title->setFixedHeight(40);
	ui->btn_tmp->setFixedSize(76, 27);
	ui->btn_tmp->setVisible(false);
	ui->frame_2->setVisible(false);
	ui->lb_top_title1->setVisible(false);
	ui->lb_top_title1->setFixedWidth(this->width());
	ui->lb_top_title1->setAlignment(Qt::AlignLeft);
	SetAllObjectName();

	QObject::connect(ui->btn_ok, SIGNAL(clicked()), this, SLOT(SlotOkOperate()));
	QObject::connect(ui->btn_cancel, SIGNAL(clicked()), this, SLOT(SlotCancelOperate()));
	QObject::connect(ui->btn_tmp, SIGNAL(clicked()), this, SLOT(SlotOtherOperate()));
	connect(ui->btn_close, SIGNAL(clicked()), this, SLOT(SlotCancelOperate()));
	//初始化定时器
	connect(&m_durationTimer_, &QTimer::timeout, this, &MsgBox::timeOver);

	SetWidthAndHeight(400, 185);
	// 此方方法可以解决居中显示的问题
	QRect curDesktopRc = QApplication::desktop()->screenGeometry(this);  //获取显示本窗口的屏幕区域
	move((curDesktopRc.width() - width()) / 2, (curDesktopRc.height() - height()) / 2); //屏幕居中显示
}

MsgBox::~MsgBox()
{
	SAFE_DELETE(ui);
}

void MsgBox::SetInfo(QString title_info, QString msg_info, QString ok_text, QString cancel_text, bool b_close_show)
{
	ui->lb_title->setText(title_info); 
	ui->lab_msg->setText(msg_info);
	
	if (!ok_text.isEmpty())
	{
		ui->btn_ok->setText(ok_text);
	}

	if (cancel_text.isEmpty())
	{
		ui->btn_cancel->setVisible(false);
	}
	else
	{
		ui->btn_cancel->setVisible(true);
		ui->btn_cancel->setText(cancel_text);
	}

	ui->btn_close->setVisible(b_close_show);
	ui->btn_ok->setFocus();
}

void MsgBox::SlotOkOperate()
{
	//if (msgbox_callback_)
	//{
	//	msgbox_callback_(MB_YES);
	//}
	ret_ = MB_YES;
	this->close();
}

void MsgBox::SlotCancelOperate()
{
	ret_ = MB_NO;
	this->close();
}

void MsgBox::SlotOtherOperate()
{
	//if (msgbox_callback_)
	//{
	//	msgbox_callback_(MB_OTHER);
	//}
	ret_ = MB_OTHER;
	this->close();
}

void MsgBox::SetDelayClose(int delay_time)
{
	m_durationTimer_.start(delay_time * 1000);
}

void MsgBox::SetCallback(MsgboxCallback cb)
{
	msgbox_callback_ = cb;
}

void MsgBox::SetOkButtonVisible(bool visible)
{
	ui->btn_ok->setVisible(visible);
}

void MsgBox::SetOkButtonText(const QString& text)
{
	ui->btn_ok->setText(text);
}

void MsgBox::SetCancelButtonVisible(bool visible)
{
	ui->btn_cancel->setVisible(visible);
}

void MsgBox::SetCancelButtonText(const QString& text)
{
	ui->btn_cancel->setText(text);
}

void MsgBox::SetTempButtonVisible(bool visible)
{
	ui->btn_tmp->setVisible(visible);
}

void MsgBox::SetTempButtonText(const QString& text)
{
	ui->btn_tmp->setText(text);
}

void MsgBox::timeOver()
{
	m_durationTimer_.stop();

	this->close();
}

void MsgBox::SetWidthAndHeight(int width, int height)
{
	this->setFixedSize(QSize(width, height));
}

void MsgBox::mousePressEvent(QMouseEvent *e) {
	// 记录鼠标按下时全局的位置和窗口左上角的位置
	m_pos_.mousePressedPosition = e->globalPos();
	m_pos_.windowPositionAsDrag = pos();
}

void MsgBox::mouseReleaseEvent(QMouseEvent *e) {
	Q_UNUSED(e)
		// 鼠标放开始设置鼠标按下的位置为 null，表示鼠标没有被按下
		m_pos_.mousePressedPosition = QPoint();
}

void MsgBox::mouseMoveEvent(QMouseEvent *e) {
	if (!m_pos_.mousePressedPosition.isNull()) {
		// 鼠标按下并且移动时，移动窗口, 相对于鼠标按下时的位置计算，是为了防止误差累积
		QPoint delta = e->globalPos() - m_pos_.mousePressedPosition;
		move(m_pos_.windowPositionAsDrag + delta);
	}
}

void MsgBox::paintEvent(QPaintEvent *e)
{
	SetWidthAndHeight(this->width(), this->height());
}

void MsgBox::closeEvent(QCloseEvent *e)
{
	if (msgbox_callback_)
	{
		if (!ui->btn_cancel->isVisible())
		{
			if (ret_ != MB_OTHER)
			{
				msgbox_callback_(MB_YES);
			}
			else
			{
				msgbox_callback_(MB_OTHER);
			}
		}	
		else
		{
			msgbox_callback_(ret_);
		}
	}

	QDialog::closeEvent(e);
}

void MsgBox::SetInfo(int width,int height,QString title_info, QString msg_info, QString ok_text, QString cancel_text, QString cancel_text_2, QString HighLignt_title)
{
	SetWidthAndHeight(width, height);
	QFont font("Microsoft YaHei",12,75);

	ui->lb_title->setText(title_info);
	ui->lb_top_title->setFont(font);
	ui->lb_top_title->setText(HighLignt_title);
	ui->lb_exclamation->setStyleSheet("QLabel#lb_exclamation{border-image:url(:/common/images/common/exclamation.png)}");
	ui->lb_exclamation->setFixedSize(QSize(20,20));
	ui->lab_msg->setText(msg_info);

	ui->frame_2->deleteLater();
	ui->frame_2 = nullptr;

	if (!ok_text.isEmpty())
	{
		ui->btn_ok->setText(ok_text);
	}

	if (cancel_text.isEmpty())
	{
		ui->btn_cancel->setVisible(false);
	}
	else
	{
		ui->btn_cancel->setVisible(true);
		ui->btn_cancel->setText(cancel_text);
	}
	ui->btn_ok->setFocus();
}

void MsgBox::SetInfo(int width, int height, QString title_info, QString msg_info, QString ok_text, QString cancel_text, QString HighLignt_title, bool b_titlecenter)
{
	ui->lb_title->setText(title_info);
	SetWidthAndHeight(width, height);
	QFont font("Microsoft YaHei", 12, 75);
	ui->lb_exclamation->hide();
	ui->lab_msg->setText(msg_info);
	if (!b_titlecenter)
	{
		ui->frame_3->setVisible(false);
		ui->frame_2->setVisible(true);
		ui->horizontalLayout_6->setContentsMargins(23, 10, 23, 10);
		ui->lb_top_title1->setVisible(true);
		ui->lb_top_title1->setFont(font);
		ui->lb_top_title1->setText(HighLignt_title);
	}
	else
	{
		ui->frame_2->setVisible(false);
		ui->lb_top_title1->setVisible(false);
		ui->lb_top_title->setFont(font);
		ui->lb_top_title->setText(HighLignt_title);
	}

	if (!ok_text.isEmpty())
	{
		ui->btn_ok->setText(ok_text);
	}

	if (cancel_text.isEmpty())
	{
		ui->btn_cancel->setVisible(false);
	}
	else
	{
		ui->btn_cancel->setVisible(true);
		ui->btn_cancel->setText(cancel_text);
	}
	ui->btn_ok->setFocus();

}

void MsgBox::SetInfo(QString title_info, QString msg_info, QString ok_text, QString cancel_text, QString other_text)
{
	ui->lb_title->setText(title_info);
	ui->lab_msg->setText(msg_info);

	if (!ok_text.isEmpty())
	{
		ui->btn_ok->setText(ok_text);
	}

	if (cancel_text.isEmpty())
	{
		ui->btn_cancel->setVisible(false);
	}
	else
	{
		ui->btn_cancel->setVisible(true);
		ui->btn_cancel->setText(cancel_text);
	}

	if (other_text.isEmpty())
	{
		ui->btn_tmp->setVisible(false);
	}
	else
	{
		ui->btn_tmp->setVisible(true);
		ui->btn_tmp->setText(other_text);
	}

	ui->btn_close->setVisible(true);
	ui->btn_ok->setFocus();
}

void MsgBox::SetAllObjectName()
{
	ui->frame->setObjectName("fr_1");
	ui->wid_title->setObjectName("wid_title");
	ui->lb_title->setObjectName("lb_text_3");
	ui->btn_close->setObjectName("btn_close_type_401");
	ui->btn_ok->setObjectName("btn_ok");
	ui->btn_tmp->setObjectName("btn_ok");
	ui->btn_cancel->setObjectName("btn_cancel");
	ui->lab_msg->setObjectName("lb_text_33");
}