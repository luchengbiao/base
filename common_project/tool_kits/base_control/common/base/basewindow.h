#pragma once
#include <QWidget>
#include <QSystemTrayIcon>
#include <QAction>
#include <QMenu>

class BaseWindow : public QWidget
{
	Q_OBJECT
public:
	BaseWindow(QWidget *parent = 0);
	~BaseWindow();

	virtual void Create();

	virtual std::wstring GetWindowClassName(void) const = 0;
	virtual std::wstring GetWindowId(void) const = 0;
	virtual void InitWindow() = 0;

	virtual void createActions();
	virtual void createTrayIcon();

	void OnShowTrayIcon();

protected:
	virtual void mousePressEvent(QMouseEvent *event);    // 鼠标按下
	virtual void mouseMoveEvent(QMouseEvent *event);        // 移动
	virtual void mouseReleaseEvent(QMouseEvent *event);     // 鼠标释放
	void UnRegisterWnd();
	void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private:
	bool RegisterWnd();

public:
	QSystemTrayIcon *trayIcon_;
	QMenu *trayIconMenu_;

private:
	bool m_pressed_;       // 判断鼠标左键是否按下
	QPoint m_pos_;         // 鼠标相对于窗口的位置，不是相对屏幕的位置
};

