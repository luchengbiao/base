#include "go_color_basket.h"
#include <QMouseEvent>
#include "common/uiutils/ui_utils.h"
#include "game/view/widget_with_pixmap/widget_with_pixmap.h"
#include "go_piece_in_basket.h"

GAME_NAMESPACE_BEGIN

UIGoColorBasket::UIGoColorBasket(GoTrait::Color color, QWidget* parent)
: QWidget(parent)
, color_(color)
{
	this->Init();
}

void UIGoColorBasket::Init()
{
	basket_ = new WidgetWithPixmap(this);
	QPixmap pxp_basket(color_ == GoColor::White ? ":/go/res/images/go/basket_white.png" : ":/go/res/images/go/basket_black.png");
	if (!pxp_basket.isNull())
	{
		basket_->setPixmap(pxp_basket);
		basket_->setFixedSize(pxp_basket.size());
	}

	this->setFixedSize(basket_->size());
}

QRect UIGoColorBasket::ChessGlobalArea() const
{
	return QRect(basket_->mapToGlobal(PosOfChessArea()), SizeOfChessArea());
}

QPoint UIGoColorBasket::GlobalPosOfChessAreaCenter() const
{
	return basket_->mapToGlobal(PosOfChessArea() + QPoint(SizeOfChessArea().width() * 0.5f, SizeOfChessArea().height() * 0.5f));
}

void UIGoColorBasket::ShowDisabledFlag()
{
	if (!disabled_flag_)
	{
		disabled_flag_ = new WidgetWithPixmap(basket_);

		QPixmap pxp_disabled(":/common/res/images/common/disabled_flag.png");
		if (!pxp_disabled.isNull())
		{
			disabled_flag_->setPixmap(pxp_disabled);
			disabled_flag_->setFixedSize(pxp_disabled.size());
		}

		UiUtils::MoveWidgetRightBottomTo(disabled_flag_, PosOfChessArea() + QPoint(SizeOfChessArea().width() + 5, SizeOfChessArea().height()));
	}

	disabled_flag_->show();
}

void UIGoColorBasket::HideDisabledFlag()
{
	if (disabled_flag_)
	{
		disabled_flag_->hide();
	}
}

void UIGoColorBasket::SetDisabledFlagVisible(bool visible)
{
	visible ? ShowDisabledFlag() : HideDisabledFlag();
}

void UIGoColorBasket::ShowFixedChess(const int grid)
{
	this->LazyCreateGoPieces(this);

	switch (grid)
	{
	case 9:
		go_piece_9_->show();
		break;

	case 13:
		go_piece_13_->show();
		break;

	case 19:
		go_piece_19_->show();
		break;

	default:
		break;
	}
}

void UIGoColorBasket::HideFixedChesses()
{
	if (!go_piece_9_.isNull()) { go_piece_9_->hide(); }

	if (!go_piece_13_.isNull()) { go_piece_13_->hide(); }

	if (!go_piece_19_.isNull()) { go_piece_19_->hide(); }
}

void UIGoColorBasket::LazyCreateGoPieces(QWidget* parent)
{
	if (!go_piece_9_.isNull()) { return; }
	bool created = false;

	auto parent_tmp = parent ? parent : this;

	go_piece_9_ = CreateGoPiece(9, parent_tmp);
	go_piece_13_ = CreateGoPiece(13, parent_tmp);
	go_piece_19_ = CreateGoPiece(19, parent_tmp);

	this->AdjustGeometryOfGoPieces();
}

UIGoPieceBaseInBasket* UIGoColorBasket::CreateGoPiece(const int grid, QWidget* parent)
{
	auto go_piece = UIGoPieceBaseInBasket::Create(grid, parent);

	int width = GoTrait::GoBoardInfoByGrid(grid).chess_width_;
	go_piece->setFixedSize(width, width);

	go_piece->SetChessId(color_ == GoColor::White ? GoTrait::IdOfWhitePiece : GoTrait::IdOfBlackPiece);

	go_piece->hide();
	go_piece->raise();

	return go_piece;
}

void UIGoColorBasket::AdjustGeometryOfGoPieces()
{
	if (!go_piece_9_.isNull())
	{
		auto pos = QPoint(PosOfChessArea().x() + SizeOfChessArea().width(), 0);

		pos = go_piece_9_->parentWidget()->mapFromGlobal(basket_->mapToGlobal(pos)) + QPoint(-10, 5);

		UiUtils::MoveWidgetMiddleTo(go_piece_9_, pos);
		UiUtils::MoveWidgetMiddleTo(go_piece_13_, pos);
		UiUtils::MoveWidgetMiddleTo(go_piece_19_, pos);
	}
}

void UIGoColorBasket::mouseReleaseEvent(QMouseEvent* mouse_event)
{
	if (QRect(PosOfChessArea(), SizeOfChessArea()).contains(mouse_event->pos()))
	{
		emit Clicked();
	}
}

GAME_NAMESPACE_END