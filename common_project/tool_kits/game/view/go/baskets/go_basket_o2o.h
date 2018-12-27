#ifndef __GAME_VIEW_GO_BASKETS_GO_BASKET_O2O_H__
#define __GAME_VIEW_GO_BASKETS_GO_BASKET_O2O_H__
#include "game/view/base/chess_face_fwd.h"
#include "game/view/go/baskets/go_basket_base.h"

namespace Ui { class GoBasketO2O; }

GAME_NAMESPACE_BEGIN

class PlayerBar;

class UIGoBasketO2O : public UIGoBasketBase
{
	Q_OBJECT

	typedef std::unique_ptr<Ui::GoBasketO2O> MyUiPtr;

public:
	explicit UIGoBasketO2O(QWidget* parent);

	virtual void		SetBasketEnabled(bool enabled) override;

	virtual	QPoint		GlobalPositionOfChessCenter(ChessId chess_id) const override;
	virtual std::map<ChessId, QRect> ChessesGlobalRect() const override;

	virtual void		SetAutoTakingPieces(bool) override;

	virtual void		SetGameVSInfo(GameMode game_mode,
									  const VSPlayerInfo* black_player = nullptr,
									  const VSPlayerInfo* white_player = nullptr,
									  const PlayerSide* current_side = nullptr) override;

	virtual void		Reset() override;

	static QSize		InitialSize() { return QSize(200, 518); }

private:
	virtual void		Init() override;

	virtual void		OnColorBasketClicked(ChessId);

	void				SetDisabledFlagVisible(bool visible);

private:
	MyUiPtr				ui_;
};

GAME_NAMESPACE_END

#endif