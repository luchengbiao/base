#include "grid_mark.h"
#include <QPainter>

GAME_NAMESPACE_BEGIN

UIGridMark::UIGridMark(QWidget* parent)
: QWidget(parent)
{}

void UIGridMark::SetColor(const QColor& color)
{
	if (color_ != color)
	{
		color_ = color;

		update();
	}
}

void UIGridMark::SetLineWidth(int line_width)
{
	if (line_width_ != line_width)
	{
		line_width_ = line_width;

		update();
	}
}

void UIGridMark::paintEvent(QPaintEvent* paint_event)
{
	QWidget::paintEvent(paint_event);

	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	QPen pen(color_);
	pen.setWidth(line_width_);
	pen.setStyle(Qt::PenStyle::DotLine);
	painter.setPen(pen);

	painter.drawEllipse(0, 0, width(), height());

	painter.end();
}

GAME_NAMESPACE_END