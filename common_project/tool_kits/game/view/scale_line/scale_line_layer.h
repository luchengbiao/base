#ifndef __GAME_VIEW_SCALE_LINE_SCALE_LINE_LAYER_H__
#define __GAME_VIEW_SCALE_LINE_SCALE_LINE_LAYER_H__
#include <memory>
#include <string>
#include <vector>
#include <QWidget>
#include "game/macros.h"

GAME_NAMESPACE_BEGIN

class ScaleLines;

class UIScaleLineLayer : public QWidget
{
	Q_OBJECT

	typedef std::unique_ptr<ScaleLines> ScaleLinesPtr;

public:
	explicit UIScaleLineLayer(QWidget* parent = nullptr);
	~UIScaleLineLayer();

	void			SetXScaleLines(const QPoint& offset, int step_width, const std::vector<std::string>& vec_text, int font_size, const QSize& size);
	void			SetYScaleLines(const QPoint& offset, int step_width, const std::vector<std::string>& vec_text, int font_size, const QSize& size);

private:
	ScaleLinesPtr	x_lines_{ nullptr };
	ScaleLinesPtr	y_lines_{ nullptr };
};

GAME_NAMESPACE_END

#endif