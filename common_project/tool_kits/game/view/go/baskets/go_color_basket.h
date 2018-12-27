#ifndef __GAME_VIEW_GO_GO_COLOR_BASKET_H__
#define __GAME_VIEW_GO_GO_COLOR_BASKET_H__
#include <QPointer>
#include <QWidget>
#include "game/model/base/chess/chess_info.h"
#include "game/model/go/go_trait.h"

class WidgetWithPixmap;

GAME_NAMESPACE_BEGIN

class UIGoPieceBaseInBasket;

class UIGoColorBasket : public QWidget
{
	Q_OBJECT

	typedef QPointer<UIGoPieceBaseInBasket> MyGoPiecePtr;

public:
	explicit UIGoColorBasket(GoTrait::Color color, QWidget* parent = nullptr);

	inline static QPoint	PosOfChessArea() { return QPoint(12, 0); }
	inline static QSize		SizeOfChessArea() { return QSize(125, 132); }

	QRect					ChessGlobalArea() const;
	QPoint					GlobalPosOfChessAreaCenter() const;

	void					ShowDisabledFlag();
	void					HideDisabledFlag();
	void					SetDisabledFlagVisible(bool visible);

	void					ShowFixedChess(const int grid);
	void					HideFixedChesses();

	void					LazyCreateGoPieces(QWidget* parent);

	void					AdjustGeometryOfGoPieces();

signals:
	void					Clicked();

private:
	void					Init();

	UIGoPieceBaseInBasket*	CreateGoPiece(const int grid, QWidget* parent);

	virtual void			mouseReleaseEvent(QMouseEvent*) override;

private:
	const GoTrait::Color	color_;

	WidgetWithPixmap*		basket_{ nullptr };
	WidgetWithPixmap*		disabled_flag_{ nullptr };

	MyGoPiecePtr			go_piece_9_{ nullptr };
	MyGoPiecePtr			go_piece_13_{ nullptr };
	MyGoPiecePtr			go_piece_19_{ nullptr };
};

GAME_NAMESPACE_END

#endif