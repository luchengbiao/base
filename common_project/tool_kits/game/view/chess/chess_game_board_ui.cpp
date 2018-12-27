#include "chess_game_board_ui.h"
#include <map>
#include "game/model/chess/actions/chess_game_action_factory.h"
#include "game/model/chess/chess_game_board.h"
#include "game/model/command_handler/command_handler.h"
#include "game/public/literal_stock/literal_stock.h"
#include "game/public/public_setting/public_setting.h"
#include "game/view/animation/animation_layer.h"
#include "game/view/base/chess_basket/chess_basket_base.h"
#include "game/view/base/operational_layer/operational_layer.h"
#include "game/view/chess/promote_dialog/target_chess_to_promote_dialog.h"
#include "game/view/scale_line/scale_line_layer.h"
#include "log/log.h"

GAME_NAMESPACE_BEGIN

UIChessGameBoard* UIChessGameBoard::Create(const GameRecordsPtr& all_games_to_be_cached, QWidget* parent)
{
	auto chess_board = new UIChessGameBoard(parent);
	if (chess_board)
	{
		chess_board->InitModel(all_games_to_be_cached);

		chess_board->InitUI();
	}

	return chess_board;
}

UIChessGameBoard::UIChessGameBoard(QWidget* parent)
: UIGameBoardBase(parent)
{
	memset(chesses_, 0x00, sizeof(chesses_));
}

void UIChessGameBoard::InitModel(const GameRecordsPtr& all_games_to_be_cached)
{
	auto action_factory = std::make_shared<ChessGameActionFactory>();

	auto master_model = ChessGameBoard::Create(all_games_to_be_cached);
	master_model->SetActionFacotry(action_factory);
	this->SetMasterModel(std::move(master_model));
}

void UIChessGameBoard::InitUI()
{
	setWindowFlags(Qt::FramelessWindowHint);

	QPalette palette;
	QPixmap pixmap(":/chess/res/images/chess/chess_board.png");
	palette.setBrush(backgroundRole(), QBrush(pixmap));
	setPalette(palette);
	setAutoFillBackground(true);
	this->setFixedSize(pixmap.size());

	this->CreateScaleLine();

	this->CreateChesses();
	this->UpdateChessesFromModel(master_model_);
}

void UIChessGameBoard::CreateChesses()
{
	for (int x = 0; x < Grid; ++x)
	{
		for (int y = 0; y < Grid; ++y)
		{
			auto& chess = chesses_[x][y];
			chess = new UIChessPiece(this);

			chess->setFixedSize(GridWidth(), GridWidth());

			chess->SetGridIndex(x, y);
			chess->move(this->PositionByGridIndex(chess->GetGridIndex()));

			chess->SetChessId(ChessInfo::EmptyId);
			chess->UpdateChess(UIChessPieceBase::UpdateReason::Forcely);
		}
	}
}

void UIChessGameBoard::CreateScaleLine()
{
	scale_line_layer_ = new UIScaleLineLayer(this);

	scale_line_layer_->setFixedSize(this->size());

	const int font_size = 12;
	const QSize size(20, 20);

	scale_line_layer_->SetXScaleLines(GridOffsetFromOrigin() + QPoint(GridWidth() * 0.5f - 1, -6), GridWidth(), { "A", "B", "C", "D", "E", "F", "G", "H" }, font_size, size);
	scale_line_layer_->SetYScaleLines(GridOffsetFromOrigin() + QPoint(-5, GridWidth() * 0.5f), GridWidth(), { "8", "7", "6", "5", "4", "3", "2", "1" }, font_size, size);

	scale_line_layer_->lower();
}

bool UIChessGameBoard::IsLocalGameOver(GameOverType* type, QString* hint)
{
	return IsGameOverOfModel(local_model_, type, hint);
}

