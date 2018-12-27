#ifndef __GAME_VIEW_GO_GO_PIECE_IN_BASKET_H__
#define __GAME_VIEW_GO_GO_PIECE_IN_BASKET_H__
#include "game/model/go/go_trait.h"
#include "game/view/base/chess_piece/chess_piece_with_trait.h"

GAME_NAMESPACE_BEGIN

class UIGoPieceBaseInBasket : public UIChessPieceBase
{
public:
	UIGoPieceBaseInBasket(QWidget* parent = nullptr) : UIChessPieceBase(parent) {}

	static UIGoPieceBaseInBasket* Create(const int grid, QWidget* parent);
};

template<int X>
class UIGoPieceInBasket : public UIChessPieceWithTrait<GoTraitX<X>, UIGoPieceBaseInBasket>
{
	typedef GoTraitX<X> MyTrait;

public:
	explicit UIGoPieceInBasket(QWidget* parent = nullptr) : UIChessPieceWithTrait(parent) {}

	virtual std::string ImagePath() const override
	{
		return MyTrait::MovingImagePathById(GetChessId());
	}
};

#define UI_GO_PIECE_IN_BASKET(X) \
class UIGoPiece##X##InBasket : public UIGoPieceInBasket<X> \
{ \
	Q_OBJECT \
public: \
	explicit UIGoPiece##X##InBasket(QWidget* parent = nullptr) : UIGoPieceInBasket<X>(parent) {} \
};

UI_GO_PIECE_IN_BASKET(9);
UI_GO_PIECE_IN_BASKET(13);
UI_GO_PIECE_IN_BASKET(19);

#undef UI_GO_PIECE_IN_BASKET

inline UIGoPieceBaseInBasket* UIGoPieceBaseInBasket::Create(const int grid, QWidget* parent)
{
	UIGoPieceBaseInBasket* go_piece = nullptr;

	if (grid == 9) { go_piece = new UIGoPiece9InBasket(parent); }
	else if (grid == 13) { go_piece = new UIGoPiece13InBasket(parent); }
	else if (grid == 19) { go_piece = new UIGoPiece19InBasket(parent); }

	return go_piece;
}

GAME_NAMESPACE_END

#endif