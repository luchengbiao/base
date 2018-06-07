#include "draw_unit.h"
#include "QPainter"
#include "QPen"
#include "qmath.h"
#include "QDebug"

#define Max(a,b)    (((a) > (b)) ? (a) : (b))
#define Min(a,b)    (((a) < (b)) ? (a) : (b))

DrawUnit::DrawUnit(QPoint point, QRect rc) : pen_width_(knLineWidth), pen_color_(Qt::black)
{
	start_point_ = point;
	end_point_ = point;

	if (rc.width() > 0 && rc.height() > 0)
	{
		rc_valid_ = rc;
	}
	else
	{
		rc_valid_ = QRect(0, 0, 30, 40);
	}
	draw_rect_ = rc_valid_;
}

DrawUnit::~DrawUnit()
{

}

void DrawUnit::Render(QImage &image, bool is_continue /*= false*/)
{
	if (image.width() > 0 && image.height() > 0) {
		SetRectValid(QRect(0, 0, image.width(), image.height()));
	}
	RenderSelf(image, is_continue);
}

void DrawUnit::SetPenWidth(int pen_width)
{
	pen_width_ = pen_width;
}

void DrawUnit::SetPenColor(QColor color)
{
	pen_color_ = color;
}

QPoint DrawUnit::GetStartPoint()
{
	return start_point_;
}

QRect DrawUnit::GetRectValid()
{
	return rc_valid_;
}

void DrawUnit::SetRectValid(const QRect& rc_valid)
{
	rc_valid_ = rc_valid;
}

bool DrawUnit::SetEndPoint(QPoint point)
{
	end_point_ = point;
	return true;
}

QPoint DrawUnit::GetEndPoint()
{
	return end_point_;
}

void DrawUnit::ResetDrawPos()
{

}

DrawUnitType DrawUnit::GetType()
{
	return DrawUnit_None;
}

QRect DrawUnit::GetDrawRect()
{
	return draw_rect_;
}

void DrawUnit::MeasurePoint(double &x, double &y)
{
	x = (x < rc_valid_.left() ? rc_valid_.left() : x);
	x = (x > rc_valid_.right() ? rc_valid_.right() : x);
	y = (y < rc_valid_.top() ? rc_valid_.top() : y);
	y = (y > rc_valid_.bottom() ? rc_valid_.bottom() : y);
}

QPointF DrawUnit::TransformPoint(QPointF point)
{
	return QPointF(point.x() * rc_valid_.width(), point.y() * rc_valid_.height());
}

QPointF DrawUnit::TransformRatio(QPoint point)
{
	return QPointF(point.x() * 1.0 / rc_valid_.width(), point.y() * 1.0 / rc_valid_.height());
}

void DrawUnit::CalcDrawRect(std::vector<QPointF> vec_point, int radius)
{
	int top = 9999999, left = 9999999, bottom = 0, right = 0;
	if (draw_rect_.width() > 0 || draw_rect_.height() > 0)
	{
		top = draw_rect_.top();
		left = draw_rect_.left();
		bottom = draw_rect_.bottom();
		right = draw_rect_.right();
	}
	
	for (int i = 0; i < vec_point.size(); i++)
	{
		top = Min((int)vec_point[i].y() - radius, top);
		left = Min((int)vec_point[i].x() - radius, left);
		bottom = Max((int)vec_point[i].y() + radius, bottom);
		right = Max((int)vec_point[i].x() + radius, right);
	}
	left = Max(left, 0);
	top = Max(top, 0);
	draw_rect_ = QRect(left, top, right - left, bottom - top);
}

void DrawUnit::CalcDrawRect(QPointF point, int radius)
{
	int top = 9999999, left = 9999999, bottom = 0, right = 0;
	if (draw_rect_.width() > 0 || draw_rect_.height() > 0)
	{
		top = draw_rect_.top();
		left = draw_rect_.left();
		bottom = draw_rect_.bottom();
		right = draw_rect_.right();
	}

	top = Min((int)point.y() - radius, top);
	left = Min((int)point.x() - radius, left);
	bottom = Max((int)point.y() + radius, bottom);
	right = Max((int)point.x() + radius, right);

	left = Max(left, 0);
	top = Max(top, 0);
	draw_rect_ = QRect(left, top, right - left, bottom - top);
}

//////////////////////////////////»­±Ê////////////////////////////////////////

