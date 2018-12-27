#include "chess_operational_layer.h"
#include <chrono>
#include <QMouseEvent>
#include "common/macros/assert.h"
#include "game/view/arrow_line/arrow_line.h"
#include "game/view/base/chess_face_impl.h"
#include "chess_path_marker_layer.h"

using namespace std::chrono;

GAME_NAMESPACE_BEGIN

int MillSeconds_To_Simulate_DoubleClick{ 250 };

struct UIChessOperationalLayer::OperateInfo
{
	typedef time_point<system_clock> MyTimePoint;

	ChessId			chess_id_being_dragged_{ ChessInfo::EmptyId };
	GridIndex		grid_from_;
	GridIndex		grid_current_;
	QRect			current_grid_global_rect_;

	ChessId			chess_id_selected_{ ChessInfo::EmptyId };
	GridIndex		grid_selected_; // grid index which is selected in last mouse-release event

	MyTimePoint		time_when_chess_selected_{ high_resolution_clock::now() };

	bool			mouse_is_moved_{ false };

	inline void		SetChessIdBeingDragged(ChessId chess_id, const GridIndex& grid_index) 
	{
		chess_id_being_dragged_ = chess_id; 
		grid_from_ = grid_index;
	}
	inline void		SetGridCurrent(const GridIndex& grid_index) { grid_current_ = grid_index; }
	inline void		SetCurrentGridGlobalRect(const QRect& rect) { current_grid_global_rect_ = rect; }

	inline void		SetChessIdSelected(ChessId chess_id, const GridIndex& grid_index) 
	{
		chess_id_selected_ = chess_id; 
		grid_selected_ = grid_index;  
		time_when_chess_selected_ = high_resolution_clock::now();
	}
	inline const MyTimePoint& TimeWhenChessSelected() const { return time_when_chess_selected_; }

	inline void		SetMouseIsMoved(bool mouse_is_moved) { mouse_is_moved_ = mouse_is_moved; }

	inline ChessId	ChessIdBeingDragged() const { return chess_id_being_dragged_; }
	inline const GridIndex& GridFrom() const { return grid_from_; }
	inline const GridIndex& GridCurrent() const { return grid_current_; }
	inline const QRect CurrentGridGlobalRect() const { return current_grid_global_rect_; }

	inline ChessId ChessIdSelected() const { return chess_id_selected_; }
	inline const GridIndex& GridSelected() const { return grid_selected_; }

	inline bool		MouseIsMoved() const { return mouse_is_moved_; }

	inline bool		HasChessIdBeingDragged() const { return ChessIdIsNotEmpty(chess_id_being_dragged_); }
	inline bool		HasChessIdSelected() const { return ChessIdIsNotEmpty(chess_id_selected_); }

	inline void		InvalidateChessBeingDragged() { chess_id_being_dragged_ = ChessInfo::EmptyId; grid_from_ = GridIndex::InvalidGrid; }
	inline void		InvalidateChessSelected() { chess_id_selected_ = ChessInfo::EmptyId; grid_selected_ = GridIndex::InvalidGrid; }
};

UIChessOperationalLayer::UIChessOperationalLayer(QWidget* parent, QWidget* presentation_layer)
: UIOperationalLayer(parent)
, presentation_layer_(presentation_layer)
, ope_info_(new OperateInfo)
{}

UIChessOperationalLayer::~UIChessOperationalLayer() = default;

void UIChessOperationalLayer::SetOperationalDelegate(MyOperationalDelegate* operational_delegate)
{
	operational_delegate_ = operational_delegate;
}

void UIChessOperationalLayer::Interrupt()
{
	this->IgnoreMouseEvent(true);

	this->InnerClear();
}

void UIChessOperationalLayer::Clear()
{
	this->InnerClear();
}

