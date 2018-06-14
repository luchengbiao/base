#ifndef __UI_UTILS_H__
#define __UI_UTILS_H__

#include <QWidget>

namespace UiUtils
{
	QPoint		GetWidgetLocalCenterPosition(const QWidget* widget);
	QPoint		GetWidgetGlobalCenterPosition(const QWidget* widget);

	void		AlignWidgetOnCenter(QWidget* widgetSrc, const QWidget* widgetDst);
}

#endif