bool UIChessGameBoard::IsGameOverOfModel(const GameBoardPtr& model, GameOverType* type, QString* hint)
{
	bool game_over = false;

	do 
	{
		if (!model) { break; }

		auto chess_model = std::static_pointer_cast<ChessGameBoard>(model);

		if (chess_model->IsRuleBased())
		{
			auto game_over_type = GameOverType::None;
			game_over = chess_model->IsGameOverWithRule(&game_over_type);

			if (type) { *type = game_over_type; }
			if (hint)
			{
				switch (game_over_type)
				{
				case GameOverType::WhiteWin:
					*hint = QStringByKey(LK_THE_WHITE_SIDE_WINS);
					break;

				case GameOverType::BlackWin:
					*hint = QStringByKey(LK_THE_BLACK_SIDE_WINS);
					break;

				case GameOverType::Draw:
					*hint = QStringByKey(LK_GAME_IS_DRAW);
					break;

				default:
					*hint = QString();
					break;
				}
			}
		}
	} while (false);

	return game_over;

}

void UIChessGameBoard::TryToHintGameSituationFromModel(const GameBoardPtr& model)
{
	if (!model) { return; }

	auto chess_model = std::static_pointer_cast<ChessGameBoard>(model);

	QString hint;
	if (IsGameOverOfModel(chess_model, nullptr, &hint))
	{
		auto game_over_type = GameOverType::None;
		if (chess_model->IsGameOverWithRule(&game_over_type))
		{
			if (!hint.isEmpty())
			{
				HintGameOver(hint);
			}
		}
		else
		{
			if (chess_model->IsCheckingWithRule())
			{
				HintChecker(QStringByKey(LK_CHESS_CHECKERS));
			}
		}
	}
}

void UIChessGameBoard::HintGameOver(const QString& hint)
{
	if (UsageTypeIsOnline())
	{
		TipOnMsgBox(hint);
	}
	else
	{
		TipOnToast(hint);
	}
}

void UIChessGameBoard::HintChecker(const QString& hint)
{
	if (UsageTypeIsOnline())
	{
		CloseTipMsgBox();
	}

	TipOnToast(hint);
}

void UIChessGameBoard::ShowLastGridMotionFromModel(const GameBoardPtr& model)
{
	if (!model) { return; }

	if (animation_layer_)
	{
		auto chess_model = std::static_pointer_cast<ChessGameBoard>(model);

		auto chess_id_moved = ChessInfo::EmptyId;
		auto grid_motion = chess_model->LastGridMotionOfCurrentGame(&chess_id_moved);

		if (grid_motion)
		{
			animation_layer_->ShowArrowLine(chess_id_moved, grid_motion);
		}
	}
}

void UIChessGameBoard::UpdateChessesFromModel(const GameBoardPtr& model)
{
	if (!model) { return; }
	
	ForeachChess([&](UIChessPiece* chess_piece){
		if (chess_piece)
		{
			chess_piece->SetChessId(*model->ChessAt(chess_piece->GetGridIndex()));
			chess_piece->Unhighlight();
		}
	});
}