void UIChessOperationalLayer::InnerClear()
{
	this->InvalidateChessSelected();

	this->InvalidateChessBeingDragged();

	if (chess_face_)
	{
		chess_face_->hide();
	}

	if (arrow_line_)
	{
		arrow_line_->hide();
	}

	this->HidePathMarker();
}

void UIChessOperationalLayer::HandleMousePressEvent(const QPoint& local_pos, const QPoint& global_pos)
{
	D_ASSERT(operational_delegate_, L"operational_delegate_ should not be null");

	do 
	{
		this->IgnoreMouseEvent(!operational_delegate_->TestToOperateOnBoardNow(global_pos));

		if (this->IsMouseEventIgnored()) { break; }

		this->LazyGetGridRects();

		ope_info_->InvalidateChessBeingDragged();
		ope_info_->SetMouseIsMoved(false);

		ChessId chess_id;
		QRect grid_global_rect;

		if (GlobalPosInBasket(global_pos, &chess_id, &grid_global_rect))
		{
			ope_info_->SetChessIdBeingDragged(chess_id, GridIndex::InvalidGrid);
		}
		else
		{
			GridIndex grid_index;
			if (GlobalPosInBoardGridWithValidChess(global_pos, &chess_id, &grid_index, &grid_global_rect))
			{
				ope_info_->SetChessIdBeingDragged(chess_id, grid_index);
			}
		}

		if (ope_info_->HasChessIdBeingDragged())
		{
			if (!operational_delegate_->TestToOperateOnThisChess(ope_info_->GridFrom(), ope_info_->ChessIdBeingDragged()))
			{
				ope_info_->InvalidateChessBeingDragged();
				break;
			}
		}
		else
		{
			break;
		}

		const auto grid_from = ope_info_->GridFrom();
		ope_info_->SetCurrentGridGlobalRect(grid_global_rect);
		ope_info_->SetGridCurrent(grid_from);

		if (ope_info_->HasChessIdSelected()) // if has selected a chess in last mouse press-move-release event loop
		{
			if (ope_info_->GridSelected() != grid_from)
			{
				ope_info_->InvalidateChessBeingDragged();

				break;
			}
		}

		this->LazyCreateChessUi();

		auto grid_from_center = grid_from.IsValid() ? GlobalPosOfBoardGridCenter(grid_from) : GlobalPosOfBasketChessCenter(ope_info_->ChessIdBeingDragged());

		if (arrow_line_)
		{
			arrow_line_->SetStartAndEndPosition(grid_from_center, global_pos);
		}

		this->ShowChessFaceInSelectedState(ope_info_->ChessIdBeingDragged(), grid_from);

		if (grid_from.IsValid())
		{
			auto grids = operational_delegate_->GridsReachableFrom(grid_from);
			this->ShowPathMarker(grids);
		}
	} while (false);
}

void UIChessOperationalLayer::HandleMouseMoveEvent(const QPoint& local_pos, const QPoint& global_pos)
{
	if (IsMouseEventIgnored() || !ope_info_->HasChessIdBeingDragged()) { return; }

	if (!ope_info_->MouseIsMoved()) // change from SelectedFace to MovingFace when first moved
	{
		ope_info_->SetMouseIsMoved(true);

		this->ShowChessFaceInMovingState(ope_info_->ChessIdBeingDragged());

		operational_delegate_->OnMouseWillMove(ope_info_->GridFrom(), ope_info_->ChessIdBeingDragged());

		if (arrow_line_) { arrow_line_->show(); }
	}

	this->MoveChessFaceCenterToPos(local_pos);

	if (arrow_line_) { arrow_line_->SetEndPosition(global_pos); }
	
	GridIndex grid_current = ope_info_->GridCurrent();

	if (grid_current.IsValid())
	{
		if (!ope_info_->CurrentGridGlobalRect().contains(global_pos))
		{
			operational_delegate_->UnhighlightGridInBoard(grid_current);

			QRect grid_global_rect;
			if (GlobalPosInBoardGrid(global_pos, &grid_current, &grid_global_rect))
			{
				operational_delegate_->HighlightGridInBoard(grid_current);

				ope_info_->SetGridCurrent(grid_current);
				ope_info_->SetCurrentGridGlobalRect(grid_global_rect);
			}
			else
			{
				ope_info_->SetGridCurrent(GridIndex::InvalidGrid);
				ope_info_->SetCurrentGridGlobalRect(QRect(global_pos, QSize(0, 0)));
			}
		}
	}
	else
	{
		QRect grid_global_rect;
		if (GlobalPosInBoardGrid(global_pos, &grid_current, &grid_global_rect))
		{
			operational_delegate_->HighlightGridInBoard(grid_current);

			ope_info_->SetGridCurrent(grid_current);
			ope_info_->SetCurrentGridGlobalRect(grid_global_rect);
		}
		else
		{
			ope_info_->SetCurrentGridGlobalRect(QRect(global_pos, QSize(0, 0)));
		}
	}
}

