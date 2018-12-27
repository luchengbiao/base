#include "go_operational_layer.h"
#include <chrono>
#include <limits>
#include <QMouseEvent>
#include "base/util/value_guard.h"
#include "common/macros/assert.h"
#include "common/uiutils/ui_utils.h"
#include "game/model/go/actions/go_action_error.h"
#include "game/public/literal_stock/literal_stock.h"
#include "game/view/base/chess_face_impl.h"

using namespace std::chrono;

GAME_NAMESPACE_BEGIN

extern int MillSeconds_To_Simulate_DoubleClick;

struct UIGoOperationalLayer::OperateInfo
{
	typedef time_point<system_clock> MyTimePoint;

	ChessId			chess_id_selected_{ ChessInfo::EmptyId };
	GridIndex		grid_selected_; // grid index which is selected in last mouse-release event

	MyTimePoint		time_when_chess_selected_{ high_resolution_clock::now() };

	inline void		SetChessIdSelected(ChessId chess_id, const GridIndex& grid_index)
	{
		chess_id_selected_ = chess_id;
		grid_selected_ = grid_index;
		time_when_chess_selected_ = high_resolution_clock::now();
	}

	inline ChessId				ChessIdSelected() const { return chess_id_selected_; }
	inline const GridIndex&		GridSelected() const { return grid_selected_; }
	inline const MyTimePoint&	TimeWhenChessSelected() const { return time_when_chess_selected_; }

	inline void					InvalidateChessSelected() { chess_id_selected_ = ChessInfo::EmptyId; grid_selected_ = GridIndex::InvalidGrid; }
};

typedef WidgetWithPixmap NogoFlagWidget;

struct UIGoOperationalLayer::NogoFlagInfo
{
	const int			grid_{ 0 };
	NogoFlagWidget*		flag_widget_{ nullptr };
	GridIndex			grid_index_;

	NogoFlagInfo(int grid, NogoFlagWidget* flag)
		: grid_(grid), flag_widget_(flag)
	{}
};

UIGoOperationalLayer::UIGoOperationalLayer(QWidget* parent, QWidget*)
: UIOperationalLayer(parent)
, ope_info_(new OperateInfo)
{}

UIGoOperationalLayer::~UIGoOperationalLayer() = default;

void UIGoOperationalLayer::SetOperationalDelegate(MyOperationalDelegate* operational_delegate)
{
	operational_delegate_ = operational_delegate;
}

void UIGoOperationalLayer::Interrupt()
{
	this->IgnoreMouseEvent(true);

	this->Clear();
}

void UIGoOperationalLayer::Clear()
{
	if (!is_trying_to_move_chess_) // is_trying_to_move_chess_ == true: triggered from mouseReleaseEvent actively
	{
		for (const auto& nogo_flag : nogo_flags_)
		{
			nogo_flag.flag_widget_->hide();
		}
	}
}

void UIGoOperationalLayer::HandleMousePressEvent(const QPoint&, const QPoint& global_pos)
{
	D_ASSERT(operational_delegate_, L"operational_delegate_ should not null");

	do 
	{
		this->IgnoreMouseEvent(!operational_delegate_->TestToOperateOnBoardNow(global_pos));

		if (this->IsMouseEventIgnored()) { break; }

		this->LazyGetGridRects();
	} while (false);
}

void UIGoOperationalLayer::HandleMouseMoveEvent(const QPoint&, const QPoint&)
{
	// do nothing
}