void UIChessGameBoard::UpdateChessesFromModelExceptVariations(const GameBoardPtr& model, const GameActionResult& result)
{
	if (!model) { return; }

	const ChessVariation* vars[Grid][Grid];
	memset(vars, 0x00, sizeof(vars));

	result.ForeachChessVariation([&](const ChessVariation& var){
		auto grid = var.GetGridIndex();
		vars[grid.X()][grid.Y()] = &var;
	});

	ForeachChess([&](UIChessPiece* chess_piece){
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

void UIChessGameBoard::Reset()
{
	UIGameBoardBase::Reset();

	TargetChessToPromoteDialog::Interrupt();

	ForeachChess([](UIChessPiece* chess_piece){
		if (chess_piece)
		{
			chess_piece->Unhighlight();
		}
	});
}

void UIChessGameBoard::RefreshBoardWithMaster()
{
	TargetChessToPromoteDialog::Interrupt();

	this->SetLocalDirty(true);

	if (!operational_layer_.isNull())
	{
		operational_layer_->Interrupt();
	}

	if (animation_layer_) { animation_layer_->Clear(); }

	this->UpdateChessesFromModel(master_model_);

	this->ShowLastGridMotionFromModel(master_model_);

	this->UpdateBasketWithModel(master_model_);

	this->TryToHintGameSituationFromModel(master_model_);
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
// 10 -- UIChessGameBoard::OnMasterDone
void UIChessGameBoard::OnMasterDone(const GameActionResult& result)
{
	D_ASSERT(result.IsValid(), L"result should be valid");

	bool play_this_result = command_handler_->TestSyncLocalWithMasterWhenMasterDone(master_model_->BoardType(), result);

	if (play_this_result)
	{ 
		TargetChessToPromoteDialog::Interrupt();

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

				this->ShowLastGridMotionFromModel(master_model_);
			}
			else
			{
				this->PlayMasterVariationOnAnimationLayer(result);

				this->PlaySoundByActionResult(result);
			}

			this->TryToHintGameSituationFromModel(master_model_);
		}

		this->UpdateBasketWithModel(master_model_);
	}
}

// triggered originally by UIOperationalLayer::TryToMoveChess (locally, actively)
// call stack roughly as following:
// 1 -- UIOperationalLayer::TryToMoveChess
// 2 -- UIOperationalDelegate::TryToMoveChessLocally
// 3 -- UIGameBoardBase::TryToMoveChessLocally: generate Command
// 4 -- GameBoardBase::TryToDoCommand: genarate GameAction in terms of Command above
// 5 -- GameBoardBase::TryToDoAction
// 6 -- GameActionWithResult::Do: do act on a concrete GameBoard to generate GameActionResult
// 7 -- GameBoardBase::NotifyGameActionDone
// 8 -- LocalModelObserver::OnGameActionDone
// 9 -- UIChessGameBoard::OnLocalDone
void  UIChessGameBoard::OnLocalDone(const GameActionResult& result)
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

			this->ShowLastGridMotionFromModel(local_model_);
		}
		else
		{
			this->PlayLocalVariationOnAnimationLayer(result);

			this->PlaySoundByActionResult(result);
		}

		this->TryToHintGameSituationFromModel(local_model_);
	}

	this->UpdateBasketWithModel(local_model_);
}

void UIChessGameBoard::PlayMasterVariationOnAnimationLayer(const GameActionResult& result)
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

	int moved_actively = 0;
	result.ForeachChessVariation([&](const ChessVariation& chess_var){
		if (chess_var.IsMovedActively())
		{
			moved_actively += 1;
			animation_layer_->PlayChessVariation(chess_var, callback, moved_actively == 1);

			callback = nullptr; // only callback once
		}
	});

	if (!moved_actively)
	{
		callback();
	}
}

void UIChessGameBoard::PlayLocalVariationOnAnimationLayer(const GameActionResult& result)
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

	auto passing_point = operational_layer_->CurrentGlobalPosOfChessFaceCenter();
	
	int moved_actively = 0;
	result.ForeachChessVariation([&](const ChessVariation& chess_var){
		if (chess_var.IsMovedActively())
		{
			moved_actively += 1;
			animation_layer_->PlayChessVariationWithPassingPoint(chess_var, passing_point, callback, moved_actively == 1);

			callback = nullptr; // only callback once
		}
	});

	if (!moved_actively)
	{
		callback();
	}
}

bool UIChessGameBoard::IsRuleBased() const
{
	auto master_board = MasterGameBoard();

	return master_board ? master_board->IsRuleBased() : false;
}

void UIChessGameBoard::OnOperationalLayerAssociated()
{
	auto local_model = ChessGameBoard::Create(nullptr);

	local_model->SetActionFacotry(master_model_->ActionFacotry());
	local_model->SetRuleBased(MasterGameBoard()->IsRuleBased());

	this->SetLocalModel(std::move(local_model));
}

void UIChessGameBoard::OnLayoutChanged()
{
	ForeachChess([this](UIChessPiece* chess_piece){
		if (chess_piece)
		{
			chess_piece->move(this->PositionByGridIndex(chess_piece->GetGridIndex()));
		}
	});
}

QPoint UIChessGameBoard::PositionByGridIndex(const GridIndex& grid_index) const
{ 
	if (GetLayout() == GameBoardLayout::Flipped)
	{
		return GridOffsetFromOrigin() + QPoint(GridWidth() * (7 - grid_index.X()), GridWidth() * (7 - grid_index.Y()));
	}
	
	return GridOffsetFromOrigin() + QPoint(GridWidth() * grid_index.X(), GridWidth() * grid_index.Y());
}

