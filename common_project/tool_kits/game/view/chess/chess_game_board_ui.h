#ifndef __GAME_VIEW_CHESS_CHESS_GAME_BOARD_UI_H__
#define __GAME_VIEW_CHESS_CHESS_GAME_BOARD_UI_H__
#include "base/callback/callback.h"
#include "game/model/base/grid/grid_index.h"
#include "game/public/enums/game_over_type.h"
#include "game/view/base/game_board_base_ui.h"
#include "game/view/chess/chess_piece.h"

GAME_NAMESPACE_BEGIN

class ChessGameBoard;
class UIScaleLineLayer;

class UIChessGameBoard : public UIGameBoardBase
{
	Q_OBJECT

public:
	static const int Grid{ 8 };

public:
	static UIChessGameBoard*	Create(const GameRecordsPtr& all_games_to_be_cached, QWidget* parent);

	// override UIGameBoardBase
	virtual GameBoardType		BoardType() const override { return GameBoardType::Chess; }
	virtual	int					RowsOfBoard() const override { return Grid; }
	virtual	int					ColumnsOfBoard() const override { return Grid; }
	virtual bool				IsRuleBased() const override;
	virtual int					GridWidth() const override { return 58; }
	virtual QPoint				PositionByGridIndex(const GridIndex& grid_index) const override;
	virtual UIChessPieceBase*	ChessPieceAt(const GridIndex& grid) const override { return InnerChessPieceAt(grid.X(), grid.Y()); }
	virtual void				SetScaleLineVisible(bool visible);
	virtual void				HandleMasterCommandSpecially(const CommandSharedPtr& cmd_ptr) override;
	virtual bool				TryToMoveChessLocally(ChessId chess_id, const GridIndex& grid_from, const GridIndex& grid_to, std::string* failed_info = nullptr) override;
	virtual void				Reset() override;
	virtual void				RefreshBoardWithMaster() override;

	ChessGameBoard*				LocalGameBoard() const;
	bool						IsLocalGameOver(GameOverType* type = nullptr, QString* hint = nullptr);

	// override UIAnimationDelegate
	virtual QPixmap				MovingPixmapOfChessId(ChessId) override;
	virtual	QPoint				GlobalPositionOfGridCenterInBoard(const GridIndex&) override;
	virtual	QPoint				GlobalPositionOfChessCenterInBasket(ChessId) override;
	virtual void				HideChessInBoardGrid(const GridIndex&) override;
	virtual int					MilliSecondsOfMovingAnimation() override;

protected:
	explicit UIChessGameBoard(QWidget* parent = nullptr);

	virtual void				InitModel(const GameRecordsPtr& all_games_to_be_cached);
	virtual void				InitUI();

	virtual void				OnOperationalLayerAssociated() override;
	virtual	void				OnLayoutChanged() override;

	void						CreateChesses();
	void						CreateScaleLine();

	UIChessPiece*				InnerChessPieceAt(int x, int y) const { return (x >= 0 && x < Grid && y >= 0 && y < Grid) ? chesses_[x][y] : nullptr; }

	template<typename Callable>
	void						ForeachChess(Callable&& callable);

	inline QPoint				GridOffsetFromOrigin() const { return QPoint(26, 24); };

	bool						IsGameOverOfModel(const GameBoardPtr& model, GameOverType* type = nullptr, QString* hint = nullptr);
	void						TryToHintGameSituationFromModel(const GameBoardPtr& model);
	void						HintGameOver(const QString& hint);
	void						HintChecker(const QString& hint);

	void						ShowLastGridMotionFromModel(const GameBoardPtr& model);
	void						UpdateChessesFromModel(const GameBoardPtr& model);
	void						UpdateChessesFromModelExceptVariations(const GameBoardPtr& model, const GameActionResult& result);

	virtual void				OnMasterDone(const GameActionResult&) override;
	virtual void				OnLocalDone(const GameActionResult&) override;

	void						PlayMasterVariationOnAnimationLayer(const GameActionResult&);
	void						PlayLocalVariationOnAnimationLayer(const GameActionResult&);

	ChessGameBoard*				MasterGameBoard() const;

	bool						HandleCastlingBeforeMovingChessLocally(ChessId chess_id, const GridIndex& grid_from, GridIndex& grid_to);
	ChessId						HandlePromotionBeforeMovingChessLocally(ChessId chess_id, const game::GridIndex& grid_from, game::GridIndex& grid_to, bool* interrupted);

private:
	UIChessPiece*				chesses_[Grid][Grid]; // 8 * 8

	UIScaleLineLayer*			scale_line_layer_{ nullptr };

	nbase::WeakCallbackFlag		chess_move_callback_flag_;
};

template<typename Callable>
void UIChessGameBoard::ForeachChess(Callable&& callable)
{
	for (int x = 0; x < Grid; ++x)
	{
		for (int y = 0; y < Grid; ++y)
		{
			callable(chesses_[x][y]);
		}
	}
}

GAME_NAMESPACE_END

#endif