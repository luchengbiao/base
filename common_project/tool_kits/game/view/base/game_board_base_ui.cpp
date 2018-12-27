#include "game_board_base_ui.h"
#include "base/util/value_guard.h"
#include "game/model/base/board/game_board_base.h"
#include "game/model/base/board/game_board_observer.h"
#include "game/model/base/record/custom_record.h"
#include "game/model/command_handler/command_handler.h"
#include "game/public/public_setting/public_setting.h"
#include "game/view/animation/animation_layer.h"
#include "game/view/base/chess_piece/chess_piece_base.h"
#include "game/view/base/chess_basket/chess_basket_base.h"
#include "game/view/base/operational_layer/operational_layer.h"
#include "game/view/tip_helper/tip_helper.h"
#include "protocol/command/commands/cmd_student_move_chess.h"
#include "protocol/command/commands/cmd_teacher_move_chess.h"

using namespace proto;

GAME_NAMESPACE_BEGIN

class MasterModelObserver : public GameBoardObserver
{
	friend class UIGameBoardBase;

	MasterModelObserver(UIGameBoardBase* game_board)
		: game_board_(game_board)
	{}

	virtual void OnGameActionDone(const GameActionResult& result) override
	{
		game_board_->OnMasterDone(result);
	}

	virtual void OnGameActionUndone(const GameActionResult& result) override
	{
		game_board_->OnMasterUndone(result);
	}

	virtual void OnGameActionRedone(const GameActionResult& result) override
	{
		game_board_->OnMasterRedone(result);
	}

	UIGameBoardBase* game_board_;
};

class LocalModelObserver : public GameBoardObserver
{
	friend class UIGameBoardBase;

	LocalModelObserver(UIGameBoardBase* game_board)
		: game_board_(game_board)
	{}

	virtual void OnGameActionDone(const GameActionResult& result) override
	{
		game_board_->OnLocalDone(result);
	}

	virtual void OnGameActionUndone(const GameActionResult& result) override
	{
		game_board_->OnLocalUndone(result);
	}

	virtual void OnGameActionRedone(const GameActionResult& result) override
	{
		game_board_->OnLocalRedone(result);
	}

	UIGameBoardBase* game_board_;
};

UIGameBoardBase::UIGameBoardBase(QWidget* parent)
: QWidget(parent)
{}

UIGameBoardBase::~UIGameBoardBase()
{
	if (master_model_observer_)
	{
		if (master_model_)
		{
			master_model_->RemoveGameObserver(master_model_observer_.get());
		}
	}

	if (local_model_observer_)
	{
		if (local_model_)
		{
			local_model_->RemoveGameObserver(local_model_observer_.get());
		}
	}
}

void UIGameBoardBase::SetMasterModel(const GameBoardPtr& model)
{
	if (master_model_ && master_model_observer_)
	{
		master_model_->RemoveGameObserver(master_model_observer_.get());
	}

	master_model_ = model;

	if (!master_model_observer_)
	{
		master_model_observer_.reset(new MasterModelObserver(this));
	}

	if (master_model_)
	{
		master_model_->AddGameObserver(master_model_observer_.get());
	}

	if (command_handler_)
	{
		command_handler_->DelegateMasterGameBoard(master_model_);
	}
}

void UIGameBoardBase::SetLocalModel(const GameBoardPtr& model)
{
	if (local_model_ && local_model_observer_)
	{
		local_model_->RemoveGameObserver(local_model_observer_.get());
	}

	local_model_ = model;

	if (!local_model_observer_)
	{
		local_model_observer_.reset(new LocalModelObserver(this));
	}
	
	if (local_model_)
	{
		local_model_->AddGameObserver(local_model_observer_.get());
	}

	if (command_handler_)
	{
		command_handler_->DelegateLocalGameBoard(local_model_);
	}
}

void UIGameBoardBase::SyncLocalWithMaster()
{
	if (!master_model_ || !local_model_) { return; }

	local_model_->SyncCurrentGame(*master_model_);
}

void UIGameBoardBase::RefreshBoardWithMaster()
{
	
}

void UIGameBoardBase::OnMasterUndone(const GameActionResult& result)
{
	nbase::ValueGuard<bool> guard(force_to_play_action_result_immediately_);
	this->SetForcingToPlayActionResultImmediately(true);

	this->OnMasterDone(result);
}

void UIGameBoardBase::OnMasterRedone(const GameActionResult& result)
{
	nbase::ValueGuard<bool> guard(force_to_play_action_result_immediately_);
	this->SetForcingToPlayActionResultImmediately(true);

	this->OnMasterDone(result);
}

