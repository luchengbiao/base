#include "operational_layer.h"
#include <QMouseEvent>

GAME_NAMESPACE_BEGIN

UIOperationalLayer::UIOperationalLayer(QWidget* parent) 
: QWidget(parent) 
{}

void UIOperationalLayer::HandleMousePressEvent(const QPoint& global_pos)
{
	this->HandleMousePressEvent(mapFromGlobal(global_pos), global_pos);
}

void UIOperationalLayer::HandleMouseMoveEvent(const QPoint& global_pos)
{
	this->HandleMouseMoveEvent(mapFromGlobal(global_pos), global_pos);
}

void UIOperationalLayer::HandleMouseReleaseEvent(const QPoint& global_pos)
{
	this->HandleMouseReleaseEvent(mapFromGlobal(global_pos), global_pos);
}

void UIOperationalLayer::mousePressEvent(QMouseEvent* mouse_event)
{
	this->HandleMousePressEvent(mouse_event->pos(), mouse_event->globalPos());
}

void UIOperationalLayer::mouseMoveEvent(QMouseEvent* mouse_event)
{
	this->HandleMouseMoveEvent(mouse_event->pos(), mouse_event->globalPos());
}

void UIOperationalLayer::mouseReleaseEvent(QMouseEvent* mouse_event)
{
	this->HandleMouseReleaseEvent(mouse_event->pos(), mouse_event->globalPos());
}

GAME_NAMESPACE_END