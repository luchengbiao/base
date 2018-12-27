#include "chess_basket_base.h"
#include "game/view/base/operational_layer/operational_hook_layer.h"

GAME_NAMESPACE_BEGIN

UIChessBasketBase::UIChessBasketBase(QWidget* parent)
: QWidget(parent)
{
	setWindowFlags(Qt::FramelessWindowHint);
}

void UIChessBasketBase::SetBasketEnabled(bool enabled)
{
	basket_enabled_ = enabled;
}

void UIChessBasketBase::SwitchToGameBoard(GameBoardType board_type)
{
	current_board_type_ = board_type;
}

QWidget* UIChessBasketBase::CreateTurnMask(const QString& qss_object_name)
{
	auto mask = new QWidget(this);
	mask->setObjectName(qss_object_name);

	mask->raise();
	mask->hide();

	mask->setAttribute(Qt::WA_TransparentForMouseEvents);

	return mask;
}

UIOperationalHookLayer* UIChessBasketBase::CreateOperationalHookLayer()
{
	auto hook_layer = new UIOperationalHookLayer(this);

	hook_layer->raise();
	hook_layer->show();

	return hook_layer;
}

void UIChessBasketBase::AssociateOperationalLayer(UIOperationalLayer* operation_layer)
{
	black_side_wrapper_.AssociateOperationalLayer(operation_layer);
	white_side_wrapper_.AssociateOperationalLayer(operation_layer);
}

GAME_NAMESPACE_END