void UIGameBoardBase::OnLocalUndone(const GameActionResult& result)
{
	nbase::ValueGuard<bool> guard(force_to_play_action_result_immediately_);
	this->SetForcingToPlayActionResultImmediately(true);

	this->OnLocalDone(result);
}

void UIGameBoardBase::OnLocalRedone(const GameActionResult& result)
{
	nbase::ValueGuard<bool> guard(force_to_play_action_result_immediately_);
	this->SetForcingToPlayActionResultImmediately(true);

	this->OnLocalDone(result);
}

bool UIGameBoardBase::NeedToPlayActionResultImmediately(const GameActionResult& result)
{
	bool ret = true;

	do
	{
		if (IsForcingToPlayActionResultImmediately()) { break; }

		if (animation_layer_.isNull()) { break; }

		const auto cmd_type = result.GetCommandTypeOfAction();
		if (cmd_type == proto::CommandType::StudentMoveChess || cmd_type == proto::CommandType::TeacherMoveChess)
		{
			ret = false;
		}

	} while (false);

	return ret;
}

void UIGameBoardBase::UpdateBasketWithModel(const GameBoardPtr& model)
{
	if (chess_basket_ && model)
	{
		GameMode game_mode{ GameMode::Teaching };
		VSPlayerInfo black_player;
		VSPlayerInfo white_player;
		PlayerSide current_side{ PlayerSide::None };
		model->VSInfoOfCurrentGame(game_mode, &black_player, &white_player, &current_side);

		chess_basket_->SetGameVSInfo(game_mode, &black_player, &white_player, &current_side);
	}
}

void UIGameBoardBase::SetCommandHandler(const CommandHandlerSharedPtr& command_handler)
{
	command_handler_ = command_handler;

	command_handler_->DelegateMasterGameBoard(master_model_);

	command_handler_->DelegateLocalGameBoard(local_model_);
}

void UIGameBoardBase::AssociateAnimationLayer(UIAnimationLayer* animation_layer)
{
	animation_layer_ = animation_layer;
}

void UIGameBoardBase::AssociateChessBasket(UIChessBasketBase* chess_basket)
{
	chess_basket_ = chess_basket;
}

void UIGameBoardBase::AssociateOperationalLayer(UIOperationalLayer* operational_layer)
{
	operational_layer_ = operational_layer;

	this->OnOperationalLayerAssociated();
}

void UIGameBoardBase::AssociateTipHelper(TipHelper* tip_helper)
{
	tip_helper_ = tip_helper;
}

void UIGameBoardBase::TryToCacheCurrentGameOfMasterModel()
{
	if (master_model_) { master_model_->TryToSaveCurrentGame(); }
}

bool UIGameBoardBase::TryToHandleCommandOnLocalModel(const CommandSharedPtr& cmd_ptr, std::string* failed_info)
{
	return local_model_ ? local_model_->TryToDoCommand(cmd_ptr, true, failed_info) : false;
}

void UIGameBoardBase::TryToRefreshLocal()
{
	if (IsLocalDirty())
	{
		SyncLocalWithMaster();

		SetLocalDirty(false);
	}
}

bool UIGameBoardBase::TryToMoveChessLocally(ChessId chess_id, const GridIndex& grid_from, const GridIndex& grid_to, std::string* failed_info)
{
	bool ret = false;

	CommandSharedPtr cmd_ptr;

	switch (GetClientType())
	{
	case ClientType::Student:
		cmd_ptr = proto::CmdStudentMoveChess::Create();
		break;

	case ClientType::Teacher:
		cmd_ptr = proto::CmdTeacherMoveChess::Create();
		break;

	default:
		break;
	}

	if (cmd_ptr)
	{
		cmd_ptr->PushVariants(int(chess_id), int(grid_from.X()), int(grid_from.Y()), int(grid_to.X()), int(grid_to.Y()));

		ret = TryToHandleCommandOnLocalModel(cmd_ptr, failed_info);
		if (!ret)
		{
			this->ClearOperationalLayer();
		}
	}

	return ret;
}

void UIGameBoardBase::SendCommand(const CommandSharedPtr& cmd_ptr)
{
	if (command_handler_)
	{
		command_handler_->SendCommand(cmd_ptr);
	}
}

ChessId UIGameBoardBase::LocalChessIdAt(const GridIndex& grid_from) const
{
	auto chess_info = local_model_? local_model_->ChessAt(grid_from) : nullptr;

	return chess_info? chess_info->GetId() : ChessInfo::EmptyId;
}

