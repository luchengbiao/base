#ifndef __GAME_VIEW_CHESS_OPERATIONAL_UI_CHESS_OPERATIONAL_LAYER_H__
#define __GAME_VIEW_CHESS_OPERATIONAL_UI_CHESS_OPERATIONAL_LAYER_H__
#include <memory>
#include <QPointer>
#include "game/view/base/chess_face_fwd.h"
#include "game/view/base/operational_layer/operational_layer.h"
#include "game/view/chess/operational/chess_operational_delegate.h"

GAME_NAMESPACE_BEGIN

class UIArrowLine;
class UIChessPathMarkerLayer;

class UIChessOperationalLayer : public UIOperationalLayer
{
	Q_OBJECT

	typedef std::map<GridIndex, QRect>	GridRectInBoardMap;
	typedef std::map<ChessId, QRect>	ChessRectInBasketMap;
	typedef ChessOperationalDelegate	MyOperationalDelegate;

	struct OperateInfo;
	typedef std::unique_ptr<OperateInfo> OperateInfoPtr;

public:
	explicit UIChessOperationalLayer(QWidget* parent, QWidget* presentation_layer = nullptr);
	~UIChessOperationalLayer();

	void					SetOperationalDelegate(MyOperationalDelegate* operational_delegate);

	virtual void			Interrupt() override;
	virtual void			Clear() override;
	virtual QPoint			CurrentGlobalPosOfChessFaceCenter() const override;

	virtual void			HandleMousePressEvent(const QPoint& local_pos, const QPoint& global_pos) override;
	virtual void			HandleMouseMoveEvent(const QPoint& local_pos, const QPoint& global_pos) override;
	virtual void			HandleMouseReleaseEvent(const QPoint& local_pos, const QPoint& global_pos) override;

protected:
	void					HandleMouseReleaseEvent_ChessIdBeingDragged(const QPoint& local_pos, const QPoint& global_pos);
	void					HandleMouseReleaseEvent_NoChessIdBeingDragged(const QPoint& local_pos, const QPoint& global_pos);
	
	QPoint					LocalPosOfBoardGridCenter(const GridIndex& grid_index) { return mapFromGlobal(GlobalPosOfBoardGridCenter(grid_index)); }
	QPoint					LocalPosOfBasketChessCenter(ChessId chess_id) { return mapFromGlobal(GlobalPosOfBasketChessCenter(chess_id)); }

	QPoint					GlobalPosOfBoardGridCenter(const GridIndex& grid_index);
	QPoint					GlobalPosOfBasketChessCenter(ChessId chess_id);

	bool					GlobalPosInBoardGrid(const QPoint& global_pos, GridIndex* grid_index = nullptr, QRect* grid_global_rect = nullptr);
	bool					GlobalPosInBoardGridWithValidChess(const QPoint& global_pos, ChessId* chess_id = nullptr, GridIndex* grid_index = nullptr, QRect* grid_global_rect = nullptr);
	bool					GlobalPosInBasket(const QPoint& global_pos, ChessId* chess_id = nullptr, QRect* grid_global_rect = nullptr);

	void					LazyGetGridRects();
	void					LazyCreateChessUi();

	void					ShowChessFaceInMovingState(ChessId chess_id);
	void					ShowChessFaceInSelectedState(ChessId chess_id, const GridIndex& grid_index);
	void					MoveChessFaceCenterToPos(const QPoint& local_pos);

	void					InvalidateChessSelected();
	void					InvalidateChessBeingDragged();

	void					ShowPathMarker(const std::map<GridIndex, bool>& vec_grid_in_board);
	void					ShowPathMarker();
	void					HidePathMarker();

	void					ZOrderUi();

	bool					TryToMoveChess(ChessId chess_id, const GridIndex& grid_from, const GridIndex& grid_to);

	void					InnerClear();

	inline	void			IgnoreMouseEvent(bool ignore_mouse_event) { ignore_mouse_event_ = ignore_mouse_event; }
	inline	bool			IsMouseEventIgnored() const { return ignore_mouse_event_; }

private:
	MyOperationalDelegate*	operational_delegate_{ nullptr };

	GridRectInBoardMap		map_grid_global_rect_in_board_;

	ChessRectInBasketMap	map_chess_global_rect_in_basket_;

	// presentation_layer_ may be from the outside, use QPointer to guard
	// the parent of chess_face_ and arrow_line_ is presentation_layer_
	QPointer<QWidget>		presentation_layer_{ nullptr };
	QPointer<ChessFace>		chess_face_{ nullptr };
	QPointer<UIArrowLine>	arrow_line_{ nullptr };

	UIChessPathMarkerLayer*	path_marker_layer_{ nullptr };
	QColor					path_marker_normal_color_{ 245, 183, 95, 255 };
	QColor					path_marker_captured_color_{ 245, 183, 95, 255 };

	OperateInfoPtr			ope_info_{ nullptr };

	bool					ignore_mouse_event_{ false };
};

GAME_NAMESPACE_END

#endif