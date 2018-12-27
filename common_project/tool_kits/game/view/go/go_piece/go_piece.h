#ifndef __GAME_VIEW_GO_GO_PIECE_GO_PIECE_H__
#define __GAME_VIEW_GO_GO_PIECE_GO_PIECE_H__
#include "game/model/go/go_trait.h"
#include "game/view/base/chess_face_impl.h"
#include "game/view/base/chess_piece/chess_piece_base.h"
#include "game/view/base/chess_piece/chess_piece_grid.h"
#include "game/view/base/chess_piece/chess_piece_with_mixins.h"
#include "game/view/base/chess_piece/chess_piece_with_trait.h"
#include "game/view/go/go_piece/go_piece_flag.h"
#include "game/view/go/go_piece/go_piece_index.h"

GAME_NAMESPACE_BEGIN

class UIGoPieceBase : public UIChessPieceWithMixins<UIChessPieceBase, ChessPieceGrid, GoPieceIndex<UIGoPieceBase>, GoPieceFlag<UIGoPieceBase>>
{
public:
	UIGoPieceBase(QWidget* parent = nullptr) : UIChessPieceWithMixins(parent) {}

	static UIGoPieceBase* Create(const int grid, QWidget* parent);

	virtual void Highlight() { /*do nothing*/ }

	// for index label
	inline QWidget* ParentWidgetOfIndexLabel() { return face_; }
	inline QPoint OffsetFromParentCenterOfIndexLabel() { return QPoint(0, -1); }
	inline QString QssObjectNameOfIndexLabel() const { return GetChessId() == GoTrait::IdOfBlackPiece ? "game_lb_11_ffffff" : "game_lb_11_333333"; }

	// for flag widget
	inline QWidget* ParentWidgetOfFlagWidget() { return face_; }
	virtual QPoint OffsetFromParentCenterOfFlagWidget() = 0;
};

template<int X>
class UIGoPiece : public UIChessPieceWithTrait<GoTraitX<X>, UIGoPieceBase>
{
	typedef GoTraitX<X> MyTrait;

public:
	explicit UIGoPiece(QWidget* parent = nullptr) : UIChessPieceWithTrait(parent) {}

	virtual std::string ImagePath() const override
	{
		return MyTrait::NormalImagePathById(GetChessId());
	}

	virtual QPoint OffsetFromParentCenterOfFlagWidget() override
	{
		auto radius = MyTrait::GoBoardInfoByGrid(X).grid_width_ * 0.3535f + 1.5f; // 0.3535 ~= 0.5 * sin45
		return QPoint(-radius, -radius);
	}
};

#define UI_GO_PIECE(X) \
class UIGoPiece##X : public UIGoPiece<X> \
{ \
	Q_OBJECT \
public: \
	explicit UIGoPiece##X(QWidget* parent = nullptr) : UIGoPiece<X>(parent) {} \
};

UI_GO_PIECE(9);
UI_GO_PIECE(13);
UI_GO_PIECE(19);

#undef UI_GO_PIECE

inline UIGoPieceBase* UIGoPieceBase::Create(const int grid, QWidget* parent)
{
	UIGoPieceBase* go_piece = nullptr;

	if (grid == 9) { go_piece = new UIGoPiece9(parent); }
	else if (grid == 13) { go_piece = new UIGoPiece13(parent); }
	else if (grid == 19) { go_piece = new UIGoPiece19(parent); }

	return go_piece;
}

GAME_NAMESPACE_END

#endif