void UIGameBoardBase::LocalVSInfoOfCurrentGame(GameMode& game_mode,
											   VSPlayerInfo* black_player,
											   VSPlayerInfo* white_player,
											   PlayerSide* current_side)
{
	if (local_model_)
	{
		local_model_->VSInfoOfCurrentGame(game_mode, black_player, white_player, current_side);
	}
}

bool UIGameBoardBase::HasAnimationPlaying() const
{
	return !animation_layer_.isNull() && animation_layer_->HasAnimationPlaying();
}

void UIGameBoardBase::HideChessAt(const GridIndex& grid_index)
{
	auto chess_piece = this->ChessPieceAt(grid_index);
	if (chess_piece)
	{
		chess_piece->HideFace();
	}
}

void UIGameBoardBase::ShowChessAt(const GridIndex& grid_index)
{
	auto chess_piece = this->ChessPieceAt(grid_index);
	if (chess_piece && chess_piece->ChessIdIsNotEmpty())
	{
		chess_piece->ShowFace();
	}
}

void UIGameBoardBase::SetLayout(const GameBoardLayout layout)
{
	if (layout_ != layout)
	{
		layout_ = layout;

		this->OnLayoutChanged();
	}
}

void UIGameBoardBase::HandleMasterCommandSpecially(const CommandSharedPtr& cmd_ptr)
{
	const auto cmd_type = cmd_ptr->Type();

	if (cmd_type == proto::CommandType::TeacherShowBoardRule)
	{
		this->SetScaleLineVisible(static_cast<bool>(cmd_ptr->IntAt(0)));
	}
}

void UIGameBoardBase::Reset()
{
	if (master_model_) { master_model_->Reset(); }
	if (local_model_) { local_model_->Reset(); }
	this->SetLocalDirty(false);
	if (animation_layer_) { animation_layer_->Clear(); }
}

void UIGameBoardBase::ClearOperationalLayer()
{
	if (operational_layer_) { operational_layer_->Clear(); }
}

void UIGameBoardBase::TipOnToast(const QString& tip)
{
	if (tip_helper_) { tip_helper_->TipOnToast(tip); }
}

void UIGameBoardBase::TipOnMsgBox(const QString& tip)
{
	if (tip_helper_) { tip_helper_->TipOnMsgBox(tip); }
}

void UIGameBoardBase::CloseTipMsgBox()
{
	if (tip_helper_) { tip_helper_->CloseMsgBox(); }
}

void UIGameBoardBase::PlaySoundByActionResult(const GameActionResult& result)
{
	bool chess_moved_into = false;
	bool chess_moved_inside = false;
	bool chess_moved_outof = false;
	bool chess_captured = false;

	result.ForeachChessVariation([&](const ChessVariation& var){
		chess_moved_into |= var.IsMovedIntoBoardActively();
		chess_moved_inside |= var.IsMovedInsideOfBoardActively();
		chess_moved_outof |= var.IsMovedOutOfBoardActively();
		chess_captured |= var.IsCapturedPassively() && ChessIdIsNotEmpty(var.GetOldChessId());
	});

	do 
	{
		if (chess_captured)
		{
			this->PlaySoundWhenChessCaptured();
			break;
		}

		if (chess_moved_outof)
		{
			this->PlaySoundWhenChessMovedOutOfBoard();
			break;
		}

		if (chess_moved_into)
		{
			this->PlaySoundWhenChessMovedIntoBoard();
			break;
		}

		if (chess_moved_inside)
		{
			this->PlaySoundWhenChessMovedInsideOfBoard();
			break;
		}

	} while (false);
}

void UIGameBoardBase::PlaySoundWhenChessMovedIntoBoard()
{
	if (tip_helper_) { tip_helper_->PlaySoundWhenChessMovedIntoBoard(); }
}

void UIGameBoardBase::PlaySoundWhenChessMovedInsideOfBoard()
{
	if (tip_helper_) { tip_helper_->PlaySoundWhenChessMovedInsideOfBoard(); }
}

void UIGameBoardBase::PlaySoundWhenChessMovedOutOfBoard()
{
	if (tip_helper_) { tip_helper_->PlaySoundWhenChessMovedOutOfBoard(); }
}

void UIGameBoardBase::PlaySoundWhenChessCaptured()
{
	if (tip_helper_) { tip_helper_->PlaySoundWhenChessCaptured(); }
}

GAME_NAMESPACE_END