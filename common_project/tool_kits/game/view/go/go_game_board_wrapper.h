#ifndef __GAME_VIEW_GO_GO_GAME_BOARD_WRAPPER_UI_H__
#define __GAME_VIEW_GO_GO_GAME_BOARD_WRAPPER_UI_H__
#include <functional>
#include <memory>
#include <QPointer>
#include "game/model/go/go_trait.h"
#include "game/view/base/game_board_wrapper_ui.h"
#include "game/view/go/chess_id_generator/chess_id_generator_type.h"

GAME_NAMESPACE_BEGIN

class ChessIdGeneratorHelper;
class GridIndex;
class UIGoBasketBase;

class UIGoGameBoardWrapper : public UIGameBoardWrapper
{
	Q_OBJECT

	class OperationalDelegate;
	typedef std::unique_ptr<OperationalDelegate> OperationalDelegatePtr;

	typedef std::unique_ptr<ChessIdGeneratorHelper> ChessIdGeneratorHelperPtr;

public:
	static UIGoGameBoardWrapper*	Create(ClientType client_type, UsageType usage_type, QWidget* parent);

	~UIGoGameBoardWrapper();

	// override GameBoardWrapper
	virtual void					HandleMasterCommandSpecially(const CommandSharedPtr&) override;

	int								CurrentBoardGrids() const;

	ChessIdGeneratorType			CurrentChessIdGeneratorType() const;
	void							SwitchChessIdGenerator(ChessIdGeneratorType type);
	ChessId							NextChessIdToMoveIn();

protected:
	explicit UIGoGameBoardWrapper(ClientType client_type, UsageType usage_type, QWidget* parent = nullptr);

	virtual void					Init() override;

	virtual GameBoardVector			CreateGameBoards() override;
	virtual UIChessBasketBase*		CreateChessBasket() override;
	virtual UIOperationalLayer*		InnerCreateOperationalLayer(QWidget* presentation_layer) override;

	virtual void					ResetForRecovery() override;

private:
	UIGoBasketBase*					go_basket_{ nullptr };

	OperationalDelegatePtr			operational_delegate_{ nullptr };

	ChessIdGeneratorHelperPtr		chess_id_generator_{ nullptr };
};

typedef QPointer<UIGoGameBoardWrapper> UIGoGameBoardWrapperPtr;
extern UIGoGameBoardWrapperPtr CurrentGoBoardWrapper;

GAME_NAMESPACE_END

#endif