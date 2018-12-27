#ifndef __GAME_VIEW_CHESS_CHESS_GAME_BOARD_WRAPPER_H__
#define __GAME_VIEW_CHESS_CHESS_GAME_BOARD_WRAPPER_H__
#include <functional>
#include <memory>
#include <QPointer>
#include "game/view/base/game_board_wrapper_ui.h"

GAME_NAMESPACE_BEGIN

class GridIndex;
class UIChessGameBoard;
class UIAnimationLayer;

class UIChessGameBoardWrapper : public UIGameBoardWrapper
{
	Q_OBJECT

	class OperationalDelegate;
	typedef std::unique_ptr<OperationalDelegate> OperationalDelegatePtr;
	
public:
	static UIChessGameBoardWrapper* Create(ClientType client_type, UsageType usage_type, QWidget* parent);

	~UIChessGameBoardWrapper();

	// override GameBoardWrapper
	virtual void					HandleMasterCommandSpecially(const CommandSharedPtr&) override;

protected:
	explicit UIChessGameBoardWrapper(ClientType client_type, UsageType usage_type, QWidget* parent = nullptr);

	virtual void					Init() override;

	virtual GameBoardVector			CreateGameBoards() override;
	virtual UIChessBasketBase*		CreateChessBasket() override;
	virtual UIOperationalLayer*		InnerCreateOperationalLayer(QWidget* presentation_layer) override;

private:
	UIChessGameBoard*				game_board_{ nullptr };
	UIAnimationLayer*				animation_layer_{ nullptr };

	OperationalDelegatePtr			operational_delegate_{ nullptr };
};

typedef QPointer<UIChessGameBoardWrapper> UIChessGameBoardWrapperPtr;
extern UIChessGameBoardWrapperPtr CurrentChessBoardWrapper;

GAME_NAMESPACE_END

#endif