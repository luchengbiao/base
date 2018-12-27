#include "go_game_board_ui.h"
#include <map>
#include <math.h>
#include "common/uiutils/ui_utils.h"
#include "game/model/go/actions/go_game_action_factory.h"
#include "game/model/go/go_game_board.h"
#include "game/model/command_handler/command_handler.h"
#include "game/public/public_setting/public_setting.h"
#include "game/view/animation/animation_layer.h"
#include "game/view/base/operational_layer/operational_layer.h"
#include "game/view/go/baskets/go_basket_base.h"
#include "game/view/go/board_painter/go_board_painter.h"
#include "game/view/scale_line/scale_line_layer.h"
#include "log/log.h"

GAME_NAMESPACE_BEGIN

UIGoGameBoard* UIGoGameBoard::Create(int grid, const GameRecordsPtr& all_games_to_be_cached, QWidget* parent)
{
	auto go_board = new UIGoGameBoard(grid, parent);
	if (go_board)
	{
		go_board->InitModel(all_games_to_be_cached);

		go_board->InitUI();
	}

	return go_board;
}

UIGoGameBoard::UIGoGameBoard(int grid, QWidget* parent)
: UIGameBoardBase(parent)
, grid_(grid)
, board_type_(GoTrait::GoBoardInfoByGrid(grid_).board_type_)
{
	memset(chesses_, 0x00, sizeof(chesses_));
}

void UIGoGameBoard::InitModel(const GameRecordsPtr& all_games_to_be_cached)
{
	auto action_factory = std::make_shared<GoGameActionFactory>();

	auto master_model = GoGameBoard::Create(grid_, all_games_to_be_cached);
	master_model->SetActionFacotry(action_factory);
	this->SetMasterModel(std::move(master_model));
}

void UIGoGameBoard::InitUI()
{
	setWindowFlags(Qt::FramelessWindowHint);
	setAutoFillBackground(true);

	QPalette palette;
	QPixmap pixmap(GoBoardPainter::CreatePixmap(grid_));
	palette.setBrush(backgroundRole(), QBrush(pixmap));
	setPalette(palette);

	this->setFixedSize(pixmap.size());

	this->CreateScaleLine();

	this->CreateChesses();
	this->UpdateChessesFromModel(master_model_);
}

void UIGoGameBoard::CreateChesses()
{
	const QSize sz_chess(InnerChessWidth(), InnerChessWidth());

	for (int x = 0; x < grid_; ++x)
	{
		for (int y = 0; y < grid_; ++y)
		{
			auto& chess = chesses_[x][y];
			chess = UIGoPieceBase::Create(grid_, this);

			chess->setFixedSize(sz_chess);
			chess->SetFaceFixedSize(sz_chess);

			chess->SetGridIndex(x, y);
			UiUtils::MoveWidgetMiddleTo(chess, this->PositionByGridIndex(chess->GetGridIndex()) + CorrectionForChessPosition());

			chess->SetChessId(ChessInfo::EmptyId);
			chess->UpdateChess(UIChessPieceBase::UpdateReason::Forcely);
		}
	}
}

void UIGoGameBoard::CreateScaleLine()
{
	scale_line_layer_ = new UIScaleLineLayer(this);

	scale_line_layer_->setFixedSize(this->size());

	const int font_size = 12;
	const QSize size(20, 20);

	std::string x_texts[19] = { "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S" };
	std::string y_texts[19] = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19" };

	scale_line_layer_->SetXScaleLines(GridOffsetFromOrigin() + QPoint(0, -6), InnerGridWidth(), { x_texts, x_texts + grid_ }, font_size, size);
	scale_line_layer_->SetYScaleLines(GridOffsetFromOrigin() + QPoint(-5, -2), InnerGridWidth(), { y_texts, y_texts + grid_ }, font_size, size);

	scale_line_layer_->lower();
}

