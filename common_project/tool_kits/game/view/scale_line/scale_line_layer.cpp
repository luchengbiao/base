#include "scale_line_layer.h"
#include "common/uiutils/ui_utils.h"
#include "scale_line.h"

GAME_NAMESPACE_BEGIN

class ScaleLines : public std::vector<UIScaleLine*>
{
public:
	ScaleLines() = default;

	void DeleteAll()
	{
		for (auto line : *this)
		{
			delete line;
		}
	}
};

UIScaleLineLayer::UIScaleLineLayer(QWidget* parent)
: QWidget(parent)
, x_lines_(new ScaleLines)
, y_lines_(new ScaleLines)
{
	setWindowFlags(Qt::FramelessWindowHint);
	setAutoFillBackground(true);

	QPalette palette;
	palette.setBrush(backgroundRole(), Qt::BrushStyle::NoBrush);
	setPalette(palette);
}

UIScaleLineLayer::~UIScaleLineLayer() = default;

void UIScaleLineLayer::SetXScaleLines(const QPoint& offset, int step_width, const std::vector<std::string>& vec_text, int font_size, const QSize& size)
{
	x_lines_->DeleteAll();

	int index = 0;
	for (const auto& text : vec_text)
	{
		auto scale_line = new UIScaleLine(QString::fromStdString(text), font_size, this);

		scale_line->setFixedSize(size);

		UiUtils::MoveWidgetMiddleBottomTo(scale_line, offset + QPoint(step_width * index, 0));

		x_lines_->push_back(scale_line);

		index += 1;
	}
}

void UIScaleLineLayer::SetYScaleLines(const QPoint& offset, int step_width, const std::vector<std::string>& vec_text, int font_size, const QSize& size)
{
	y_lines_->DeleteAll();

	int index = 0;
	for (const auto& text : vec_text)
	{
		auto scale_line = new UIScaleLine(QString::fromStdString(text), font_size, this);

		scale_line->setFixedSize(size);

		UiUtils::MoveWidgetRightMiddleTo(scale_line, offset + QPoint(0, step_width * index));

		y_lines_->push_back(scale_line);

		index += 1;
	}
}

GAME_NAMESPACE_END