void UIChessOperationalLayer::HandleMouseReleaseEvent(const QPoint& local_pos, const QPoint& global_pos)
{
	if (IsMouseEventIgnored()) { return; }

	if (ope_info_->HasChessIdBeingDragged())
	{ 
		this->HandleMouseReleaseEvent_ChessIdBeingDragged(local_pos, global_pos);
	}
	else
	{
		this->HandleMouseReleaseEvent_NoChessIdBeingDragged(local_pos, global_pos);
	}
}

void UIChessOperationalLayer::HandleMouseReleaseEvent_ChessIdBeingDragged(const QPoint&, const QPoint&)
{
	if (ope_info_->GridCurrent().IsValid())
	{
		operational_delegate_->UnhighlightGridInBoard(ope_info_->GridCurrent());
	}

	if (arrow_line_)
	{
		arrow_line_->hide();
	}

	if (ope_info_->GridFrom() != ope_info_->GridCurrent())
	{
		this->TryToMoveChess(ope_info_->ChessIdBeingDragged(), ope_info_->GridFrom(), ope_info_->GridCurrent());
	}
	else
	{
		if (ope_info_->HasChessIdSelected()) // if a grid has already been selected, then un-select it
		{
			const auto chess_id_selected = ope_info_->ChessIdSelected();
			const auto grid_selected = ope_info_->GridSelected();
			const auto duration = duration_cast<milliseconds>(high_resolution_clock::now() - ope_info_->TimeWhenChessSelected()).count();

			if (ope_info_->GridCurrent() == grid_selected && duration <= MillSeconds_To_Simulate_DoubleClick) // simulate double-click to remove chess from board to basket
			{
				this->InnerClear();

				this->TryToMoveChess(chess_id_selected, grid_selected, GridIndex::InvalidGrid);
			}
			else
			{
				operational_delegate_->ShowChessInBoardGrid(ope_info_->GridFrom());
				this->HidePathMarker();
				if (chess_face_) { chess_face_->hide(); }

				ope_info_->InvalidateChessSelected();
			}
		}
		else
		{
			ope_info_->SetChessIdSelected(ope_info_->ChessIdBeingDragged(), ope_info_->GridCurrent());

			this->ShowChessFaceInSelectedState(ope_info_->ChessIdSelected(), ope_info_->GridSelected());
		}
	}
}