void UIGoGameBoard::UpdateChessesFromModel(const GameBoardPtr& model)
{
	if (!model) { return; }

	auto go_model = std::dynamic_pointer_cast<GoGameBoard>(model);
	int max_index = go_model->MaxIndexOfChess();

	ForeachChess([=](UIGoPieceBase* chess_piece){
		if (chess_piece)
		{
			auto grid_index = chess_piece->GetGridIndex();

			auto chess_info = go_model->ChessAt(grid_index);
			auto chess_index = go_model->IndexOfChessAt(grid_index);

			chess_piece->SetChessId(chess_info->GetId());

			if (chess_info->IsNotEmpty())
			{
				chess_piece->ShowIndex(chess_index, chess_piece->QssObjectNameOfIndexLabel());

				chess_piece->SetFlagVisible(chess_index == max_index);
			}
			else
			{
				chess_piece->HideIndex();

				chess_piece->HideFlag();
			}
		}
	});

	this->MarkLastGridsTakenFromModel(model);
}

void UIGoGameBoard::UpdateChessesFromModelExceptVariations(const GameBoardPtr& model, const GameActionResult& result)
{
	if (!model) { return; }

	D_ASSERT(grid_ <= MaxGrid, L"grid_ is too large");

	const ChessVariation* vars[MaxGrid][MaxGrid];
	memset(vars, 0x00, sizeof(vars));

	result.ForeachChessVariation([&](const ChessVariation& var){
		auto grid = var.GetGridIndex();
		vars[grid.X()][grid.Y()] = &var;
	});

	ForeachChess([&](UIGoPieceBase* chess_piece){
		if (chess_piece)
		{
			auto grid = chess_piece->GetGridIndex();
			auto var = vars[grid.X()][grid.Y()];
			if (var)
			{
				chess_piece->SetChessId(var->GetOldChessId());
			}
			else
			{
				chess_piece->SetChessId(*model->ChessAt(grid));
			}
		}
	});
}

void UIGoGameBoard::RefreshBoardWithMaster()
{
	this->SetLocalDirty(true);

	if (!operational_layer_.isNull())
	{
		operational_layer_->Interrupt();
	}

	if (animation_layer_) { animation_layer_->Clear(); }

	this->UpdateChessesFromModel(master_model_);

	this->UpdateBasketWithModel(master_model_);
}

// triggered originally by AgoraMessageChannel (remotely, passively)
// call stack roughly as following:
// 1 -- MessageProcessor::SlotAgoraMessageChannel: receive raw data and parse it into CommandCell entities which aggregate in a CommandGroup
// 2 -- MessageProcessor::HandleCommandGroup: dispatch(notify) each Command entity
// 3 -- MessageCommandObserver::OnCommand: receive Command entity
// 4 -- UIGameBoardBase::HandleCommandOnMasterModel
// 5 -- GameBoardBase::DoCommand: genarate GameAction in terms of Command above
// 6 -- GameBoardBase::DoAction
// 7 -- GameActionWithResult::Do: do act on a concrete GameBoard to generate GameActionResult
// 8 -- GameBoardBase::NotifyGameActionDone
// 9 -- MasterModelObserver::OnGameActionDone
// 10 -- UIGoGameBoard::OnMasterDone
void UIGoGameBoard::OnMasterDone(const GameActionResult& result)
{
	D_ASSERT(result.IsValid(), L"result should be valid");

	bool play_this_result = command_handler_->TestSyncLocalWithMasterWhenMasterDone(master_model_->BoardType(), result);

	if (play_this_result)
	{
		this->SetLocalDirty(true);

		if (!operational_layer_.isNull())
		{
			operational_layer_->Interrupt();
		}

		if (result.BoardIsChangedByAction())
		{
			if (animation_layer_) { animation_layer_->Clear(); }

			if (NeedToPlayActionResultImmediately(result))
			{
				this->UpdateChessesFromModel(master_model_);
			}
			else
			{
				this->PlayMasterVariationOnAnimationLayer(result);

				this->PlaySoundByActionResult(result);
			}
		}

		this->UpdateBasketWithModel(master_model_);
	}
}

