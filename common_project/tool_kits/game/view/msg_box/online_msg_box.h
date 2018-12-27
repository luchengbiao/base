#ifndef __GAME_VIEW_MSG_BOX_ONLINE_MSG_BOX_H__
#define __GAME_VIEW_MSG_BOX_ONLINE_MSG_BOX_H__
#include <memory>
#include <QDialog>
#include <QPointer>
#include "game/macros.h"

namespace Ui { class OnlineMsgBox; }
class QTimer;

GAME_NAMESPACE_BEGIN

class UIOnlineMsgBox : public QDialog
{
	Q_OBJECT

	typedef std::unique_ptr<Ui::OnlineMsgBox> MyUiPtr;
	typedef QPointer<QTimer> QTimerPtr;

public:
	explicit UIOnlineMsgBox(QWidget* parent = nullptr);
	~UIOnlineMsgBox();

public:
	void			SetMessage(const QString& msg);
	void			SetTitle(const QString& title);
	void			DelayToClose(int milliseconds);

private:
	void			Init();

	void			UpdateCountDown();

private	slots:
	void			OnCloseBtnClicked();
	void			OnConfirmBtnClicked();
	void			OnTimeoutToClose();

private:
	MyUiPtr			ui_;
	QTimerPtr		timer_{ nullptr };
	int				milliseconds_remain_{ 0 };
};

GAME_NAMESPACE_END

#endif