void UIChessOperationalLayer::HandleMouseReleaseEvent_NoChessIdBeingDragged(const QPoint&, const QPoint& global_pos)
{
	if (ope_info_->HasChessIdSelected())
	{
		const auto chess_id_selected = ope_info_->ChessIdSelected();
		const auto grid_selected = ope_info_->GridSelected();

		GridIndex grid_current;
		if (!GlobalPosInBoardGrid(global_pos, &grid_current))
		{
			grid_current = GridIndex::InvalidGrid;
		}

		this->InnerClear();

		if (grid_selected != grid_current && grid_current.IsValid())
		{
			bool moved = this->TryToMoveChess(chess_id_selected, grid_selected, grid_current);

			if (!moved)
			{
				auto chess_id_new_selected = ChessInfo::EmptyId;
				if (GlobalPosInBoardGridWithValidChess(global_pos, &chess_id_new_selected, &grid_current, nullptr))
				{
					ope_info_->SetChessIdSelected(chess_id_new_selected, grid_current);

					this->ShowChessFaceInSelectedState(ope_info_->ChessIdSelected(), ope_info_->GridSelected());
					auto grids = operational_delegate_->GridsReachableFrom(ope_info_->GridSelected());
					this->ShowPathMarker(grids);
				}
			}
		}
	}
}

QPoint UIChessOperationalLayer::GlobalPosOfBoardGridCenter(const GridIndex& grid_index)
{
	auto iter = map_grid_global_rect_in_board_.find(grid_index);

	return iter != map_grid_global_rect_in_board_.cend()? iter->second.center() : QPoint();
}

QPoint UIChessOperationalLayer::GlobalPosOfBasketChessCenter(ChessId chess_id)
{
	auto iter = map_chess_global_rect_in_basket_.find(chess_id);

	return iter != map_chess_global_rect_in_basket_.cend() ? iter->second.center() : QPoint();
}

bool UIChessOperationalLayer::GlobalPosInBoardGrid(const QPoint& global_pos, GridIndex* grid_index, QRect* grid_global_rect)
{
	for (const auto& pair : map_grid_global_rect_in_board_)
	{
		if (pair.second.contains(global_pos))
		{
			if (grid_index) { *grid_index = pair.first; }
			if (grid_global_rect) { *grid_global_rect = pair.second; }
			
			return true;
		}
	}
	
	return false;
}

bool UIChessOperationalLayer::GlobalPosInBoardGridWithValidChess(const QPoint& global_pos, ChessId* chess_id, GridIndex* grid_index, QRect* grid_global_rect)
{
	if (GlobalPosInBoardGrid(global_pos, grid_index, grid_global_rect))
	{
		if (operational_delegate_->GridContainsChessInBoard(*grid_index, chess_id))
		{
			return true;
		}
	}

	return false;
}

bool UIChessOperationalLayer::GlobalPosInBasket(const QPoint& global_pos, ChessId* chess_id, QRect* grid_global_rect)
{
	for (const auto& pair : map_chess_global_rect_in_basket_)
	{
		if (pair.second.contains(global_pos))
		{
			if (chess_id) { *chess_id = pair.first; }
			if (grid_global_rect) { *grid_global_rect = pair.second; }

			return true;
		}
	}

	return false;
}

void UIChessOperationalLayer::LazyGetGridRects()
{
	//if (map_grid_global_rect_in_board_.empty())
	{
		map_grid_global_rect_in_board_ = operational_delegate_->GridsGlobalRectInBoard();
	}

	//if (map_chess_global_rect_in_basket_.empty())
	{
		map_chess_global_rect_in_basket_ = operational_delegate_->ChessesGlobalRectInBasket();
	}
}

void UIChessOperationalLayer::LazyCreateChessUi()
{
	if (!chess_face_)
	{
		if (!presentation_layer_)
		{
			presentation_layer_ = this;
		}

		chess_face_ = new ChessFace(presentation_layer_);

		arrow_line_ = new UIArrowLine(presentation_layer_);
		arrow_line_->SetLineStyle(Qt::DotLine);

		arrow_line_->setFixedSize(this->size());
		
		this->ZOrderUi();
	}
}

void UIChessOperationalLayer::ShowChessFaceInMovingState(ChessId chess_id)
{
	if (chess_face_)
	{
		auto pixmap = operational_delegate_->MovingPixmapOfChessId(chess_id);

		chess_face_->setPixmap(pixmap);
		chess_face_->setFixedSize(pixmap.size());

		chess_face_->show();
	}
}