DrawUnitPen::DrawUnitPen(QPoint point, QRect rc) : DrawUnit(point, rc), draw_pos_(0)
{
	QPointF ratio = TransformRatio(point);
	if (rc_valid_.contains(point))
	{
		m_vtPoints.push_back(ratio);
	}
}

bool DrawUnitPen::SetEndPoint(QPoint point)
{
	if (rc_valid_.contains(point))
	{
		if (m_vtPoints.size() > 0)
		{
			QPointF end_pt = m_vtPoints.back();
			if (fabs(point.x() - end_pt.x() * rc_valid_.width()) >= 1.0
				|| fabs(point.y() - end_pt.y() * rc_valid_.height()) >= 1.0)
			{
				QPointF ratio = TransformRatio(point);
				m_vtPoints.push_back(ratio);
				return true;
			}
		}
		else
		{
			QPointF ratio = TransformRatio(point);
			m_vtPoints.push_back(ratio);
			return true;
		}
	}
	return false;
}

QPoint DrawUnitPen::GetEndPoint()
{
	QPoint pt(-1, -1);
	if (m_vtPoints.size() > 0)
	{
		QPointF pt1 = TransformPoint(m_vtPoints[m_vtPoints.size() - 1]);
		pt = pt1.toPoint();
	}
	return pt;
}

void DrawUnitPen::ResetDrawPos()
{
	draw_pos_ = 0;
}

DrawUnitType DrawUnitPen::GetType()
{
	return DrawUnit_Pen;
}

void DrawUnitPen::RenderSelf(QImage &image, bool is_continue)
{
	draw_rect_ = QRect(0, 0, 0, 0);
	int draw_pos = is_continue ? draw_pos_ : 0;
	draw_pos = draw_pos > 0 ? draw_pos : 1;

	std::vector<QPointF> draw_points;
	for (size_t i = draw_pos; i < m_vtPoints.size(); i++)
	{
		QPointF pt1 = TransformPoint(m_vtPoints[i - 1]);
		QPointF pt2 = TransformPoint(m_vtPoints[i]);
		DrawSingleLine(image, pt1, pt2);
		draw_points.push_back(pt1);
	}
	if (m_vtPoints.size() > 0)
	{
		int index = m_vtPoints.size() - 1;
		QPointF pt = TransformPoint(m_vtPoints[index]);
		draw_points.push_back(pt);
	}
	draw_pos_ = m_vtPoints.size();
	CalcDrawRect(draw_points, 5);
}

void DrawUnitPen::DrawSingleLine(QImage &image, const QPointF &pt_start, const QPointF &pt_end)
{
	QPainter painter(&image);
	QPen pen = QPen();
	pen.setColor(pen_color_);
	pen.setStyle(Qt::SolidLine);
	pen.setWidthF(knLineWidth);
	pen.setJoinStyle(Qt::RoundJoin);
	pen.setCapStyle(Qt::RoundCap);
	painter.setPen(pen);
	painter.setRenderHint(QPainter::Antialiasing, true);

	double start_x = pt_start.x(), start_y = pt_start.y();
	double end_x = pt_end.x(), end_y = pt_end.y();

	MeasurePoint(start_x, start_y);
	MeasurePoint(end_x, end_y);

	painter.drawLine(QPointF(start_x, start_y), QPointF(end_x, end_y));
}

//////////////////////////////////ÏðÆ¤²Á////////////////////////////////////////

DrawUnitErase::DrawUnitErase(QPoint point, QRect rc) : DrawUnit(point, rc), draw_pos_(0)
{
	QPointF ratio = TransformRatio(point);
	if (rc_valid_.contains(point))
	{
		m_vtPoints.push_back(ratio);
	}
}

bool DrawUnitErase::SetEndPoint(QPoint point)
{
	if (rc_valid_.contains(point))
	{
		if (m_vtPoints.size() > 0)
		{
			QPointF end_pt = m_vtPoints.back();
			if (fabs(point.x() - end_pt.x() * rc_valid_.width()) >= 1.0
				|| fabs(point.y() - end_pt.y() * rc_valid_.height()) >= 1.0)
			{
				QPointF ratio = TransformRatio(point);
				m_vtPoints.push_back(ratio);
				return true;
			}
		}
		else
		{
			QPointF ratio = TransformRatio(point);
			m_vtPoints.push_back(ratio);
			return true;
		}
	}
	return false;
}

