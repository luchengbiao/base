#include "player_wrapper_in_basket.h"
#include <QWidget>
#include "game/view/base/operational_layer/operational_hook_layer.h"
#include "game/view/base/operational_layer/operational_layer.h"
#include "game/view/player_bar/player_bar.h"
#include "game/public/structs/vs_player_info/vs_player_info.h"

GAME_NAMESPACE_BEGIN

PlayerWrapperInBasket::PlayerWrapperInBasket(PlayerBar* player_bar, QWidget* player_turn_mask, UIOperationalHookLayer* operational_hook_layer)
: player_bar_(player_bar)
, player_turn_mask_(player_turn_mask)
, operational_hook_layer_(operational_hook_layer)
{}

void PlayerWrapperInBasket::ShowPlayeBar()
{
	if (!player_bar_.isNull())
	{
		player_bar_->show();
	}
}

void PlayerWrapperInBasket::HidePlayeBar()
{
	if (!player_bar_.isNull())
	{
		player_bar_->hide();
	}
}

void PlayerWrapperInBasket::SetPlayeBarVisible(bool visible)
{
	visible ? ShowPlayeBar() : HidePlayeBar();
}

void PlayerWrapperInBasket::SetPlayerInfo(const VSPlayerInfo& player_info)
{
	if (!player_bar_.isNull())
	{
		player_bar_->SetPlayerInfo(player_info);
	}
}

void PlayerWrapperInBasket::ShowTurnMask()
{
	if (!player_turn_mask_.isNull())
	{
		player_turn_mask_->show();
	}
}

void PlayerWrapperInBasket::HideTurnMask()
{
	if (!player_turn_mask_.isNull())
	{
		player_turn_mask_->hide();
	}
}

void PlayerWrapperInBasket::SetTurnMaskVisible(bool visible)
{
	visible ? ShowTurnMask() : HideTurnMask();
}

void PlayerWrapperInBasket::SetGeometryOfTurnMask(const QRect& geometry)
{
	if (!player_turn_mask_.isNull())
	{
		player_turn_mask_->setGeometry(geometry);
	}
}

void PlayerWrapperInBasket::SetGeometryOfOperationalHookLayer(const QRect& geometry)
{
	if (!operational_hook_layer_.isNull())
	{
		operational_hook_layer_->setGeometry(geometry);
	}
}

void PlayerWrapperInBasket::AssociateOperationalLayer(UIOperationalLayer* operational_layer)
{
	if (!operational_hook_layer_.isNull())
	{
		operational_hook_layer_->AssociateOperationalLayer(operational_layer);
	}
}

void PlayerWrapperInBasket::Reset(PlayerBar* player_bar, QWidget* player_turn_mask, UIOperationalHookLayer* operational_hook_layer)
{
	player_bar_ = player_bar;
	player_turn_mask_ = player_turn_mask;
	operational_hook_layer_ = operational_hook_layer;
}

GAME_NAMESPACE_END