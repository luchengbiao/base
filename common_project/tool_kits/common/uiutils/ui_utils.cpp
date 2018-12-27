#include "ui_utils.h"
#include <QLayout>
#include <QLayoutItem>
#include <QPainter>
#include <QPixmap>
#include <QWidget>

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

	QRect GetWidgetGlobalGeometry(const QWidget* widget)
	{
		return QRect(widget->mapToGlobal(QPoint(0, 0)), widget->size());
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

#define LEFT_ANCHOR		0.0f
#define RGIHT_ANCHOR	1.0f
#define TOP_ANCHOR		0.0f
#define BOTTOM_ANCHOR	1.0f
#define MIDDLE_ANCHOR	0.5f

	void MoveWidgetAnchorPointTo(QWidget* widget, const QPoint& pos, const QPointF& anchorPoint)
	{
		widget->move(pos.x() - anchorPoint.x() * widget->width(), pos.y() - anchorPoint.y() * widget->height());
	}

	void MoveWidgetLeftTopTo(QWidget* widget, const QPoint& pos)
	{
		widget->move(pos);
	}

	void MoveWidgetMiddleTopTo(QWidget* widget, const QPoint& pos)
	{
		MoveWidgetAnchorPointTo(widget, pos, QPointF(MIDDLE_ANCHOR, TOP_ANCHOR));
	}

	void MoveWidgetRightTopTo(QWidget* widget, const QPoint& pos)
	{
		MoveWidgetAnchorPointTo(widget, pos, QPointF(RGIHT_ANCHOR, TOP_ANCHOR));
	}

	void MoveWidgetLeftMiddleTo(QWidget* widget, const QPoint& pos)
	{
		MoveWidgetAnchorPointTo(widget, pos, QPointF(LEFT_ANCHOR, MIDDLE_ANCHOR));
	}

	void MoveWidgetMiddleTo(QWidget* widget, const QPoint& pos)
	{
		MoveWidgetAnchorPointTo(widget, pos, QPointF(MIDDLE_ANCHOR, MIDDLE_ANCHOR));
	}

	void MoveWidgetRightMiddleTo(QWidget* widget, const QPoint& pos)
	{
		MoveWidgetAnchorPointTo(widget, pos, QPointF(RGIHT_ANCHOR, MIDDLE_ANCHOR));
	}

	void MoveWidgetLeftBottomTo(QWidget* widget, const QPoint& pos)
	{
		MoveWidgetAnchorPointTo(widget, pos, QPointF(LEFT_ANCHOR, BOTTOM_ANCHOR));
	}

	void MoveWidgetMiddleBottomTo(QWidget* widget, const QPoint& pos)
	{
		MoveWidgetAnchorPointTo(widget, pos, QPointF(MIDDLE_ANCHOR, BOTTOM_ANCHOR));
	}

	void MoveWidgetRightBottomTo(QWidget* widget, const QPoint& pos)
	{
		MoveWidgetAnchorPointTo(widget, pos, QPointF(RGIHT_ANCHOR, BOTTOM_ANCHOR));
	}

	QPoint WidgetPosByAnchorPoint(const QWidget* widget, const QPointF& anchorPoint)
	{
		return QPoint(widget->x() + anchorPoint.x() * widget->width(), widget->y() + anchorPoint.y() * widget->height());
	}

	QPoint WidgetPosOfLeftTop(QWidget* widget)
	{
		return widget->pos();
	}

	QPoint WidgetPosOfMiddleTop(QWidget* widget)
	{
		return WidgetPosByAnchorPoint(widget, QPointF(MIDDLE_ANCHOR, TOP_ANCHOR));
	}

	QPoint WidgetPosOfRightTop(QWidget* widget)
	{
		return WidgetPosByAnchorPoint(widget, QPointF(RGIHT_ANCHOR, TOP_ANCHOR));
	}

	QPoint WidgetPosOfLeftMiddle(QWidget* widget)
	{
		return WidgetPosByAnchorPoint(widget, QPointF(LEFT_ANCHOR, MIDDLE_ANCHOR));
	}

	QPoint WidgetPosOfMiddle(QWidget* widget)
	{
		return WidgetPosByAnchorPoint(widget, QPointF(MIDDLE_ANCHOR, MIDDLE_ANCHOR));
	}

	QPoint WidgetPosOfRightMiddle(QWidget* widget)
	{
		return WidgetPosByAnchorPoint(widget, QPointF(RGIHT_ANCHOR, MIDDLE_ANCHOR));
	}

	QPoint WidgetPosOfLeftBottom(QWidget* widget)
	{
		return WidgetPosByAnchorPoint(widget, QPointF(LEFT_ANCHOR, BOTTOM_ANCHOR));
	}

	QPoint WidgetPosOfMiddleBottom(QWidget* widget)
	{
		return WidgetPosByAnchorPoint(widget, QPointF(MIDDLE_ANCHOR, BOTTOM_ANCHOR));
	}

	QPoint WidgetPosOfRightBottom(QWidget* widget)
	{
		return WidgetPosByAnchorPoint(widget, QPointF(RGIHT_ANCHOR, BOTTOM_ANCHOR));
	}

	bool WidgetContainsGlobalPosition(const QWidget* widget, const QPoint& globalPos)
	{
		auto localPos = widget->mapFromGlobal(globalPos);

		auto rect = QRect(0, 0, widget->width(), widget->height());

		return rect.contains(localPos);
	}

	void RemoveAllWidgetsInLayout(QLayout* layout)
	{
		int cnt = layout->count();
		for (int i = cnt-1; i>=0; i--)
		{
			QLayoutItem* item = layout->itemAt(i);
			QWidget* widget = item->widget();
			if (widget)
			{
				widget->hide();
				layout->removeWidget(widget);
				widget->deleteLater();
				widget = nullptr;
			}
		}
	}

	void RemoveAllChildWidgets(QWidget* widget)
	{
		auto list = widget->findChildren<QWidget*>();
		for (auto item : list)
		{
			item->deleteLater();
		}
	}

	QWidget* GetAncestorWidget(QWidget* widget)
	{
		QWidget* parent = widget;
		while (parent)
		{
			auto parent_parent = parent->parentWidget();
			if (parent_parent)
			{
				parent = parent_parent;
			}
			else
			{
				break;
			}
		}

		return parent;
	}

	std::unique_ptr<QPixmap> PixmapFrom9Patch(const QPixmap& pixSrc, int horzCorner, int vertCorner, int widthDst, int heightDst)
	{
		QPixmap* pixRet = nullptr;

		do
		{
			if (pixSrc.isNull()) { break; }

			int pixWidth = pixSrc.width();
			int pixHeight = pixSrc.height();

			QPixmap pix_1 = pixSrc.copy(0, 0, horzCorner, vertCorner);
			QPixmap pix_2 = pixSrc.copy(horzCorner, 0, pixWidth - horzCorner * 2, vertCorner);
			QPixmap pix_3 = pixSrc.copy(pixWidth - horzCorner, 0, horzCorner, vertCorner);

			QPixmap pix_4 = pixSrc.copy(0, vertCorner, horzCorner, pixHeight - vertCorner * 2);
			QPixmap pix_5 = pixSrc.copy(horzCorner, vertCorner, pixWidth - horzCorner * 2, pixHeight - vertCorner * 2);
			QPixmap pix_6 = pixSrc.copy(pixWidth - horzCorner, vertCorner, horzCorner, pixHeight - vertCorner * 2);

			QPixmap pix_7 = pixSrc.copy(0, pixHeight - vertCorner, horzCorner, vertCorner);
			QPixmap pix_8 = pixSrc.copy(horzCorner, pixHeight - vertCorner, pixWidth - horzCorner * 2, pixWidth - horzCorner * 2);
			QPixmap pix_9 = pixSrc.copy(pixWidth - horzCorner, pixHeight - vertCorner, horzCorner, vertCorner);

			pix_2 = pix_2.scaled(widthDst - horzCorner * 2, vertCorner, Qt::IgnoreAspectRatio);//保持高度拉宽
			pix_4 = pix_4.scaled(horzCorner, heightDst - vertCorner * 2, Qt::IgnoreAspectRatio);//保持宽度拉高
			pix_5 = pix_5.scaled(widthDst - horzCorner * 2, heightDst - vertCorner * 2, Qt::IgnoreAspectRatio);//宽高都缩放
			pix_6 = pix_6.scaled(horzCorner, heightDst - vertCorner * 2, Qt::IgnoreAspectRatio);//保持宽度拉高
			pix_8 = pix_8.scaled(widthDst - horzCorner * 2, vertCorner);//保持高度拉宽

			pixRet = new QPixmap(widthDst, heightDst);
			pixRet->fill(QColor(255, 255, 255, 0));
			if (!pixRet->isNull())
			{
				QPainter painter(pixRet);

				painter.drawPixmap(0, 0, pix_1);
				painter.drawPixmap(horzCorner, 0, pix_2);
				painter.drawPixmap(widthDst - horzCorner, 0, pix_3);

				painter.drawPixmap(0, vertCorner, pix_4);
				painter.drawPixmap(horzCorner, vertCorner, pix_5);
				painter.drawPixmap(widthDst - horzCorner, vertCorner, pix_6);

				painter.drawPixmap(0, heightDst - vertCorner, pix_7);
				painter.drawPixmap(horzCorner, heightDst - vertCorner, pix_8);
				painter.drawPixmap(widthDst - horzCorner, heightDst - vertCorner, pix_9);

				painter.end();
			}

		} while (false);

		return std::unique_ptr<QPixmap>(pixRet);
	}

	std::unique_ptr<QPixmap> PixmapFrom9Patch(const QString& picFilePath, int horzCorner, int vertCorner, int widthDst, int heightDst)
	{
		return PixmapFrom9Patch(QPixmap(picFilePath), horzCorner, vertCorner, widthDst, heightDst);
	}
}
