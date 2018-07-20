#include "basewindow.h"
#include <QCoreApplication>
#include <QEvent>
#include <QMessageBox>
#include <QCloseEvent>
#include "..\windows_manager\windows_manage.h"
#include "windows.h"

BaseWindow::BaseWindow(QWidget *parent /*= 0*/) :QWidget(parent)
{
	this->setWindowFlags(Qt::FramelessWindowHint); //隐藏主界面的标题栏
	this->setAttribute(Qt::WA_DeleteOnClose);
	m_pressed_ = false;
	trayIcon_ = new QSystemTrayIcon(this);
	QIcon icon = QIcon(":/icon/images/icon/xqs_logo.ico");
	trayIcon_->setIcon(icon);
	setWindowIcon(icon);	
}

BaseWindow::~BaseWindow()
{
	//delete trayIcon_;
}

bool BaseWindow::RegisterWnd()
{
	std::wstring wnd_class_name = GetWindowClassName();
	std::wstring wnd_id = GetWindowId();
	if (wnd_class_name == L"MainWindow")
	{
		trayIcon_->show();
		createActions();
		createTrayIcon();
	}
	if (!WindowsManager::GetInstance()->RegisterWindow(wnd_class_name, wnd_id, this))
	{
		return false;
	}
	return true;
}

void BaseWindow::Create()
{
	RegisterWnd();
	InitWindow();
//	QWidget::installEventFilter(this);//为这个窗口安装过滤器  
}

void BaseWindow::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_pos_ = event->pos();
		if (m_pos_.y() <= 65)
		{
			//m_pressed_ = true;
			if (ReleaseCapture())
				SendMessage(HWND(this->winId()), WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);
			event->ignore();
		}
	}
}
void BaseWindow::mouseMoveEvent(QMouseEvent *event)
{
	//if (m_pressed_)
	//{
	//	move(event->pos() - m_pos_ + this->pos());
	//}
}

void BaseWindow::mouseReleaseEvent(QMouseEvent *event)
{
	//Q_UNUSED(event);            // avoid the warnning that 'event' is unused while building the project
	
	//m_pressed_ = false;
}

void BaseWindow::createActions()
{
	/*QString q_min = QString::fromLocal8Bit("最小化");
	minimizeAction_ = new QAction(q_min, this);
	connect(minimizeAction_, &QAction::triggered, this, &QWidget::hide);

	maximizeAction_ = new QAction(tr("Ma&ximize"), this);
	connect(maximizeAction_, &QAction::triggered, this, &QWidget::showMaximized);

	restoreAction_ = new QAction(tr("&Restore"), this);
	connect(restoreAction_, &QAction::triggered, this, &QWidget::showNormal);

	quitAction_ = new QAction(tr("&Quit"), this);
	connect(quitAction_, &QAction::triggered, qApp, &QCoreApplication::quit);*/
}

void BaseWindow::createTrayIcon()
{
	/*trayIconMenu_ = new QMenu(this);
	trayIconMenu_->addAction(minimizeAction_);
	trayIconMenu_->addAction(maximizeAction_);
	trayIconMenu_->addAction(restoreAction_);
	trayIconMenu_->addSeparator();
	trayIconMenu_->addAction(quitAction_);

	trayIcon_->setContextMenu(trayIconMenu_);*/
}

void BaseWindow::closeEvent(QCloseEvent *event)
{
#ifdef Q_OS_OSX
	if (!event->spontaneous() || !isVisible()) {
		return;
	}
#endif
	if (trayIcon_->isVisible() && GetWindowClassName() == L"MainWindow") {
		/*QMessageBox::information(this, tr("login"),
			tr("The program will keep running in the "
			"system tray. To terminate the program, "
			"choose <b>Quit</b> in the context menu "
			"of the system tray entry."));*/
		hide();
		event->ignore();
	}
	else
	{
		UnRegisterWnd();
	}
}

void BaseWindow::OnShowTrayIcon()
{
	trayIcon_->show();
}

void BaseWindow::UnRegisterWnd()
{
	std::wstring wnd_class_name = GetWindowClassName();
	std::wstring wnd_id = GetWindowId();
	WindowsManager::GetInstance()->UnRegisterWindow(wnd_class_name, wnd_id, this);
}
