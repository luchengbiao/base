#include "animation_layer.h"
#include <algorithm>
#include <QEvent>
#include <QPropertyAnimation>
#include "common/macros/assert.h"
#include "common/uiutils/ui_utils.h"
#include "game/model/base/chess/chess_variation.h"
#include "game/view/arrow_line/arrow_line.h"
#include "game/view/base/chess_piece/chess_piece_base.h"
#include "game/view/base/chess_face_impl.h"
#include "grid_mark.h"

GAME_NAMESPACE_BEGIN

struct UIAnimationLayer::ChessFaceInfo
{
	ChessFace*				chess_face_{ nullptr };
	QPropertyAnimation*		move_ani_{ nullptr };
	bool					is_being_used_{ false };
	std::function<void()>	moved_callback_;

	ChessFaceInfo(ChessFace* chess_face, QPropertyAnimation* move_ani, bool is_being_used)
		: chess_face_(chess_face), move_ani_(move_ani), is_being_used_(is_being_used)
	{}

	bool isMoving() const
	{
		return move_ani_ && move_ani_->state() == QAbstractAnimation::Running;
	}
};

UIAnimationLayer::UIAnimationLayer(QWidget* parent)
: QWidget(parent)
{
}

UIAnimationLayer::~UIAnimationLayer()
{
	for (auto item : vec_chess_face_)
	{
		delete item;
	}
}

void UIAnimationLayer::SetAnimationDelegate(UIAnimationDelegate* animation_delegate)
{
	animation_delegate_ = animation_delegate;
}

void UIAnimationLayer::PlayChessVariation(const ChessVariation& chess_var,
	const std::function<void()>& moved_callback,
	bool show_arrow_line)
{
	D_ASSERT(animation_delegate_, L"animation_delegate_ should not be null");

	const auto chess_id_moved = chess_var.GetChessIdMoved();

	if (ChessIdIsNotEmpty(chess_id_moved))
	{
		const auto grid_motion = chess_var.GetGridMotion();

		if (chess_var.IsMovedInsideOfBoardActively() || chess_var.IsMovedOutOfBoardActively())
		{
			animation_delegate_->HideChessInBoardGrid(grid_motion.From());
		}

		this->MoveChess(chess_id_moved, grid_motion.From(), grid_motion.To(), moved_callback, show_arrow_line);
	}
}

void UIAnimationLayer::PlayChessVariationWithPassingPoint(const ChessVariation& chess_var,
	const QPoint& passing_pos_global,
	const std::function<void()>& moved_callback,
	bool show_arrow_line)
{
	D_ASSERT(animation_delegate_, L"animation_delegate_ should not be null");

	const auto chess_id_moved = chess_var.GetChessIdMoved();

	if (ChessIdIsNotEmpty(chess_id_moved))
	{
		const auto grid_motion = chess_var.GetGridMotion();

		if (chess_var.IsMovedInsideOfBoardActively() || chess_var.IsMovedOutOfBoardActively())
		{
			animation_delegate_->HideChessInBoardGrid(grid_motion.From());
		}

		this->MoveChessWithPassingPoint(chess_id_moved, grid_motion.From(), passing_pos_global, grid_motion.To(), moved_callback, show_arrow_line);
	}
}

void UIAnimationLayer::MoveChess(ChessId chess_id,
	const GridIndex& grid_from,
	const GridIndex& grid_to,
	const std::function<void()>& moved_callback,
	bool show_arrow_line)
{
	D_ASSERT(animation_delegate_, L"animation_delegate_ should not be null");

	auto pos_from_global = grid_from.IsValid() ? animation_delegate_->GlobalPositionOfGridCenterInBoard(grid_from) : animation_delegate_->GlobalPositionOfChessCenterInBasket(chess_id);

	this->MoveChessWithPassingPoint(chess_id, grid_from, pos_from_global, grid_to, moved_callback, show_arrow_line);
}

void UIAnimationLayer::MoveChessWithPassingPoint(ChessId chess_id,
	const GridIndex& grid_from,
	const QPoint& passing_pos_global,
	const GridIndex& grid_to,
	const std::function<void()>& moved_callback,
	bool show_arrow_line)
{
	D_ASSERT(animation_delegate_, L"animation_delegate_ should not be null");

	auto item = GetOrCreateChessFace();

	item->is_being_used_ = true;
	item->moved_callback_ = moved_callback;

	auto chess_face = item->chess_face_;
	auto move_ani = item->move_ani_;

	auto pixmap = animation_delegate_->MovingPixmapOfChessId(chess_id);
	chess_face->setPixmap(pixmap);
	chess_face->setFixedSize(pixmap.size());

	auto chess_width_half = chess_face->width() * 0.5f;
	auto chess_height_half = chess_face->height() * 0.5f;

	auto pos_from_global = grid_from.IsValid() ? animation_delegate_->GlobalPositionOfGridCenterInBoard(grid_from) : animation_delegate_->GlobalPositionOfChessCenterInBasket(chess_id);
	auto pos_to_global = grid_to.IsValid() ? animation_delegate_->GlobalPositionOfGridCenterInBoard(grid_to) : animation_delegate_->GlobalPositionOfChessCenterInBasket(chess_id);

	auto pos_from = this->mapFromGlobal(passing_pos_global) - QPoint(chess_width_half, chess_height_half);
	auto pos_to = this->mapFromGlobal(pos_to_global) - QPoint(chess_width_half, chess_height_half);

	chess_face->show();
	chess_face->move(pos_from);

	move_ani->setStartValue(pos_from);
	move_ani->setEndValue(pos_to); // will trigger valueChanged
	move_ani->setDuration(animation_delegate_->MilliSecondsOfMovingAnimation());
	move_ani->setEasingCurve(QEasingCurve::OutExpo);

	move_ani->start();

	if (show_arrow_line)
	{
		animation_traced_by_arrow_line_ = move_ani;
		this->ShowArrowLine(pos_from_global, passing_pos_global);
	}
}

