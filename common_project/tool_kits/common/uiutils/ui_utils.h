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
	**  ����			: �Ź���ͼ����
	**  picName		: ͼƬ����
	**  horzCorner	: �ĸ����ϸ��ӵĿ��
	**  vertCorner	: �ĸ����ϸ��ӵĸ߶�
	**  widthDst	: ����ͼƬ���ŵĿ��
	**  widthDst	: ����ͼƬ���ŵĸ߶�
	**  ����ֵ		: ������ɵ�ͼƬ
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