void UIChessGameBoard::SetScaleLineVisible(bool visible)
{
	if (scale_line_layer_)
	{
		scale_line_layer_->setVisible(visible);
	}
}

void UIChessGameBoard::HandleMasterCommandSpecially(const CommandSharedPtr& cmd_ptr)
{
	UIGameBoardBase::HandleMasterCommandSpecially(cmd_ptr);
}

bool UIChessGameBoard::TryToMoveChessLocally(ChessId chess_id, const GridIndex& grid_from, const GridIndex& grid_to, std::string* failed_info)
{
	bool ret = false;

	do 
	{
		auto grid_to_tmp = grid_to;
		if (!HandleCastlingBeforeMovingChessLocally(chess_id, grid_from, grid_to_tmp))
		{
			bool interrupted = false;
			auto chess_id_to_promote = HandlePromotionBeforeMovingChessLocally(chess_id, grid_from, grid_to_tmp, &interrupted);

			if (interrupted) { break; }

			if (ChessIdIsNotEmpty(chess_id_to_promote)) { chess_id = chess_id_to_promote; }
		}

		ret = UIGameBoardBase::TryToMoveChessLocally(chess_id, grid_from, grid_to_tmp, failed_info);

	} while (false);

	if (!ret)
	{
		this->ClearOperationalLayer();
	}

	return ret;
}

bool UIChessGameBoard::HandleCastlingBeforeMovingChessLocally(ChessId chess_id, const GridIndex& grid_from, GridIndex& grid_to)
{
	if (IsRuleBased())
	{
		if (ChessTrait::TypeById(chess_id) == game::ChessType::King && grid_from.Y() == grid_to.Y())
		{
			int diff = grid_from.X() - grid_to.X();
			if (abs(diff) == 2)
			{
				grid_to.SetX(diff > 0 ? 0 : 7);

				return true;
			}
		}
	}

	return false;
}

ChessId UIChessGameBoard::HandlePromotionBeforeMovingChessLocally(ChessId chess_id, const game::GridIndex& grid_from, game::GridIndex& grid_to, bool* interrupted)
{
	auto chess_id_to_promote = game::ChessInfo::EmptyId;

	if (IsRuleBased())
	{
		if (game::ChessTrait::TypeById(chess_id) == game::ChessType::Pawn
			&& ChessTrait::BottomRowIndexOfColor(game::ChessTrait::ColorById(chess_id)) == grid_to.Y()
			&& LocalGameBoard()->IsMoveLegalWithRule(grid_from, grid_to))
		{
			chess_id_to_promote = TargetChessToPromoteDialog::GetTargetChessToPromote(game::ChessTrait::ColorById(chess_id), interrupted, this);
		}
	}

	return chess_id_to_promote;
}

ChessGameBoard* UIChessGameBoard::MasterGameBoard() const 
{ 
	return static_cast<ChessGameBoard*>(master_model_.get()); 
}

ChessGameBoard* UIChessGameBoard::LocalGameBoard() const
{
	return static_cast<ChessGameBoard*>(local_model_.get());
}

// override UIAnimationDelegate -- begin
QPixmap	UIChessGameBoard::MovingPixmapOfChessId(ChessId chess_id)
{
	return QPixmap(QString::fromStdString(ChessTrait::MovingImagePathById(chess_id)));
}

QPoint UIChessGameBoard::GlobalPositionOfGridCenterInBoard(const GridIndex& grid_index)
{
	auto grid_width_half = GridWidth() * 0.5f;
	return mapToGlobal(PositionByGridIndex(grid_index) + QPoint(grid_width_half, grid_width_half));
}

QPoint UIChessGameBoard::GlobalPositionOfChessCenterInBasket(ChessId chess_id)
{
	D_ASSERT(chess_basket_, L"chess_basket_ should not be null");

	return chess_basket_->GlobalPositionOfChessCenter(chess_id);
}

void UIChessGameBoard::HideChessInBoardGrid(const GridIndex& grid_index)
{
	this->HideChessAt(grid_index);
}

int	 UIChessGameBoard::MilliSecondsOfMovingAnimation()
{
	return 200;
}
// override UIAnimationDelegate -- end

GAME_NAMESPACE_END