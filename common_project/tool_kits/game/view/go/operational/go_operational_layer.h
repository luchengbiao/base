#ifndef __GAME_VIEW_GO_OPERATIONAL_GO_OPERATIONAL_LAYER_H__
#define __GAME_VIEW_GO_OPERATIONAL_GO_OPERATIONAL_LAYER_H__
#include <memory>
#include <vector>
#include <QPointer>
#include "game/view/base/operational_layer/operational_layer.h"
#include "game/view/go/operational/go_operational_delegate.h"

GAME_NAMESPACE_BEGIN

class UIGoOperationalLayer : public UIOperationalLayer
{
	Q_OBJECT

	typedef std::map<GridIndex, QRect>	GridRectInBoardMap;
	typedef std::map<ChessId, QRect>	ChessRectInBasketMap;
	typedef GoOperationalDelegate		MyOperationalDelegate;

	struct OperateInfo;
	typedef std::unique_ptr<OperateInfo> OperateInfoPtr;

	struct NogoFlagInfo;
	typedef std::vector<NogoFlagInfo> NogoFlagInfoVector;

public:
	explicit UIGoOperationalLayer(QWidget* parent, QWidget* presentation_layer = nullptr);
	~UIGoOperationalLayer();

	void					SetOperationalDelegate(MyOperationalDelegate* operational_delegate);

	virtual void			Interrupt() override;
	virtual void			Clear() override;
	virtual QPoint			CurrentGlobalPosOfChessFaceCenter() const override;

	virtual void			HandleMousePressEvent(const QPoint& local_pos, const QPoint& global_pos) override;
	virtual void			HandleMouseMoveEvent(const QPoint& local_pos, const QPoint& global_pos) override;
	virtual void			HandleMouseReleaseEvent(const QPoint& local_pos, const QPoint& global_pos) override;

protected:
	QPoint					LocalPosOfBoardGridCenter(const GridIndex& grid_index) { return mapFromGlobal(GlobalPosOfBoardGridCenter(grid_index)); }
	QPoint					LocalPosOfBasketChessCenter(ChessId chess_id) { return mapFromGlobal(GlobalPosOfBasketChessCenter(chess_id)); }

	QPoint					GlobalPosOfBoardGridCenter(const GridIndex& grid_index);
	QPoint					GlobalPosOfBasketChessCenter(ChessId chess_id);

	bool					GlobalPosInBoardGrid(const QPoint& global_pos, GridIndex* grid_index = nullptr, QRect* grid_global_rect = nullptr);
	bool					GlobalPosInBasket(const QPoint& global_pos, ChessId* chess_id = nullptr, QRect* grid_global_rect = nullptr);

	void					LazyGetGridRects();

	inline	void			IgnoreMouseEvent(bool ignore_mouse_event) { ignore_mouse_event_ = ignore_mouse_event; }
	inline	bool			IsMouseEventIgnored() const { return ignore_mouse_event_; }

	void					ShowNogoFlagAtGrid(const GridIndex& grid_index);
	bool					IsNogoFlagShownAtGrid(const GridIndex& grid_index);
	NogoFlagInfo*			GetOrCreateNogoFlag();

private:
	MyOperationalDelegate*	operational_delegate_{ nullptr };

	int						grid_{ 0 };

	GridRectInBoardMap		map_grid_global_rect_in_board_;

	ChessRectInBasketMap	map_chess_global_rect_in_basket_;

	NogoFlagInfoVector		nogo_flags_;

	OperateInfoPtr			ope_info_{ nullptr };

	bool					ignore_mouse_event_{ false };

	bool					is_trying_to_move_chess_{ false };
};

GAME_NAMESPACE_END

#endif