// triggered originally by UIOperationalLayer::TryToMoveChess (locally, actively)
// call stack roughly as following:
// 1 -- UIOperationalLayer::TryToMoveChess
// 2 -- UIOperationalDelegate::TryToMoveChessLocally
// 3 -- UIGameBoardBase::TryToMoveChessLocally: generate Command
// 4 -- GameBoardBase::DoCommand: genarate GameAction in terms of Command above
// 5 -- GameBoardBase::DoAction
// 6 -- GameActionWithResult::Do: do act on a concrete GameBoard to generate GameActionResult
// 7 -- GameBoardBase::NotifyGameActionDone
// 8 -- LocalModelObserver::OnGameActionDone
// 9 -- UIGoGameBoard::OnLocalDone
void  UIGoGameBoard::OnLocalDone(const GameActionResult& result)
{
	D_ASSERT(result.IsValid(), L"result should be valid");

	this->ClearOperationalLayer();

	this->SendCommand(result.GetCommandPtr());

	if (result.BoardIsChangedByAction())
	{
		if (animation_layer_) { animation_layer_->Clear(); }

		if (NeedToPlayActionResultImmediately(result))
		{
			this->UpdateChessesFromModel(local_model_);
		}
		else
		{
			this->PlayLocalVariationOnAnimationLayer(result);

			this->PlaySoundByActionResult(result);
		}
	}

	this->UpdateBasketWithModel(local_model_);
}

void UIGoGameBoard::PlayMasterVariationOnAnimationLayer(const GameActionResult& result)
{
	this->UpdateChessesFromModelExceptVariations(master_model_, result);

	std::function<void()> callback = chess_move_callback_flag_.ToWeakCallback([=]{
		this->UpdateChessesFromModel(master_model_);

		result.ForeachChessVariation([=](const ChessVariation& chess_var){
			if (chess_var.IsCapturedPassively())
			{
				animation_layer_->PlayChessVariation(chess_var);
			}
		});
	});

	bool moved_actively = false;
	result.ForeachChessVariationWithBreaker([&](const ChessVariation& chess_var){
		if (chess_var.IsMovedActively())
		{
			animation_layer_->PlayChessVariation(chess_var, callback, false);

			moved_actively = true;
		}

		return moved_actively;
	});

	if (!moved_actively)
	{
		callback();
	}
}

void UIGoGameBoard::PlayLocalVariationOnAnimationLayer(const GameActionResult& result)
{
	this->UpdateChessesFromModelExceptVariations(local_model_, result);

	std::function<void()> callback = chess_move_callback_flag_.ToWeakCallback([=]{
		this->UpdateChessesFromModel(local_model_);

		result.ForeachChessVariation([=](const ChessVariation& chess_var){
			if (chess_var.IsCapturedPassively())
			{
				animation_layer_->PlayChessVariation(chess_var);
			}
		});
	});

	bool moved_actively = false;
	result.ForeachChessVariationWithBreaker([&](const ChessVariation& chess_var){
		if (chess_var.IsMovedActively())
		{
			animation_layer_->PlayChessVariation(chess_var, callback, false);

			moved_actively = true;
		}

		return moved_actively;
	});

	if (!moved_actively)
	{
		callback();
	}
}

void UIGoGameBoard::MarkLastGridsTakenFromModel(const GameBoardPtr& model)
{
	do 
	{
		if (!model) { break; }
		if (!animation_layer_) { break; }

		auto go_board = std::static_pointer_cast<GoGameBoard>(model);

		ChessId chess_id_taken = ChessInfo::EmptyId;
		std::vector<GridIndex>;
		auto grids_taken = go_board->LastGridsTaken(&chess_id_taken);

		if (!grids_taken.empty())
		{
			auto mark_width = GridWidth() * 0.66f;
			auto mark_color = chess_id_taken == GoTrait::IdOfBlackPiece ? Qt::GlobalColor::black : Qt::GlobalColor::white;
			animation_layer_->MarkOnGridsInBoard(grids_taken, QSize(mark_width, mark_width), mark_color);
		}
	} while (false);
}

