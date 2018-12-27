#include "chess_path_marker_layer.h"
#include <QBrush>
#include <QPainter>
#include <QPaintEvent>
#include <QPen>


GAME_NAMESPACE_BEGIN

UIChessPathMarkerLayer::UIChessPathMarkerLayer(QWidget* parent)
: QWidget(parent)
{}

void UIChessPathMarkerLayer::SetMarkerPoints(const Point2ColorMap& map_global_point_color)
{
	map_point_color_.clear();
	
	if (!map_global_point_color.empty())
	{
		for (const auto& pair : map_global_point_color)
		{
			map_point_color_.emplace(this->mapFromGlobal(pair.first), pair.second);
		}
	}
	
	this->update();
}

void  UIChessPathMarkerLayer::SetMarkerDotRadius(int marker_dot_radius)
{
	if (marker_dot_radius_ != marker_dot_radius)
	{
		marker_dot_radius_ = marker_dot_radius;

		this->update();
	}
}

void UIChessPathMarkerLayer::paintEvent(QPaintEvent* paint_event)
{
	QWidget::paintEvent(paint_event);

	if (map_point_color_.empty()) { return; }

	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	painter.setPen(Qt::NoPen);

	QColor color = map_point_color_.begin()->second;
	painter.setBrush(QBrush(color, Qt::SolidPattern));

	for (const auto& pair : map_point_color_)
	{
		if (pair.second != color)
		{
			color = pair.second;
			painter.setBrush(QBrush(color, Qt::SolidPattern));
		}

		painter.drawEllipse(pair.first, marker_dot_radius_, marker_dot_radius_);
	}

	painter.end();
}

GAME_NAMESPACE_END