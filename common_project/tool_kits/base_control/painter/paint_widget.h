#ifndef PAINTAREA_H
#define PAINTAREA_H

#include <QWidget>
#include<QMouseEvent>
#include<QPoint>
#include<stdio.h>
#include <QTextStream>
#include "draw_unit.h"
#include <functional>
#include "paint_data.h"
#include "QLabel"

typedef std::function<void(DrawOpInfo info)> DrawOpCallback;

struct DrawRecord
{
	int m_sign_;
	DrawUnit* m_draw_unit_;

	DrawRecord() : m_sign_(DrawRecordSign_None), m_draw_unit_(NULL)
	{

	}
};



class PaintWidget : public QWidget
{
public:
	PaintWidget(PaintBaseInfo info, QWidget *parent = 0);
	~PaintWidget();
	
	bool IsModified();
	QImage GetImage();
	QImage GetImageByForce();
	void SetPenStyle(Qt::PenStyle);
	void SetPenWidth(int w);
	void SetTeaPenColor(QColor color);
	void SetStuPenColor(QColor color);
	void SetTeaShape(Shape shape);
	void SetStuShape(Shape shape);
	void SetPenIcon(std::string color);
	void ResizeDrawBoard(int width, int height);
	void SetVisibleEx(bool bvisible);
	void OnStuDrawInfos(const std::list<DrawOpInfo>& info_list, bool b_paint = true);
	void OnTeaDrawInfos(const std::list<DrawOpInfo>& info_list, bool b_paint = true);
	void SetDrawCb(DrawOpCallback cb);
	void SetAcceptEvent(bool bAccept);
	QRect GetDrawRect();
	int GetCoursewareType();
	int GetPage();
	bool GetDrawUnits(std::list<DrawRecord>& list_draw_units);
	void ClearDrawedUnits();

	// 给数码笔留的接口
	void OnLButtonDown(QPoint pt);
	void OnMouseMove(QPoint pt);
	void OnLButtonUp(QPoint pt);

protected:
	void paintEvent(QPaintEvent *);
	bool eventFilter(QObject *watched, QEvent *event);//事件过滤器  

private:
	void PaintContent(QImage &Cimage, bool redraw = false);
	void OnDrawUnitStart(const QPoint &point);
	void CreateStuDrawUnit(const QPoint& point);
	void CreateTeaDrawUnit(const QPoint& point);
	void OnDrawUnitProcess(const QPoint &point);
	void DoDrawUnitProcess(const QPoint &point);

	void OnDrawUnitEnd(const QPoint &point);
	void SubmitDrawUnit();
	void ReleaseAllDrawUnits();
	bool UndoStuDraw();
	bool UndoTeaDraw();
	bool ClearStuDraw();
	bool ClearTeaDraw();
	void MousePress(QMouseEvent *);
	void MouseMove(QMouseEvent *);
	void MouseRelease(QMouseEvent *);
	void MouseLeave(QMouseEvent *);
	void MovePenIcon(QPoint pt);
	void ReplaceEraseColorPixel(bool redraw);
	void CalcDrawRect(QRect rect, bool& bfirst);
	void UpdateDrawList();

private:
	Shape tea_shape_;
	Shape stu_shape_;
	QSize image_size_;
	QImage image_;
	bool b_accept_event_;
	DrawOpCallback draw_op_cb_;
	int m_type_;
	int page_;
	bool b_modified_;
	QColor teaPenColor_;
	QColor stuPenColor_;
	Qt::PenStyle penStyle_;
	QPoint pen_point_;
	QImage pen_icon_;
	DrawUnit* current_stu_draw_unit_;         // 当前学生正在绘制的单元
	DrawUnit* current_tea_draw_unit_;         // 当前学生正在绘制的单元
	std::list<DrawRecord> list_draw_units_;   // 已绘制列表
	std::vector<DrawUnit*> vec_stu_waiting_draw_units_;	// 当前学生绘制单元列表等待列表
	std::vector<DrawUnit*> vec_tea_waiting_draw_units_;	// 当前老师绘制单元列表等待列表
	std::map<std::string, std::string> map_pen_icon_;
	QRect draw_rect_;
	bool b_stu_draw_;
};

#endif // PAINTAREA_H