bool UIGoGameBoard::IsRuleBased() const
{
	auto master_board = MasterGameBoard();

	return master_board ? master_board->IsRuleBased() : false;
}

void UIGoGameBoard::OnOperationalLayerAssociated()
{
	auto local_model = GoGameBoard::Create(grid_, nullptr);

	local_model->SetActionFacotry(master_model_->ActionFacotry());
	local_model->SetRuleBased(MasterGameBoard()->IsRuleBased());

	this->SetLocalModel(std::move(local_model));
}

void UIGoGameBoard::OnLayoutChanged()
{
	ForeachChess([this](UIGoPieceBase* chess){
		if (chess)
		{
			UiUtils::MoveWidgetMiddleTo(chess, this->PositionByGridIndex(chess->GetGridIndex()) + CorrectionForChessPosition());
		}
	});
}

QPoint UIGoGameBoard::PositionByGridIndex(const GridIndex& grid_index) const
{
	const auto width = InnerGridWidth();

	if (GetLayout() == GameBoardLayout::Flipped)
	{
		return GridOffsetFromOrigin() + QPoint(width * (grid_ - grid_index.X()), width * (grid_ - grid_index.Y()));
	}

	return GridOffsetFromOrigin() + QPoint(width * grid_index.X(), width * grid_index.Y());
}

void UIGoGameBoard::SetScaleLineVisible(bool visible)
{
	if (scale_line_layer_)
	{
		scale_line_layer_->setVisible(visible);
	}
}

void UIGoGameBoard::HandleMasterCommandSpecially(const CommandSharedPtr& cmd_ptr)
{
	UIGameBoardBase::HandleMasterCommandSpecially(cmd_ptr);
}

GoGameBoard* UIGoGameBoard::MasterGameBoard() const
{
	return static_cast<GoGameBoard*>(master_model_.get());
}

GoGameBoard* UIGoGameBoard::LocalGameBoard() const
{
	return static_cast<GoGameBoard*>(local_model_.get());
}

// override UIAnimationDelegate -- begin
QPixmap	UIGoGameBoard::MovingPixmapOfChessId(ChessId chess_id)
{
	return QPixmap(QString::fromStdString(GoTrait::GoPieceInfoByIdAndGrid(chess_id, grid_).moving_image_));
}

QPoint UIGoGameBoard::GlobalPositionOfGridCenterInBoard(const GridIndex& grid_index)
{
	return mapToGlobal(PositionByGridIndex(grid_index));
}

QPoint UIGoGameBoard::GlobalPositionOfChessCenterInBasket(ChessId chess_id)
{
	D_ASSERT(chess_basket_, L"chess_basket_ should not be null");

	return chess_basket_->GlobalPositionOfChessCenter(chess_id);
}

void UIGoGameBoard::HideChessInBoardGrid(const GridIndex& grid_index)
{
	this->HideChessAt(grid_index);
}

int	 UIGoGameBoard::MilliSecondsOfMovingAnimation()
{
	return 200;
}
// override UIAnimationDelegate -- end

void UIGoGameBoard::UpdateBasketWithModel(const GameBoardPtr& model)
{
	UIGameBoardBase::UpdateBasketWithModel(model);

	auto go_model = std::static_pointer_cast<GoGameBoard>(model);
	if (go_model && !go_model->IsRuleBased())
	{
		auto go_basket = dynamic_cast<UIGoBasketBase*>(chess_basket_.data());
		if (go_basket)
		{
			go_basket->SetAutoTakingPieces(go_model->IsAutoTakingPieces());
		}
	}
}

GAME_NAMESPACE_END