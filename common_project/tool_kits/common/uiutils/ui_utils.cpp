#include "ui_utils.h"
#include <QPainter>
#include <QPixmap>

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

	bool WidgetContainsGlobalPosition(const QWidget* widget, const QPoint& globalPos)
	{
		auto localPos = widget->mapFromGlobal(globalPos);

		auto rect = QRect(0, 0, widget->width(), widget->height());

		return rect.contains(localPos);
	}

	QPixmap* PixmapFrom9Patch(const QPixmap& pixSrc, int horzCorner, int vertCorner, int widthDst, int heightDst)
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

		return pixRet;
	}

	QPixmap* PixmapFrom9Patch(const QString& picFilePath, int horzCorner, int vertCorner, int widthDst, int heightDst)
	{
		return PixmapFrom9Patch(QPixmap(picFilePath), horzCorner, vertCorner, widthDst, heightDst);
	}
}
