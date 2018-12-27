#include "arrow_line.h"
#include <QColor>
#include <QPainter>
#include <QPaintEvent>
#include <QPen>
#include <QtMath>

GAME_NAMESPACE_BEGIN

UIArrowLine::UIArrowLine(QWidget* parent)
: QWidget(parent)
{}

UIArrowLine::~UIArrowLine() = default;

void UIArrowLine::SetStartAndEndPosition(const QPoint& pos_start_global, const QPoint& pos_end_global)
{
	this->InnerSetStartPosition(pos_start_global);
	this->InnerSetEndPosition(pos_end_global);

	this->CalcArrowWingPositions();

	this->update();
}

void UIArrowLine::SetStartPosition(const QPoint& pos_global)
{
	this->InnerSetStartPosition(pos_global);

	this->CalcArrowWingPositions();

	this->update();
}

void UIArrowLine::SetEndPosition(const QPoint& pos_global)
{
	this->InnerSetEndPosition(pos_global);

	this->CalcArrowWingPositions();

	this->update();
}

void UIArrowLine::InnerSetStartPosition(const QPoint& pos_global)
{
	if (!pos_start_) { pos_start_.reset(new QPoint); }

	*pos_start_ = this->mapFromGlobal(pos_global);
}

void UIArrowLine::InnerSetEndPosition(const QPoint& pos_global)
{
	if (!pos_end_) { pos_end_.reset(new QPoint); }

	*pos_end_ = this->mapFromGlobal(pos_global);
}

void UIArrowLine::SetColor(const QColor& color)
{
	if (color_ != color)
	{
		color_ = color;

		this->update();
	}
}

void UIArrowLine::SetLineWidth(int line_width)
{
	if (line_width_ != line_width)
	{
		line_width_ = line_width;

		this->update();
	}
}

void UIArrowLine::SetLineStyle(Qt::PenStyle line_style)
{
	if (line_style_ != line_style)
	{
		line_style_ = line_style;

		this->update();
	}
}

void UIArrowLine::SetArrowWingLength(int arrow_wing_length)
{
	if (arrow_wing_length_ != arrow_wing_length)
	{
		arrow_wing_length_ = arrow_wing_length;

		this->CalcArrowWingPositions();

		this->update();
	}
}

void UIArrowLine::paintEvent(QPaintEvent* paint_event)
{
	QWidget::paintEvent(paint_event);

	if (!pos_start_ || !pos_end_ || *pos_start_ == *pos_end_) { return; }

	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	QPen pen(color_);
	
	// line part
	{
		pen.setStyle(line_style_);
		pen.setWidth(line_width_);
		painter.setPen(pen);

		painter.drawLine(*pos_start_, (pos_arrow_wing0_ + pos_arrow_wing1_) * 0.5f);
	}

	// arrow part
	{
		pen.setStyle(Qt::PenStyle::SolidLine);
		pen.setWidth(2);
		painter.setPen(pen);

		painter.setBrush(QBrush(color_, Qt::SolidPattern));

		QPoint points[3] = { *pos_end_, pos_arrow_wing0_, pos_arrow_wing1_ };
		painter.drawPolygon(points, 3, Qt::WindingFill);
	}

	painter.end();
}

// x' = x * cosθ − y * sinθ
// y' = x * sinθ + y * cosθ
void UIArrowLine::CalcArrowWingPositions()
{
	if (!pos_start_ || !pos_end_ || *pos_start_ == *pos_end_) { return; }

	QPoint v = *pos_start_ - *pos_end_;

	double v_length_square = v.x() * v.x() + v.y() * v.y();
	double scale = qSqrt((arrow_wing_length_ * arrow_wing_length_) / v_length_square);

	v *= scale;

	auto rad = M_PI / 4; // 45 degree
	auto cos_rad = qCos(rad);
	auto sin_rad = qSin(rad);

	pos_arrow_wing0_ = QPoint(v.x() * cos_rad - v.y() * sin_rad, v.x() * sin_rad + v.y() * cos_rad) + *pos_end_;
	pos_arrow_wing1_ = QPoint(v.x() * cos_rad + v.y() * sin_rad, -v.x() * sin_rad + v.y() * cos_rad) + *pos_end_;
}

GAME_NAMESPACE_END