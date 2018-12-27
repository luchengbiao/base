#ifndef __GAME_WIDGET_WITH_PIXMAP_WIDGET_WITH_PIXMAP_H__
#define __GAME_WIDGET_WITH_PIXMAP_WIDGET_WITH_PIXMAP_H__
#include <memory>
#include <QWidget>

class QPixmap;

// because pixmap in QLabel will be gray when QLabel is disabled, so implement our own WidgetWithPixmap
class WidgetWithPixmap : public QWidget
{
	Q_OBJECT

public:
	explicit WidgetWithPixmap(QWidget* parent = nullptr);
	WidgetWithPixmap(const QPixmap& pixmap, QWidget* parent = nullptr);

	~WidgetWithPixmap();

	// QLabel::setPixmap compatibility:
	const QPixmap*  pixmap() const;
	void			setPixmap(const QPixmap& pixmap);

protected:
	virtual void	paintEvent(QPaintEvent* paint_event) override;

protected:
	class WidgetWithPixmapImpl;
	typedef std::unique_ptr<WidgetWithPixmapImpl> WidgetWithPixmapImplPtr;
	WidgetWithPixmapImplPtr impl_{ nullptr };
};

#endif