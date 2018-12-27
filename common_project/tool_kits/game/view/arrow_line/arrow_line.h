#ifndef __GAME_VIEW_ARROW_LINE_ARROW_LINE_H__
#define __GAME_VIEW_ARROW_LINE_ARROW_LINE_H__
#include <memory>
#include <QColor>
#include <QWidget>
#include "game/macros.h"

GAME_NAMESPACE_BEGIN

class UIArrowLine : public QWidget
{
	Q_OBJECT

	typedef std::unique_ptr<QPoint> QPointPtr;

public:
	explicit UIArrowLine(QWidget* parent = nullptr);
	~UIArrowLine();

	void			SetStartAndEndPosition(const QPoint& pos_start_global, const QPoint& pos_end_global);

	void			SetStartPosition(const QPoint& pos_global);

	void			SetEndPosition(const QPoint& pos_global);

	void			SetColor(const QColor& color);

	void			SetLineWidth(int line_width);

	void			SetLineStyle(Qt::PenStyle line_style);

	void			SetArrowWingLength(int arrow_wing_length);

private:
	virtual void	paintEvent(QPaintEvent*) override;

	void			InnerSetStartPosition(const QPoint& pos_global);
	void			InnerSetEndPosition(const QPoint& pos_global);

	void			CalcArrowWingPositions();

private:
	QPointPtr		pos_start_{ nullptr };
	QPointPtr		pos_end_{ nullptr };

	QColor			color_{ 245, 183, 95, 255 };
	int				line_width_{ 8 };
	Qt::PenStyle	line_style_{ Qt::PenStyle::SolidLine };

	int				arrow_wing_length_{ 15 };
	QPoint			pos_arrow_wing0_;
	QPoint			pos_arrow_wing1_;
};

GAME_NAMESPACE_END

#endif