void UIAnimationLayer::MarkOnGridsInBoard(const std::vector<GridIndex>& grids, const QSize& size, const QColor& color)
{
	for (const auto& grid : grids)
	{
		auto grid_mark = GetOrCreateGridMark();

		grid_mark->show();
		grid_mark->setFixedSize(size);
		grid_mark->SetColor(color);

		UiUtils::MoveWidgetMiddleTo(grid_mark, mapFromGlobal(animation_delegate_->GlobalPositionOfGridCenterInBoard(grid)) + QPoint(1, 1)); // QPoint(1, 1) for correction?!
	}
}

bool UIAnimationLayer::HasAnimationPlaying() const
{
	for (auto item : vec_chess_face_)
	{
		if (item->isMoving())
		{
			return true;
		}
	}

	return false;
}

void UIAnimationLayer::StopAnimations()
{
	for (auto item : vec_chess_face_)
	{
		if (item->is_being_used_)
		{
			item->is_being_used_ = false;
			item->chess_face_->hide();
			item->move_ani_->stop();
			item->moved_callback_ = nullptr;
		}
	}
}

void UIAnimationLayer::HideGridMarks()
{
	for (auto grid_mark : vec_grid_mark_)
	{
		grid_mark->hide();
	}
}

UIAnimationLayer::ChessFaceInfo* UIAnimationLayer::GetOrCreateChessFace()
{
	for (auto item : vec_chess_face_)
	{
		if (item->is_being_used_ == false)
		{
			return item;
		}
	}

	auto chess_face = new ChessFace(this);
	auto move_ani = new QPropertyAnimation(chess_face, "pos", this);

	chess_face->hide();

	vec_chess_face_.emplace_back(new ChessFaceInfo(chess_face, move_ani, true));

	connect(move_ani, &QPropertyAnimation::finished, this, [=]{
		if (move_ani == animation_traced_by_arrow_line_)
		{
			animation_traced_by_arrow_line_ = nullptr;
		}

		ChessFaceInfo* target = nullptr;
		for (auto item : vec_chess_face_)
		{
			if (item->move_ani_ == move_ani)
			{
				target = item;
				break;
			}
		}

		if (target)
		{
			auto callback = target->moved_callback_;

			target->is_being_used_ = false;
			target->chess_face_->hide();
			target->moved_callback_ = nullptr;

			if (callback)
			{
				callback();
			}
		}
	});

	connect(move_ani, &QPropertyAnimation::valueChanged, this, [=](const QVariant&){
		if (move_ani == animation_traced_by_arrow_line_)
		{
			auto size = chess_face->size();
			auto pos = chess_face->mapToGlobal(QPoint(size.width() * 0.5f, size.height() * 0.5f));
			GetOrCreateArrowLine()->SetEndPosition(pos);
		}
	});

	return vec_chess_face_.back();
}

UIGridMark* UIAnimationLayer::GetOrCreateGridMark()
{
	for (auto grid_mark : vec_grid_mark_)
	{
		if (!grid_mark->isVisible())
		{
			return grid_mark;
		}
	}

	auto grid_mark = new UIGridMark(this);

	grid_mark->setFixedSize(20, 20);

	grid_mark->hide();

	vec_grid_mark_.emplace_back(grid_mark);

	return vec_grid_mark_.back();
}

UIArrowLine* UIAnimationLayer::GetOrCreateArrowLine()
{
	if (!arrow_line_)
	{
		arrow_line_ = new UIArrowLine(this);

		arrow_line_->setFixedSize(this->size());
		arrow_line_->raise();
	}

	return arrow_line_;
}

void UIAnimationLayer::ShowArrowLine(ChessId chess_id, const GridIndex& grid_from, const GridIndex& grid_to)
{
	D_ASSERT(animation_delegate_, L"animation_delegate_ should not be null");

	auto pos_from_global = grid_from.IsValid() ? animation_delegate_->GlobalPositionOfGridCenterInBoard(grid_from) : animation_delegate_->GlobalPositionOfChessCenterInBasket(chess_id);
	auto pos_to_global = grid_to.IsValid() ? animation_delegate_->GlobalPositionOfGridCenterInBoard(grid_to) : animation_delegate_->GlobalPositionOfChessCenterInBasket(chess_id);

	this->ShowArrowLine(pos_from_global, pos_to_global);
}

void UIAnimationLayer::ShowArrowLine(const QPoint& pos_start_global, const QPoint& pos_end_global)
{
	auto arrow_line = GetOrCreateArrowLine();

	arrow_line->show();
	arrow_line->SetStartAndEndPosition(pos_start_global, pos_end_global);
}

void UIAnimationLayer::HideArrowLine()
{
	if (arrow_line_)
	{
		arrow_line_->hide();
	}

	animation_traced_by_arrow_line_ = false;
}

void UIAnimationLayer::Clear()
{
	this->StopAnimations();

	this->HideArrowLine();

	this->HideGridMarks();
}

GAME_NAMESPACE_END