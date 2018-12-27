#include "operational_hook_layer.h"
#include <QMouseEvent>
#include "game/view/base/operational_layer/operational_layer.h"

GAME_NAMESPACE_BEGIN

UIOperationalHookLayer::UIOperationalHookLayer(QWidget* parent) 
: QWidget(parent) 
{}

void UIOperationalHookLayer::AssociateOperationalLayer(UIOperationalLayer* operational_layer)
{
	operational_layer_ = operational_layer;
}

void UIOperationalHookLayer::mousePressEvent(QMouseEvent* mouse_event)
{
	if (!operational_layer_.isNull())
	{
		operational_layer_->HandleMousePressEvent(mouse_event->globalPos());
	}
}

void UIOperationalHookLayer::mouseMoveEvent(QMouseEvent* mouse_event)
{
	if (!operational_layer_.isNull())
	{
		operational_layer_->HandleMouseMoveEvent(mouse_event->globalPos());
	}
}

void UIOperationalHookLayer::mouseReleaseEvent(QMouseEvent* mouse_event)
{
	if (!operational_layer_.isNull())
	{
		operational_layer_->HandleMouseReleaseEvent(mouse_event->globalPos());
	}
}

GAME_NAMESPACE_END