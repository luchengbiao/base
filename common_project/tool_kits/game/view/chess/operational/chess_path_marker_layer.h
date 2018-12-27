#ifndef __GAME_VIEW_CHESS_OPERATIONAL_CHESS_PATH_MARKER_LAYER_H__
#define __GAME_VIEW_CHESS_OPERATIONAL_CHESS_PATH_MARKER_LAYER_H__
#include <map>
#include <QColor>
#include <QWidget>
#include "game/macros.h"

GAME_NAMESPACE_BEGIN

struct PointPredicter
{
	inline bool operator()(const QPoint& lh, const QPoint& rh)
	{
		if (lh.x() < rh.x()) { return true; }
		else if (lh.x() == rh.x())
		{
			return (lh.y() < rh.y());
		}
		return false;
	}
};

using Point2ColorMap = std::map<QPoint, QColor, PointPredicter>;

class UIChessPathMarkerLayer : public QWidget
{
	Q_OBJECT

public:
	explicit UIChessPathMarkerLayer(QWidget* parent = nullptr);

	void			SetMarkerPoints(const Point2ColorMap& map_global_point_color);

	void			SetMarkerDotRadius(int marker_dot_radius);

private:
	virtual void	paintEvent(QPaintEvent*) override;

private:
	Point2ColorMap	map_point_color_; // point is global coordinate

	int				marker_dot_radius_{ 10 };
};

GAME_NAMESPACE_END

#endif