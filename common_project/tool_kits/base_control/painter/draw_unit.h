#pragma once
#include <vector>
#include <QColor>
#include <QPoint>
#include <QRect>

class QImage;

enum DrawUnitType
{
	DrawUnit_None = 0,
	DrawUnit_Pen,
	DrawUnit_Erase,
};

class DrawUnit
{
public:
	DrawUnit(const QPoint& point, const QRect& rc);
	virtual ~DrawUnit();
	void Render(QImage &image, bool is_continue = false);  // 绘制接口
	void SetPenWidth(int pen_width);
	void SetPenColor(const QColor& color);
	QPoint GetStartPoint();
	QRect GetRectValid();
	void SetRectValid(const QRect& rc_valid);
	QRect GetDrawRect();
	virtual bool SetEndPoint(const QPoint& point);
	virtual QPoint GetEndPoint();
	virtual void ResetDrawPos();
	virtual DrawUnitType GetType();

protected:
	virtual	void RenderSelf(QImage &image, bool is_continue) = 0;
	void MeasurePoint(double &x, double &y);	// 确保要绘制的点在截图区域范围内 
	QPointF TransformPoint(const QPointF& point);
	QPointF TransformRatio(const QPoint& point);
	void CalcDrawRect(const std::vector<QPointF>& vec_point, int radius);
	void CalcDrawRect(const QPointF& point, int radius);

protected:
	QPoint start_point_;
	QPoint end_point_;
	int pen_width_;		//线粗
	QColor pen_color_;	//颜色
	QRect rc_valid_;    //合法范围
	QRect draw_rect_;   // 当前绘制区域
};

//////////////////////////////////画笔////////////////////////////////////////

class DrawUnitPen : public DrawUnit
{
public:
	DrawUnitPen(const QPoint& point, const QRect& rc);
	virtual	bool SetEndPoint(const QPoint& point) override;
	virtual	QPoint GetEndPoint() override;
	virtual void ResetDrawPos() override;
	virtual DrawUnitType GetType() override;

protected:
	virtual	void RenderSelf(QImage &image, bool is_continue);

private:
	void DrawSingleLine(QImage &image, const QPointF &pt_start, const QPointF &pt_end);

private:
	std::vector<QPointF> m_vtPoints;
	int draw_pos_;	//绘制度（对于复杂的一步操作设置完成度，不需要每次都重新画）
};

//////////////////////////////////橡皮擦////////////////////////////////////////

class DrawUnitErase : public DrawUnit
{
public:
	DrawUnitErase(QPoint point, QRect rc);
	virtual	bool SetEndPoint(const QPoint& point) override;
	virtual	QPoint GetEndPoint() override;
	virtual void ResetDrawPos() override;
	virtual DrawUnitType GetType() override;

protected:
	virtual	void RenderSelf(QImage &image, bool is_continue);

private:
	void FillArea(QImage &image, const QPointF &pt_start, const QPointF &pt_end);
	void DrawEllipse(QImage &image, const QPointF &pt);

private:
	std::vector<QPointF> m_vtPoints;
	int draw_pos_;	//绘制度（对于复杂的一步操作设置完成度，不需要每次都重新画）
	int m_radius_;
};