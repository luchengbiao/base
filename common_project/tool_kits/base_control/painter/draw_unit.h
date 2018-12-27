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
	void Render(QImage &image, bool is_continue = false);  // ���ƽӿ�
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
	void MeasurePoint(double &x, double &y);	// ȷ��Ҫ���Ƶĵ��ڽ�ͼ����Χ�� 
	QPointF TransformPoint(const QPointF& point);
	QPointF TransformRatio(const QPoint& point);
	void CalcDrawRect(const std::vector<QPointF>& vec_point, int radius);
	void CalcDrawRect(const QPointF& point, int radius);

protected:
	QPoint start_point_;
	QPoint end_point_;
	int pen_width_;		//�ߴ�
	QColor pen_color_;	//��ɫ
	QRect rc_valid_;    //�Ϸ���Χ
	QRect draw_rect_;   // ��ǰ��������
};

//////////////////////////////////����////////////////////////////////////////

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
	int draw_pos_;	//���ƶȣ����ڸ��ӵ�һ������������ɶȣ�����Ҫÿ�ζ����»���
};

//////////////////////////////////��Ƥ��////////////////////////////////////////

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
	int draw_pos_;	//���ƶȣ����ڸ��ӵ�һ������������ɶȣ�����Ҫÿ�ζ����»���
	int m_radius_;
};