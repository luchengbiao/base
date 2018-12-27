#include "scale_line.h"
#include <QLabel>
#include <QResizeEvent>

GAME_NAMESPACE_BEGIN

UIScaleLine::UIScaleLine(const QString& text, int font_size, QWidget* parent)
: QWidget(parent)
{
	setWindowFlags(Qt::FramelessWindowHint);
	setAutoFillBackground(true);

	QPalette palette;
	palette.setBrush(backgroundRole(), Qt::BrushStyle::NoBrush);
	setPalette(palette);

	base_label_ = new ScaleLabel(this);
	base_label_->setStyleSheet(QString("QLabel{font-size:%1px;color:rgba(255,255,255,0.5);}").arg(font_size));
	base_label_->setAlignment(Qt::AlignCenter);
	base_label_->setText(text);

	fore_label_ = new ScaleLabel(this);
	fore_label_->setStyleSheet(QString("QLabel{font-size:%1px;color:#000000;}").arg(font_size));
	fore_label_->setAlignment(Qt::AlignCenter);
	fore_label_->setText(text);
}

void UIScaleLine::resizeEvent(QResizeEvent* resize_event)
{
	base_label_->setGeometry(QRect(QPoint(0, 1), resize_event->size()));
	fore_label_->setGeometry(QRect(QPoint(0, 0), resize_event->size()));
}

GAME_NAMESPACE_END