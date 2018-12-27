#ifndef __GAME_VIEW_CHESS_BASKETS_CHESS_PIECE_IN_BASKET_H__
#define __GAME_VIEW_CHESS_BASKETS_CHESS_PIECE_IN_BASKET_H__
#include "game/model/chess/chess_trait.h"
#include "game/view/base/chess_piece/chess_piece_with_trait.h"

class WidgetWithPixmap;

GAME_NAMESPACE_BEGIN

class UIChessPieceInBasket : public QWidget
{
	Q_OBJECT

	typedef UIChessPieceWithTrait<ChessTrait> MyChessPiece;

public:
	explicit UIChessPieceInBasket(QWidget* parent = nullptr);

	void			SetChessId(const ChessId chess_id);
	ChessId			GetChessId() const;

	void			ShowDisabledFlag();
	void			HideDisabledFlag();
	void			SetDisabledFlagVisible(bool visible);

	QRect			GlobalRectOfChessPiece();
	QPoint			GlobalPositionOfChessPieceCenter();

private:
	void			Init();

	virtual void	resizeEvent(QResizeEvent *) override;

private:
	WidgetWithPixmap* chess_bk_{ nullptr };
	MyChessPiece*	  chess_piece_{ nullptr };
	WidgetWithPixmap* disabled_flag_{ nullptr };
};

GAME_NAMESPACE_END

#endif