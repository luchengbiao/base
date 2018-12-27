#ifndef __GAME_VIEW_CHESS_BASKETS_CHESS_BASKET_O2O_H__
#define __GAME_VIEW_CHESS_BASKETS_CHESS_BASKET_O2O_H__
#include <unordered_map>
#include "game/view/base/chess_basket/chess_basket_base.h"
#include "game/view/base/chess_face_fwd.h"

namespace Ui { class ChessBasketO2O; }

GAME_NAMESPACE_BEGIN

class UIChessPieceInBasket;

class UIChessBasketO2O : public UIChessBasketBase
{
	Q_OBJECT

	typedef std::unordered_map<ChessId, UIChessPieceInBasket*>	MyChessPieces;
	typedef std::unique_ptr<Ui::ChessBasketO2O> MyUiPtr;

public:
	explicit UIChessBasketO2O(QWidget* parent);

	virtual void	SetBasketEnabled(bool enabled) override;

	virtual	QPoint	GlobalPositionOfChessCenter(ChessId chess_id) const override;
	virtual std::map<ChessId, QRect> ChessesGlobalRect() const override;

	virtual void	SetGameVSInfo(GameMode game_mode,
								  const VSPlayerInfo* white_player = nullptr,
								  const VSPlayerInfo* black_player = nullptr,
								  const PlayerSide* current_side = nullptr) override;

	virtual void	Reset() override;

	static QSize	InitialSize() { return QSize(200, 518); }

private:
	virtual void	Init() override;

	void			SetDisabledFlagVisible(bool visible);

	virtual void	resizeEvent(QResizeEvent *) override;

private:
	MyUiPtr			ui_;
	MyChessPieces	chess_pieces_;
};

GAME_NAMESPACE_END

#endif