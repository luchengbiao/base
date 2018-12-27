#ifndef __GAME_VIEW_GO_GO_GAME_BOARD_UI_H__
#define __GAME_VIEW_GO_GO_GAME_BOARD_UI_H__
#include <QPointer>
#include "base/callback/callback.h"
#include "game/model/base/grid/grid_index.h"
#include "game/model/base/record/game_record.h"
#include "game/model/go/go_trait.h"
#include "game/view/base/game_board_base_ui.h"
#include "game/view/go/go_piece/go_piece.h"

GAME_NAMESPACE_BEGIN

class GoGameBoard;
class UIScaleLineLayer;

class UIGoGameBoard : public UIGameBoardBase
{
	Q_OBJECT

public:
	static const int MaxGrid{ 19 };

public:
	static UIGoGameBoard*		Create(int grid, const GameRecordsPtr& all_games_to_be_cached, QWidget* parent);

	// override UIGameBoardBase
	virtual GameBoardType		BoardType() const override { return board_type_; }
	virtual	int					RowsOfBoard() const override { return grid_; }
	virtual	int					ColumnsOfBoard() const override { return grid_; }
	virtual bool				IsRuleBased() const override;
	virtual int					GridWidth() const override { return InnerGridWidth(); }
	virtual QPoint				PositionByGridIndex(const GridIndex& grid_index) const override;
	virtual UIChessPieceBase*	ChessPieceAt(const GridIndex& grid) const override { return InnerChessPieceAt(grid.X(), grid.Y()); }
	virtual void				SetScaleLineVisible(bool visible);
	virtual void				HandleMasterCommandSpecially(const CommandSharedPtr& cmd_ptr) override;
	virtual void				RefreshBoardWithMaster() override;

	GoGameBoard*				LocalGameBoard() const;

	// override UIAnimationDelegate
	virtual QPixmap				MovingPixmapOfChessId(ChessId) override;
	virtual	QPoint				GlobalPositionOfGridCenterInBoard(const GridIndex&) override;
	virtual	QPoint				GlobalPositionOfChessCenterInBasket(ChessId) override;
	virtual void				HideChessInBoardGrid(const GridIndex&) override;
	virtual int					MilliSecondsOfMovingAnimation() override;

	inline static QPoint		CorrectionForChessPosition() { return QPoint(1, 2); }

protected:
	explicit UIGoGameBoard(int grid, QWidget* parent);

	virtual void				InitModel(const GameRecordsPtr& all_games_to_be_cached);
	virtual void				InitUI();

	virtual void				OnOperationalLayerAssociated() override;
	virtual	void				OnLayoutChanged() override;

	void						CreateChesses();
	void						CreateScaleLine();

	UIGoPieceBase*				InnerChessPieceAt(int x, int y) const { return (x >= 0 && x < grid_ && y >= 0 && y < grid_) ? chesses_[x][y] : nullptr; }

	template<typename Callable>
	void						ForeachChess(Callable&& callable);

	inline QPoint				GridOffsetFromOrigin() const { return GoTrait::GoBoardInfoByGrid(grid_).offset_; };

	void						UpdateChessesFromModel(const GameBoardPtr& model);
	void						UpdateChessesFromModelExceptVariations(const GameBoardPtr& model, const GameActionResult& result);

	virtual void				OnMasterDone(const GameActionResult&) override;
	virtual void				OnLocalDone(const GameActionResult&) override;

	virtual void				UpdateBasketWithModel(const GameBoardPtr& model) override;

	void						PlayMasterVariationOnAnimationLayer(const GameActionResult&);
	void						PlayLocalVariationOnAnimationLayer(const GameActionResult&);
	void						MarkLastGridsTakenFromModel(const GameBoardPtr& model);

	GoGameBoard*				MasterGameBoard() const;

	inline int					InnerGridWidth() const { return GoTrait::GoBoardInfoByGrid(grid_).grid_width_; }
	inline int					InnerChessWidth() const { return GoTrait::GoBoardInfoByGrid(grid_).chess_width_; }

private:
	const int					grid_{ 0 };
	const GameBoardType			board_type_{ GameBoardType::None };

	UIGoPieceBase*				chesses_[MaxGrid][MaxGrid]; // representing 9x9 13x13 19x19 with some waste of memory but high-efficiency of time

	UIScaleLineLayer*			scale_line_layer_{ nullptr };

	nbase::WeakCallbackFlag		chess_move_callback_flag_;
};

template<typename Callable>
void UIGoGameBoard::ForeachChess(Callable&& callable)
{
	for (int x = 0; x < grid_; ++x)
	{
		for (int y = 0; y < grid_; ++y)
		{
			callable(chesses_[x][y]);
		}
	}
}

GAME_NAMESPACE_END

#endif