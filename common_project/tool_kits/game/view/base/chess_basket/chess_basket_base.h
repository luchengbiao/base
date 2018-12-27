#ifndef __GAME_VIEW_BASE_CHESS_BASKET_CHESS_BASKET_BASE_H__
#define __GAME_VIEW_BASE_CHESS_BASKET_CHESS_BASKET_BASE_H__
#include <map>
#include <QWidget>
#include "game/model/base/chess/chess_info.h"
#include "game/public/enums/game_board_type.h"
#include "game/public/enums/game_mode.h"
#include "game/public/enums/player_side.h"
#include "game/public/structs/vs_player_info/vs_player_info.h"
#include "game/view/base/chess_basket/player_wrapper_in_basket.h"
#include "protocol/command/base/command_fwd.h"

GAME_NAMESPACE_BEGIN

class UIOperationalLayer;
class UIOperationalHookLayer;

class UIChessBasketBase : public QWidget
{
	Q_OBJECT

	template<typename UIChessBasketT, typename... Args>
	friend UIChessBasketT* CreateChessBasket(Args&&... args);

	typedef QPointer<UIOperationalLayer> OperationalLayerPtr;

public:
	explicit UIChessBasketBase(QWidget* parent = nullptr);

	virtual void			SetBasketEnabled(bool enabled) = 0;

	virtual void			SwitchToGameBoard(GameBoardType board_type);
	inline GameBoardType	CurrentGameBoardType() const { return current_board_type_; }

	virtual	QPoint			GlobalPositionOfChessCenter(ChessId chess_id) const = 0;

	virtual std::map<ChessId, QRect> ChessesGlobalRect() const = 0;

	virtual void			SetGameVSInfo(GameMode game_mode, 
										  const VSPlayerInfo* black_player = nullptr,
										  const VSPlayerInfo* white_player = nullptr,
										  const PlayerSide* current_side = nullptr) = 0;
	
	virtual void			HandleMasterCommandSpecially(const CommandSharedPtr&) {}

	virtual void			Reset() = 0;

	// associate operational layer and delegate mouse event on chess to it later
	void					AssociateOperationalLayer(UIOperationalLayer*);

protected:
	virtual void			Init() {}

	inline bool				IsBasketEnabled() const { return basket_enabled_; }
	inline bool				IsBasketDisabled() const { return !basket_enabled_; }

	QWidget*				CreateTurnMask(const QString& qss_object_name);
	UIOperationalHookLayer* CreateOperationalHookLayer();

protected:
	bool					basket_enabled_{ true };
	GameBoardType			current_board_type_{ GameBoardType::None };
	GameMode				current_game_mode_{ GameMode::None };

	PlayerWrapperInBasket	black_side_wrapper_;
	PlayerWrapperInBasket	white_side_wrapper_;
};

template<typename UIChessBasketT, typename... Args>
UIChessBasketT* CreateChessBasket(Args&&... args)
{
	UIChessBasketBase* chess_basket = new (std::nothrow) UIChessBasketT(std::forward<Args>(args)...);

	if (chess_basket) { chess_basket->Init(); }

	return static_cast<UIChessBasketT*>(chess_basket);
}

GAME_NAMESPACE_END

#endif