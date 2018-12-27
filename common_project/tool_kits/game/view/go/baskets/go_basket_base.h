#ifndef __GAME_VIEW_GO_BASKETS_GO_BASKET_BASE_H__
#define __GAME_VIEW_GO_BASKETS_GO_BASKET_BASE_H__
#include "game/model/base/chess/chess_info.h"
#include "game/view/base/chess_basket/chess_basket_base.h"

GAME_NAMESPACE_BEGIN

class UIGoColorBasket;

class UIGoBasketBase : public UIChessBasketBase
{
	Q_OBJECT

public:
	explicit UIGoBasketBase(QWidget* parent = nullptr);

	void				FixChessId(const int grid, const ChessId chess_id);
	void				CancelFixedChessId();
	inline ChessId		ChessIdFixed() const { return chess_id_fixed_; }

	virtual void		SwitchToGameBoard(GameBoardType board_type) override;

	virtual void		SetAutoTakingPieces(bool) = 0;

protected:
	virtual void		Init() override;

	virtual void		resizeEvent(QResizeEvent *) override;

	virtual void		OnColorBasketClicked(ChessId);
	
protected:
	UIGoColorBasket*	white_basket_{ nullptr };
	UIGoColorBasket*	black_basket_{ nullptr };

	ChessId				chess_id_fixed_{ ChessInfo::EmptyId };
	int					current_board_grids_{ 19 };
};

GAME_NAMESPACE_END

#endif