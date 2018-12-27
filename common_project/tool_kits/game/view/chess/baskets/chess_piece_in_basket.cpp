#include "chess_piece_in_basket.h"
#include <QResizeEvent>
#include "common/uiutils/ui_utils.h"
#include "game/view/widget_with_pixmap/widget_with_pixmap.h"

GAME_NAMESPACE_BEGIN

UIChessPieceInBasket::UIChessPieceInBasket(QWidget* parent) 
: QWidget(parent)
, chess_bk_(new WidgetWithPixmap(this))
, chess_piece_(new MyChessPiece(this))
{
	this->Init();
}

void UIChessPieceInBasket::Init()
{
	QSize fixed_size(62, 62);
	this->setFixedSize(fixed_size);
	QPoint center(fixed_size.width() / 2, fixed_size.height() / 2);

	QPixmap pxp_chess_bk(":/chess/res/images/chess/chess_bk.png");
	if (!pxp_chess_bk.isNull())
	{
		chess_bk_->setPixmap(pxp_chess_bk);
		chess_bk_->setFixedSize(pxp_chess_bk.size());
	}

	chess_piece_->setFixedSize(fixed_size);

	UiUtils::MoveWidgetMiddleTo(chess_bk_, center);
	UiUtils::MoveWidgetMiddleTo(chess_piece_, center);

	chess_bk_->raise();
	chess_piece_->raise();
}

static QPoint Offset_Disabled_Flag{4, 5};

void UIChessPieceInBasket::ShowDisabledFlag()
{
	if (!disabled_flag_)
	{
		disabled_flag_ = new WidgetWithPixmap(this);
		disabled_flag_->raise();

		QPixmap pxp_disabled(":/common/res/images/common/disabled_flag.png");
		if (!pxp_disabled.isNull())
		{
			disabled_flag_->setPixmap(pxp_disabled);
			disabled_flag_->setFixedSize(pxp_disabled.size());
		}

		UiUtils::MoveWidgetRightBottomTo(disabled_flag_, QPoint(width(), height()) + Offset_Disabled_Flag);
	}

	disabled_flag_->show();
}

void UIChessPieceInBasket::HideDisabledFlag()
{
	if (disabled_flag_)
	{
		disabled_flag_->hide();
	}
}

void UIChessPieceInBasket::SetDisabledFlagVisible(bool visible)
{
	visible ? ShowDisabledFlag() : HideDisabledFlag();
}

void UIChessPieceInBasket::SetChessId(const ChessId chess_id)
{
	chess_piece_->SetChessId(chess_id);

	UiUtils::MoveWidgetMiddleTo(chess_piece_, QPoint(width() / 2, height() / 2));
}

ChessId UIChessPieceInBasket::GetChessId() const
{
	return chess_piece_->GetChessId();
}

QRect UIChessPieceInBasket::GlobalRectOfChessPiece()
{
	return QRect(chess_piece_->mapToGlobal(QPoint(0, 0)), chess_piece_->size());
}

QPoint UIChessPieceInBasket::GlobalPositionOfChessPieceCenter()
{
	return chess_piece_->mapToGlobal(QPoint(chess_piece_->width() * 0.5f, chess_piece_->height() * 0.5f));
}

void UIChessPieceInBasket::resizeEvent(QResizeEvent* resize_event)
{
	auto size = resize_event->size();
	QPoint center(size.width() / 2, size.height() / 2);

	UiUtils::MoveWidgetMiddleTo(chess_bk_, center);
	UiUtils::MoveWidgetMiddleTo(chess_piece_, center);
	if (disabled_flag_)
	{
		UiUtils::MoveWidgetRightBottomTo(disabled_flag_, QPoint(width(), height()) + Offset_Disabled_Flag);
	}
}

GAME_NAMESPACE_END