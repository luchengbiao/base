#ifndef MSGBOX_H
#define MSGBOX_H

#include <QDialog>
#include <QPushButton>
#include "QLabel"
#include "QCheckBox"
#include "QTimer"
#include "msghint.h"

namespace Ui {
class MsgBox;
}


struct MsgBoxPrivate {
	QPoint mousePressedPosition; // 鼠标按下时的坐标
	QPoint windowPositionAsDrag; // 鼠标按小时窗口左上角的坐标
};

class MsgBox : public QDialog
{
    Q_OBJECT

public:
	explicit MsgBox(QWidget *parent = 0);
    ~MsgBox();
	void MsgBox::SetInfo(QString title_info, QString msg_info, QString ok_text, QString cancel_text,bool b_close_show=true);
	void MsgBox::SetInfo(QString title_info, QString msg_info, QString ok_text, QString cancel_text, QString other_text);
	void MsgBox::SetInfo(int width,int height,QString title_info,QString msg_info,QString ok_text,QString cancel_text,QString cancel_text_2,QString HighLignt_title);
	void SetInfo(int width, int height, QString title_info, QString msg_info, QString ok_text, QString cancel_text, QString HighLignt_title, bool b_titlecenter=true);
	void SetDelayClose(int delay_time);
	void SetCallback(MsgboxCallback cb);
	

protected:
	void mousePressEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
	void mouseReleaseEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
	void mouseMoveEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
	void paintEvent(QPaintEvent *e) Q_DECL_OVERRIDE;
	void closeEvent(QCloseEvent *e) Q_DECL_OVERRIDE;

public slots:
	void SlotOkOperate();
	void SlotCancelOperate();
	void SlotOtherOperate();
	void timeOver();  //定时器超时响应函数

private:
	void SetWidthAndHeight(int width,int height);
	void SetAllObjectName();

private:
    Ui::MsgBox *ui;
	MsgBoxPrivate m_pos_;
	QTimer m_durationTimer_;
	MsgboxCallback msgbox_callback_;
	MsgBoxRet ret_;
};

#endif // MSGBOX_H
