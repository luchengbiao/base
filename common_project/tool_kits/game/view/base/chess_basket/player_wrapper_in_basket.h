#ifndef __GAME_VIEW_BASE_CHESS_BASKET_PLAYER_WRAPPER_IN_BASKET_H__
#define __GAME_VIEW_BASE_CHESS_BASKET_PLAYER_WRAPPER_IN_BASKET_H__
#include <QPointer>
#include "game/macros.h"

class QWidget;
class QRect;

GAME_NAMESPACE_BEGIN

class PlayerBar;
class VSPlayerInfo;
class UIOperationalLayer;
class UIOperationalHookLayer;

class PlayerWrapperInBasket
{
	typedef QPointer<QWidget> QWidgetPtr;
	typedef QPointer<PlayerBar> PlayerBarPtr;
	typedef QPointer<UIOperationalHookLayer> OperationalHookLayerPtr;

public:
	PlayerWrapperInBasket() = default;
	PlayerWrapperInBasket(PlayerBar* player_bar, QWidget* player_turn_mask, UIOperationalHookLayer* operational_hook_layer = nullptr);

	void					ShowPlayeBar();
	void					HidePlayeBar();
	void					SetPlayeBarVisible(bool visible);
	
	void					SetPlayerInfo(const VSPlayerInfo&);

	void					ShowTurnMask();
	void					HideTurnMask();
	void					SetTurnMaskVisible(bool visible);

	void					SetGeometryOfTurnMask(const QRect&);

	void					SetGeometryOfOperationalHookLayer(const QRect&);
	void					AssociateOperationalLayer(UIOperationalLayer* operational_layer);

	void					Reset(PlayerBar* player_bar, QWidget* player_turn_mask, UIOperationalHookLayer* operational_hook_layer = nullptr);

private:
	PlayerBarPtr			player_bar_;
	QWidgetPtr				player_turn_mask_;
	OperationalHookLayerPtr operational_hook_layer_;
};

GAME_NAMESPACE_END

#endif