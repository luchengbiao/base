#include "widget_with_pixmap.h"
#include <memory>
#include <QPainter>
#include <QPaintEvent>
#include <QPixmap>

class WidgetWithPixmap::WidgetWithPixmapImpl
{
	friend class WidgetWithPixmap;

	inline const QPixmap* pixmap() const
	{
		return pixmap_.get();
	}

	inline void setPixmap(const QPixmap& pixmap)
	{
		if (!pixmap_)
		{
			pixmap_.reset(new QPixmap);
		}

		*pixmap_ = pixmap;
	}

	inline bool paint(WidgetWithPixmap* widget, QPaintEvent* paint_event)
	{
		if (pixmap_ && !pixmap_->isNull())
		{
			QPainter painter(widget);

			painter.drawPixmap((widget->width() - pixmap_->width()) * 0.5f, (widget->height() - pixmap_->height()) * 0.5f, *pixmap_);

			return true;
		}

		return false;
	}

	std::unique_ptr<QPixmap> pixmap_;
};

WidgetWithPixmap::WidgetWithPixmap(QWidget* parent)
: QWidget(parent)
, impl_(new WidgetWithPixmapImpl)
{}

WidgetWithPixmap::WidgetWithPixmap(const QPixmap& pixmap, QWidget* parent)
: WidgetWithPixmap(parent)
{
	impl_->setPixmap(pixmap);
}

WidgetWithPixmap::~WidgetWithPixmap() = default;

const QPixmap* WidgetWithPixmap::pixmap() const
{
	return impl_->pixmap();
}

void WidgetWithPixmap::setPixmap(const QPixmap& pixmap)
{
	impl_->setPixmap(pixmap);

	this->update();
}

void  WidgetWithPixmap::paintEvent(QPaintEvent* paint_event)
{
	if (!impl_->paint(this, paint_event))
	{
		QWidget::paintEvent(paint_event);
	}
}