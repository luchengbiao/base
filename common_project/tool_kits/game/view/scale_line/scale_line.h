#ifndef __GAME_VIEW_SCALE_LINE_SCALE_LINE_H__
#define __GAME_VIEW_SCALE_LINE_SCALE_LINE_H__
#include <QWidget>
#include "game/macros.h"

class QLabel;

GAME_NAMESPACE_BEGIN

class UIScaleLine : public QWidget
{
	Q_OBJECT

	typedef QLabel ScaleLabel;

public:
	explicit UIScaleLine(const QString& text, int font_size, QWidget* parent = nullptr);

protected:
	virtual void	resizeEvent(QResizeEvent* resize_event) override;

private:
	ScaleLabel*		base_label_{ nullptr };
	ScaleLabel*		fore_label_{ nullptr };
};

GAME_NAMESPACE_END

#endif