#include "ui_utils.h"

namespace UiUtils
{
	QPoint	GetWidgetLocalCenterPosition(const QWidget* widget)
	{
		return QPoint(widget->width() * 0.5f, widget->height() * 0.5f);
	}

	QPoint	GetWidgetGlobalCenterPosition(const QWidget* widget)
	{
		return widget->mapToGlobal(GetWidgetLocalCenterPosition(widget));
	}

	void AlignWidgetOnCenter(QWidget* widgetSrc, const QWidget* widgetDst)
	{
		auto globalCenter = GetWidgetGlobalCenterPosition(widgetDst);

		auto widgetParent = dynamic_cast<QWidget*>(widgetSrc->parent());
		if (widgetParent)
		{
			auto localCenter = widgetParent->mapFromGlobal(globalCenter);

			widgetSrc->move(localCenter - GetWidgetLocalCenterPosition(widgetSrc));
		}
		else
		{
			widgetSrc->move(globalCenter - GetWidgetLocalCenterPosition(widgetSrc));
		}
	}
}