void UIChessOperationalLayer::ShowChessFaceInSelectedState(ChessId chess_id, const GridIndex& grid_index)
{
	if (chess_face_)
	{
		auto pixmap = operational_delegate_->SelectedPixmapOfChessId(chess_id);

		chess_face_->setPixmap(pixmap);
		chess_face_->setFixedSize(pixmap.size());

		chess_face_->show();
	}

	auto grid_center = grid_index.IsValid() ? GlobalPosOfBoardGridCenter(grid_index) : GlobalPosOfBasketChessCenter(chess_id);
	this->MoveChessFaceCenterToPos(mapFromGlobal(grid_center));

	if (grid_index.IsValid()) // in board
	{
		operational_delegate_->HideChessInBoardGrid(grid_index);
	}
}

void UIChessOperationalLayer::MoveChessFaceCenterToPos(const QPoint& local_pos)
{
	if (chess_face_)
	{
		chess_face_->move(local_pos.x() - chess_face_->width() * 0.5f, local_pos.y() - chess_face_->height() * 0.5f);
	}
}

void UIChessOperationalLayer::InvalidateChessSelected()
{
	if (ope_info_->HasChessIdSelected())
	{
		operational_delegate_->ShowChessInBoardGrid(ope_info_->GridSelected());
		ope_info_->InvalidateChessSelected();
	}
}

void UIChessOperationalLayer::InvalidateChessBeingDragged()
{
	if (ope_info_->HasChessIdBeingDragged())
	{
		operational_delegate_->ShowChessInBoardGrid(ope_info_->GridFrom());
		ope_info_->InvalidateChessBeingDragged();
	}
}

void UIChessOperationalLayer::ShowPathMarker(const std::map<GridIndex, bool>& vec_grid_in_board)
{
	if (!path_marker_layer_)
	{
		path_marker_layer_ = new UIChessPathMarkerLayer(this);

		path_marker_layer_->setFixedSize(this->size());

		this->ZOrderUi();
	}

	ChessId chess_id;

	if (vec_grid_in_board.empty())
	{
		//TODO: tell why no legal grids with rule checking

		path_marker_layer_->SetMarkerPoints(Point2ColorMap());
	}
	else
	{
		Point2ColorMap map_point_color;
		for (const auto& pair : vec_grid_in_board)
		{
			map_point_color.emplace(GlobalPosOfBoardGridCenter(pair.first), pair.second ? path_marker_captured_color_ : path_marker_normal_color_);
		}

		path_marker_layer_->SetMarkerPoints(map_point_color);
	}

	path_marker_layer_->show();
}

void UIChessOperationalLayer::ShowPathMarker()
{
	if (path_marker_layer_)
	{
		path_marker_layer_->show();
	}
}

void UIChessOperationalLayer::HidePathMarker()
{
	if (path_marker_layer_)
	{
		path_marker_layer_->hide();
	}
}

void UIChessOperationalLayer::ZOrderUi()
{
	if (path_marker_layer_)
	{
		path_marker_layer_->lower();
	}

	if (chess_face_)
	{
		chess_face_->raise();
	}

	if (arrow_line_)
	{
		arrow_line_->raise();
	}
}

bool UIChessOperationalLayer::TryToMoveChess(ChessId chess_id, const GridIndex& grid_from, const GridIndex& grid_to)
{
	return operational_delegate_->TryToMoveChessLocally(chess_id, grid_from, grid_to);
}

QPoint UIChessOperationalLayer::CurrentGlobalPosOfChessFaceCenter() const
{
	if (chess_face_)
	{
		return chess_face_->mapToGlobal(QPoint(chess_face_->width() * 0.5f, chess_face_->height() * 0.5f));
	}

	return this->mapToGlobal(QPoint(this->width() * 0.5f, this->height() * 0.5f));
}

GAME_NAMESPACE_END