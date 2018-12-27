#ifndef __UI_UTILS_H__
#define __UI_UTILS_H__
#include <memory>
#include <QPoint>
#include <QRect>

class QLayout;
class QPixmap;
class QWidget;

namespace UiUtils
{
	QPoint			GetWidgetLocalCenterPosition(const QWidget* widget);
	QPoint			GetWidgetGlobalCenterPosition(const QWidget* widget);

	QRect			GetWidgetGlobalGeometry(const QWidget* widget);

	void			AlignWidgetOnCenter(QWidget* widgetSrc, const QWidget* widgetDst);

	// pos post is in parent local-coordinate
	void			MoveWidgetAnchorPointTo(QWidget* widget, const QPoint& pos, const QPointF& anchorPoint);
	void			MoveWidgetLeftTopTo(QWidget* widget, const QPoint& pos);
	void			MoveWidgetMiddleTopTo(QWidget* widget, const QPoint& pos);
	void			MoveWidgetRightTopTo(QWidget* widget, const QPoint& pos);
	void			MoveWidgetLeftMiddleTo(QWidget* widget, const QPoint& pos);
	void			MoveWidgetMiddleTo(QWidget* widget, const QPoint& pos);
	void			MoveWidgetRightMiddleTo(QWidget* widget, const QPoint& pos);
	void			MoveWidgetLeftBottomTo(QWidget* widget, const QPoint& pos);
	void			MoveWidgetMiddleBottomTo(QWidget* widget, const QPoint& pos);
	void			MoveWidgetRightBottomTo(QWidget* widget, const QPoint& pos);

	// pos returned is in parent local-coordinate
	QPoint			WidgetPosByAnchorPoint(const QWidget* widget, const QPointF& anchorPoint);
	QPoint			WidgetPosOfLeftTop(QWidget* widget);
	QPoint			WidgetPosOfMiddleTop(QWidget* widget);
	QPoint			WidgetPosOfRightTop(QWidget* widget);
	QPoint			WidgetPosOfLeftMiddle(QWidget* widget);
	QPoint			WidgetPosOfMiddle(QWidget* widget);
	QPoint			WidgetPosOfRightMiddle(QWidget* widget);
	QPoint			WidgetPosOfLeftBottom(QWidget* widget);
	QPoint			WidgetPosOfMiddleBottom(QWidget* widget);
	QPoint			WidgetPosOfRightBottom(QWidget* widget);

	bool			WidgetContainsGlobalPosition(const QWidget* widget, const QPoint& globalPos);

	void			RemoveAllWidgetsInLayout(QLayout*);
	void			RemoveAllChildWidgets(QWidget*);

	QWidget*		GetAncestorWidget(QWidget*);

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
	std::unique_ptr<QPixmap>	PixmapFrom9Patch(const QPixmap& pixSrc, int horzCorner, int vertCorner, int widthDst, int heightDst);
	std::unique_ptr<QPixmap>	PixmapFrom9Patch(const QString& picFilePath, int horzCorner, int vertCorner, int widthDst, int heightDst);
}

#endif