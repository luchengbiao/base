#pragma once
#include "base_board.h"

class DrawBoard : public BaseBoard
{
	Q_OBJECT
public:
	DrawBoard(PaintBaseInfo info, QWidget *parent = 0);
	~DrawBoard();

	virtual QImage GetImage() override;
	virtual QImage GetImage(bool withBackgroudImage);
	virtual void SetVisibleEx(bool bvisible) override;
	virtual void SetTeaPenColor(QColor color) override;
	virtual void SetStuPenColor(QColor color) override;
	virtual void SetTeaShape(Shape shape) override;
	virtual void SetStuShape(Shape shape) override;
	virtual void SetPenIcon(std::string color) override;
	virtual void ResizeBoard(int width, int height) override;
	virtual void OnStuDrawInfos(std::list<DrawOpInfo> info_list, bool b_paint = true) override;
	virtual void OnTeaDrawInfos(std::list<DrawOpInfo> info_list, bool b_paint = true) override;
	virtual void SetDrawCb(DrawOpCallback cb) override;
	virtual void SetAcceptEvent(bool bAccept) override;
	virtual bool IsModified() override;
	virtual int GetCoursewareType() override;
	virtual int GetPage() override;
	virtual QImage GetLocalDrawImage() override;
	

	// 给数码笔留的接口
	virtual void OnLButtonDown(QPoint pt) override;
	virtual void OnMouseMove(QPoint pt) override;
	virtual void OnLButtonUp(QPoint pt) override;

	// 个别需求清除所有笔记，无法用撤销恢复
	void ClearDrawedUnits();

private:
	PaintWidget* draw_board_;
	bool b_visible_;
};