void DrawUnitErase::ResetDrawPos()
{
	draw_pos_ = 0;
}

DrawUnitType DrawUnitErase::GetType()
{
	return DrawUnit_Erase;
}

void DrawUnitErase::RenderSelf(QImage &image, bool is_continue)
{
	draw_rect_ = QRect(0, 0, 0, 0);
	int draw_pos = is_continue ? draw_pos_ : 0;
	draw_pos = draw_pos > 0 ? draw_pos : 1;

	std::vector<QPointF> draw_points;
	for (size_t i = draw_pos; i < m_vtPoints.size(); i++)
	{
		QPointF pt1 = TransformPoint(m_vtPoints[i - 1]);
		QPointF pt2 = TransformPoint(m_vtPoints[i]);
		FillArea(image, pt1, pt2);
		draw_points.push_back(pt1);
	}

	if (m_vtPoints.size() > 0)
	{
		int index = m_vtPoints.size() - 1;
		QPointF pt = TransformPoint(m_vtPoints[index]);
		draw_points.push_back(pt);
	}
	draw_pos_ = m_vtPoints.size();
	CalcDrawRect(draw_points, m_radius_ + 5);
}

void DrawUnitErase::FillArea(QImage &image, const QPointF &pt_start, const QPointF &pt_end)
{
	double start_x = pt_start.x(), start_y = pt_start.y();
	double end_x = pt_end.x(), end_y = pt_end.y();

	MeasurePoint(start_x, start_y);
	MeasurePoint(end_x, end_y);

	double total_disc = pow(fabs(pt_end.x() - pt_start.x()), 2) + pow(fabs(pt_end.y() - pt_start.y()), 2);
	if (total_disc <=0)
	{
		return;
	}
	m_radius_ = knEraseRadiusRatio * rc_valid_.height();
	double ratio = (m_radius_ * 1) / sqrt(total_disc);
	double sum = ratio;
	QPainter painter(&image);
	painter.setPen(QPen(QColor(255, 255, 255, 0), 1, Qt::SolidLine));//ÉèÖÃ»­Ë¢ÐÎÊ½ 
	painter.setBrush(QBrush(QColor(255, 255, 255, 0), Qt::SolidPattern));//ÉèÖÃ»­Ë¢ÐÎÊ½
	painter.setCompositionMode(QPainter::CompositionMode_Clear);
	painter.drawEllipse(QRect(pt_start.x() - m_radius_, pt_start.y() - m_radius_, m_radius_ * 2, m_radius_ * 2));
	CalcDrawRect(pt_start, m_radius_ + 5);
	while (sum < 1)
	{
		double xt = start_x * sum + end_x * (1.0 - sum);
		double yt = start_y * sum + end_y * (1.0 - sum);
		
		QPointF pt(xt,yt);
		painter.drawEllipse(QRect(pt.x() - m_radius_, pt.y() - m_radius_, m_radius_ * 2, m_radius_ * 2));
		CalcDrawRect(pt, m_radius_ + 5);
		sum += ratio;
	}
	painter.drawEllipse(QRect(pt_end.x() - m_radius_, pt_end.y() - m_radius_, m_radius_ * 2, m_radius_ * 2));
	CalcDrawRect(pt_end, m_radius_ + 5);
}

void DrawUnitErase::DrawEllipse(QImage &image, const QPointF &pt)
{
	int width = image.width();
	int height = image.height();

	for (int i = -m_radius_; i <= m_radius_; i++)
	{
		for (int j = -m_radius_; j <= m_radius_; j++)
		{
			int t_x = pt.x() + i;
			int t_y = pt.y() + j;
			if (t_x >= 0 && t_y >= 0 && t_x < width &&t_y < height)
			{
				int distance = sqrt(pow(fabs(t_y - pt.y()), 2) + pow(fabs(t_x - pt.x()), 2));
				if (distance <= m_radius_)
				{
					image.setPixel(t_x, t_y, 0);
				}
			}
		}
	}
}

QPoint DrawUnitErase::GetEndPoint()
{
	QPoint pt(-1, -1);
	if (m_vtPoints.size() > 0)
	{
		QPointF pt1 = TransformPoint(m_vtPoints[m_vtPoints.size() - 1]);
		pt = pt1.toPoint();
	}
	return pt;
}