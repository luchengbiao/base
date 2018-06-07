#pragma once
#include "paint_image.h"
#include "paint_widget.h"
#include <QtWebKitWidgets/QWebView>

class BaseBoard : public PaintImage
{
	Q_OBJECT
public:
	BaseBoard(PaintBaseInfo info, QWidget *parent = 0);
	~BaseBoard();
	virtual void SetVisibleEx(bool bvisible) override;

	virtual QImage GetImage() = 0;
	virtual void SetTeaPenColor(QColor color) = 0;


	virtual void SetStuPenColor(QColor color) = 0;
	virtual void SetTeaShape(Shape shape) = 0;
	virtual void SetStuShape(Shape shape) = 0;
	virtual void SetPenIcon(std::string color) = 0;
	virtual void ResizeBoard(int width, int height) = 0;
	virtual void OnStuDrawInfos(std::list<DrawOpInfo> info_list, bool b_paint = true) = 0;
	virtual void OnTeaDrawInfos(std::list<DrawOpInfo> info_list, bool b_paint = true) = 0;
	virtual void SetDrawCb(DrawOpCallback cb) = 0;
	virtual void SetAcceptEvent(bool bAccept) = 0;
	virtual bool IsModified() = 0;
	virtual int GetCoursewareType() = 0;
	virtual int GetPage() = 0;
	virtual QImage GetLocalDrawImage() = 0;

	// 给数码笔留的接口
	virtual void OnLButtonDown(QPoint pt) = 0;
	virtual void OnMouseMove(QPoint pt) = 0;
	virtual void OnLButtonUp(QPoint pt) = 0;
};