void UIGoOperationalLayer::HandleMouseReleaseEvent(const QPoint&, const QPoint& global_pos)
{
	if (IsMouseEventIgnored()) { return; }

	GridIndex grid_current;
	if (GlobalPosInBoardGrid(global_pos, &grid_current))
	{
		ChessId chess_id_current = ChessInfo::EmptyId;
		if (operational_delegate_->GridContainsChessInBoard(grid_current, &chess_id_current))
		{
			const auto chess_id_selected = ope_info_->ChessIdSelected();
			const auto grid_selected = ope_info_->GridSelected();
			const auto duration = duration_cast<milliseconds>(high_resolution_clock::now() - ope_info_->TimeWhenChessSelected()).count();

			if (grid_selected == grid_current &&
				chess_id_selected == chess_id_current &&
				duration <= MillSeconds_To_Simulate_DoubleClick) // simulate double-click to remove chess from board to basket
			{
				ope_info_->InvalidateChessSelected();

				bool ok = false;

				{
					nbase::ValueGuard<bool> guard(is_trying_to_move_chess_);
					is_trying_to_move_chess_ = true;

					ok = operational_delegate_->TryToMoveChessLocally(chess_id_current, grid_current, GridIndex::InvalidGrid);
				}

				if (ok) { this->Clear(); }
			}
			else
			{
				ope_info_->SetChessIdSelected(chess_id_current, grid_current);
			}
		}
		else
		{
			bool ok = false;
			std::string failed_info;

			{
				nbase::ValueGuard<bool> guard(is_trying_to_move_chess_);
				is_trying_to_move_chess_ = true;

				ok = operational_delegate_->TryToMoveChessLocally(operational_delegate_->NextChessIdToMoveIntoBoard(), GridIndex::InvalidGrid, grid_current, &failed_info);
			}

			if (ok)
			{
				this->Clear();
			}
			else
			{
				if (!failed_info.empty())
				{
					if (strcmp(failed_info.c_str(), go_actoin_error::ToGridIsANoGoPoint) == 0)
					{
						operational_delegate_->TipOnToast(QStringByKey(LK_NOGO_POINT));

						this->ShowNogoFlagAtGrid(grid_current);
					}
					else if (strcmp(failed_info.c_str(), go_actoin_error::ToGridIsKoPoint) == 0)
					{
						operational_delegate_->TipOnToast(QStringByKey(LK_KO_POINT));

						this->ShowNogoFlagAtGrid(grid_current);
					}
				}
			}
		}
	}
}

QPoint UIGoOperationalLayer::GlobalPosOfBoardGridCenter(const GridIndex& grid_index)
{
	auto iter = map_grid_global_rect_in_board_.find(grid_index);

	return iter != map_grid_global_rect_in_board_.cend() ? iter->second.center() : QPoint();
}

QPoint UIGoOperationalLayer::GlobalPosOfBasketChessCenter(ChessId chess_id)
{
	auto iter = map_chess_global_rect_in_basket_.find(chess_id);

	return iter != map_chess_global_rect_in_basket_.cend()? iter->second.center() : QPoint();
}

bool UIGoOperationalLayer::GlobalPosInBoardGrid(const QPoint& global_pos, GridIndex* grid_index, QRect* grid_global_rect)
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

bool UIGoOperationalLayer::GlobalPosInBasket(const QPoint& global_pos, ChessId* chess_id, QRect* grid_global_rect)
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

void UIGoOperationalLayer::LazyGetGridRects()
{
	int grid = operational_delegate_->GridsOfBoard();

	if (grid_ != grid)
	{
		grid_ = grid;

		map_grid_global_rect_in_board_ = operational_delegate_->GridsGlobalRectInBoard();

		map_chess_global_rect_in_basket_ = operational_delegate_->ChessesGlobalRectInBasket();
	}
}

QPoint UIGoOperationalLayer::CurrentGlobalPosOfChessFaceCenter() const
{
	return this->mapToGlobal(QPoint(this->width() * 0.5f, this->height() * 0.5f));
}

void UIGoOperationalLayer::ShowNogoFlagAtGrid(const GridIndex& grid_index)
{
	if (!IsNogoFlagShownAtGrid(grid_index))
	{
		auto nogo_flag = GetOrCreateNogoFlag();
		nogo_flag->grid_index_ = grid_index;

		UiUtils::MoveWidgetMiddleTo(nogo_flag->flag_widget_, this->mapFromGlobal(GlobalPosOfBoardGridCenter(grid_index)));
		nogo_flag->flag_widget_->show();
	}
}

bool UIGoOperationalLayer::IsNogoFlagShownAtGrid(const GridIndex& grid_index)
{
	for (const auto& nogo_flag : nogo_flags_)
	{
		if (nogo_flag.flag_widget_->isVisible() && nogo_flag.grid_ == grid_ && nogo_flag.grid_index_ == grid_index)
		{
			return true;
		}
	}

	return false;
}

UIGoOperationalLayer::NogoFlagInfo* UIGoOperationalLayer::GetOrCreateNogoFlag()
{
	for (auto& nogo_flag : nogo_flags_)
	{
		if (!nogo_flag.flag_widget_->isVisible() && nogo_flag.grid_ == grid_)
		{
			return &nogo_flag;
		}
	}

	auto nogo_flag_widget = new NogoFlagWidget(this);

	QPixmap pixmap(grid_ >= 13 ? R"(:go/res/images/go/nogo_small.png)" : R"(:go/res/images/go/nogo_big.png)");
	if (!pixmap.isNull())
	{
		nogo_flag_widget->setFixedSize(pixmap.size());
		nogo_flag_widget->setPixmap(pixmap);
	}

	nogo_flag_widget->hide();

	nogo_flags_.emplace_back(grid_, nogo_flag_widget);

	return &nogo_flags_.back();
}

GAME_NAMESPACE_END