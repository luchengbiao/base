#ifndef __UI_UTILS_H__
#define __UI_UTILS_H__
#include <QWidget>

class QPixmap;

namespace UiUtils
{
	QPoint		GetWidgetLocalCenterPosition(const QWidget* widget);
	QPoint		GetWidgetGlobalCenterPosition(const QWidget* widget);

	QRect		GetWidgetGlobalGeometry(const QWidget* widget);

	void		AlignWidgetOnCenter(QWidget* widgetSrc, const QWidget* widgetDst);

	bool		WidgetContainsGlobalPosition(const QWidget* widget, const QPoint& globalPos);

	/*
	**  功能			: 九宫格图处理
	**  picName		: 图片名字
	**  horzCorner	: 四个角上格子的宽度
	**  vertCorner	: 四个角上格子的高度
	**  widthDst	: 整个图片缩放的宽度
	**  widthDst	: 整个图片缩放的高度
	**  返回值		: 处理完成的图片
	**	1 | 2 | 3
	**	---------
	**	4 | 5 | 6
	**	---------
	**	7 | 8 | 9
	*/
	QPixmap*	PixmapFrom9Patch(const QPixmap& pixSrc, int horzCorner, int vertCorner, int widthDst, int heightDst);
	QPixmap*	PixmapFrom9Patch(const QString& picFilePath, int horzCorner, int vertCorner, int widthDst, int heightDst);
}

#endif