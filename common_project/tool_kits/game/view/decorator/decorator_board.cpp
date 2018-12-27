#include "decorator_board.h"
#include <QResizeEvent>
#include "common/macros/assert.h"
#include "base_control/painter/draw_board.h"
#include "base_control/painter/chess_mark_widget.h"
#include "base_control/painter/go_mark_widget.h"
#include "game/model/go/go_trait.h"
#include "game/public/public_setting/public_setting.h"

GAME_NAMESPACE_BEGIN

UIDecoratorBoard::UIDecoratorBoard(const GameBoardType board_type, const QSize& fixed_size, QWidget* parent)
: QWidget(parent)
{
	this->Init(board_type, fixed_size);
}

void UIDecoratorBoard::Init(const GameBoardType board_type, const QSize& fixed_size)
{
	PaintBaseInfo paint;
	paint.width = fixed_size.width();
	paint.height = fixed_size.height();
	paint.offset_x_ = 0;
	paint.offset_y_ = 0;
	paint.b_stu_ = ClientTypeIsStudent();
	paint.b_accept_event_ = false;

	draw_board_ = new DrawBoard(paint, this);
	draw_board_->setFixedSize(fixed_size);
	draw_board_->SetShowBkImage(false);
	draw_board_->SetTeaPenColor(QColor(255, 0, 0));
	draw_board_->SetVisibleEx(false);

	if (board_type == GameBoardType::Chess)
	{
		mark_board_ = new ChessMarkForm(fixed_size, this);
		mark_board_->setGeometry(0, 0, fixed_size.width(), fixed_size.height());
		mark_board_->OnSetParamCalValidPoint(QPoint(26, 24), 58); // ugly to hard code here!
		mark_board_->setVisible(true);
	}
	else
	{
		mark_board_ = new GoMarkForm(fixed_size, this);
		int grid = GoTrait::GoBoardGridsByType(board_type);
		const auto& board_info = GoTrait::GoBoardInfoByGrid(grid);

		mark_board_->OnSetParamCalValidPoint(board_info.offset_, 4, grid, board_info.grid_width_ * (grid - 1));
		mark_board_->setGeometry(0, 0, fixed_size.width(), fixed_size.height());
		mark_board_->setVisible(true);
	}

	this->setFixedSize(fixed_size);
}

void UIDecoratorBoard::SetCallbackForDrawBoard(const std::function<void(const DrawOpInfo&)>& draw_callback)
{
	draw_board_->SetDrawCb(draw_callback);
}

void UIDecoratorBoard::SetCallbackForMarkBoard(const std::function<void(const MarkOpInfo&)>& mark_callback)
{
	mark_board_->OnSendCommandCallBack(mark_callback);
}

void UIDecoratorBoard::SetGridGlobalPositionDelegateForMarkBoard(const std::function<QPoint(int, int)>& mark_delegate)
{
	mark_board_->SetGridGlobalPositionDelegate(mark_delegate);
}

bool UIDecoratorBoard::IsActive() const
{
	return draw_board_->IsEventAccepted() || mark_board_->GetAcceptEvent();
}

void UIDecoratorBoard::SetDrawBoardActive(bool active)
{
	draw_board_->SetAcceptEvent(active);

	if (draw_board_->IsEventAccepted())
	{
		mark_board_->SetAcceptEvent(false);
		draw_board_->raise();
	}
}

void UIDecoratorBoard::SetMarkBoardActive(bool active)
{
	mark_board_->SetAcceptEvent(active);

	if (mark_board_->GetAcceptEvent())
	{
		draw_board_->SetAcceptEvent(false);
		mark_board_->raise();
	}
}

void UIDecoratorBoard::resizeEvent(QResizeEvent* resize_event)
{
	auto size = resize_event->size();

	draw_board_->setGeometry(0, 0, size.width(), size.height());
	mark_board_->setGeometry(0, 0, size.width(), size.height());
}

void UIDecoratorBoard::showEvent(QShowEvent*)
{
	draw_board_->SetVisibleEx(true);
	draw_board_->show();

	mark_board_->show();
}

void UIDecoratorBoard::hideEvent(QHideEvent*)
{
	draw_board_->SetVisibleEx(false);
	draw_board_->hide();

	mark_board_->hide();
}

void UIDecoratorBoard::Reset()
{
	draw_board_->ClearDrawedUnits();
	draw_board_->SetAcceptEvent(false);

	mark_board_->OnHideAllMarkIcons();
	draw_board_->SetAcceptEvent(false);
}

GAME_NAMESPACE_END