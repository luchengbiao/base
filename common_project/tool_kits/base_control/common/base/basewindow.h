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
	virtual void mousePressEvent(QMouseEvent *event);    // ��갴��
	virtual void mouseMoveEvent(QMouseEvent *event);        // �ƶ�
	virtual void mouseReleaseEvent(QMouseEvent *event);     // ����ͷ�
	void UnRegisterWnd();
	void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private:
	bool RegisterWnd();

public:
	QSystemTrayIcon *trayIcon_;
	QMenu *trayIconMenu_;

private:
	bool m_pressed_;       // �ж��������Ƿ���
	QPoint m_pos_;         // �������ڴ��ڵ�λ�ã����������Ļ��λ��
};

