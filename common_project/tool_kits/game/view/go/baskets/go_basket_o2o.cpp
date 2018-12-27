#include "go_basket_o2o.h"
#include "common/uiutils/ui_utils.h"
#include "game/GeneratedFiles/ui_go_basket_o2o.h"
#include "game/model/go/go_trait.h"
#include "game/public/literal_stock/literal_stock.h"
#include "game/public/public_setting/public_setting.h"
#include "game/view/widget_with_pixmap/widget_with_pixmap.h"
#include "game/view/go/go_game_board_wrapper.h"
#include "go_color_basket.h"
#include "go_piece_in_basket.h"

GAME_NAMESPACE_BEGIN

UIGoBasketO2O::UIGoBasketO2O(QWidget* parent) 
: UIGoBasketBase(parent)
, ui_(new Ui::GoBasketO2O)
{
	ui_->setupUi(this);
}

void UIGoBasketO2O::Init()
{
	UIGoBasketBase::Init();

	this->setFixedSize(InitialSize());

	ui_->lb_title->setObjectName("game_lb_16_ffffff");
	ui_->lb_title->setText(QStringByKey(LK_TEACHING_MODE));

	ui_->wdgt_space_under_titile->setFixedHeight(10);

	ui_->top_player_wrapper->setFixedHeight(ui_->top_player->height());
	ui_->wdgt_space_under_top_player->setFixedHeight(10);
	ui_->wdgt_space_over_bottom_player->setFixedHeight(10);
	ui_->bottom_player_wrapper->setFixedHeight(ui_->bottom_player->height());

	ui_->wdgt_space_between_side->setFixedHeight(10);

	ui_->top_player->hide();
	ui_->bottom_player->hide();

	ui_->layout_top_basket->addWidget(white_basket_);
	ui_->layout_bottom_basket->addWidget(black_basket_);

	ui_->chk_auto_taking_pieces->setObjectName("game_chk_auto_taking_pieces");
	ui_->chk_auto_taking_pieces->setVisible(ClientTypeIsTeacher());
	ui_->lb_auto_taking_pieces->setObjectName("game_lb_14_f7d8c8");
	ui_->lb_auto_taking_pieces->setVisible(ClientTypeIsTeacher());

	QString mask_object_name("game_wgt_000a45p_r15");
	black_side_wrapper_.Reset(ui_->bottom_player, CreateTurnMask(mask_object_name));
	white_side_wrapper_.Reset(ui_->top_player, CreateTurnMask(mask_object_name));
}

void UIGoBasketO2O::SetBasketEnabled(bool enabled)
{
	UIChessBasketBase::SetBasketEnabled(enabled);

	this->SetDisabledFlagVisible(!enabled);
}

void UIGoBasketO2O::SetDisabledFlagVisible(bool visible)
{
	white_basket_->SetDisabledFlagVisible(visible);
	black_basket_->SetDisabledFlagVisible(visible);
}

std::map<ChessId, QRect>  UIGoBasketO2O::ChessesGlobalRect() const
{
	std::map<ChessId, QRect> id2Rects;

	id2Rects.emplace(GoTrait::IdOfWhitePiece, white_basket_->ChessGlobalArea());
	id2Rects.emplace(GoTrait::IdOfBlackPiece, black_basket_->ChessGlobalArea());

	return id2Rects;
}

QPoint  UIGoBasketO2O::GlobalPositionOfChessCenter(ChessId chess_id) const
{
	QPoint pos = this->mapToGlobal(QPoint(0, 0));

	if (chess_id == GoTrait::IdOfWhitePiece)
	{
		pos = white_basket_->GlobalPosOfChessAreaCenter();
	}
	else if (chess_id == GoTrait::IdOfBlackPiece)
	{
		pos = black_basket_->GlobalPosOfChessAreaCenter();
	}

	return pos;
}

void UIGoBasketO2O::SetAutoTakingPieces(bool auto_taking_pieces)
{
	ui_->chk_auto_taking_pieces->setChecked(auto_taking_pieces);
	ui_->lb_auto_taking_pieces->setVisible(ClientTypeIsTeacher() || auto_taking_pieces);
}

void UIGoBasketO2O::SetGameVSInfo(GameMode game_mode, 
								  const VSPlayerInfo* black_player, 
								  const VSPlayerInfo* white_player, 
								  const PlayerSide* current_side)
{
	do
	{
		if (game_mode == current_game_mode_ && current_game_mode_ == GameMode::Teaching) { break; }

		bool is_vs_mode = (game_mode == GameMode::VS);

		if (current_game_mode_ != game_mode)
		{
			current_game_mode_ = game_mode;

			ui_->lb_title->setText(QStringByKey(is_vs_mode ? LK_VS_MODE : LK_TEACHING_MODE));
			this->SetDisabledFlagVisible(is_vs_mode || IsBasketDisabled());
		}

		this->CancelFixedChessId();
		ui_->wdgt_auto_taking_pieces_wrapper->setVisible(!is_vs_mode);
		black_side_wrapper_.SetPlayeBarVisible(is_vs_mode);
		white_side_wrapper_.SetPlayeBarVisible(is_vs_mode);
		black_side_wrapper_.HideTurnMask();
		white_side_wrapper_.HideTurnMask();

		this->updateGeometry();

		if (!is_vs_mode) { break; }

		if (black_player) { black_side_wrapper_.SetPlayerInfo(*black_player); }
		if (white_player) { white_side_wrapper_.SetPlayerInfo(*white_player); }

		if (current_side)
		{
			black_side_wrapper_.SetTurnMaskVisible(*current_side == PlayerSide::White);
			white_side_wrapper_.SetTurnMaskVisible(*current_side == PlayerSide::Black);

			if (black_player->Client() == GetClientType() || white_player->Client() == GetClientType())
			{
				this->FixChessId(current_board_grids_, *current_side == PlayerSide::White ? GoTrait::IdOfWhitePiece : GoTrait::IdOfBlackPiece);
			}
		}

	} while (false);
}

void UIGoBasketO2O::Reset()
{
	this->SetBasketEnabled(false);

	this->SetGameVSInfo(GameMode::Teaching);

	this->SetAutoTakingPieces(false);
}

void UIGoBasketO2O::OnColorBasketClicked(ChessId chess_id)
{
	if (IsBasketEnabled() && GameMode::Teaching == current_game_mode_ && !CurrentGoBoardWrapper.isNull())
	{
		auto type = (ChessIdFixed() == chess_id) ?  ChessIdGeneratorType::Alternate : 
			(GoTrait::IdOfBlackPiece == chess_id ? ChessIdGeneratorType::OnlyBlack : ChessIdGeneratorType::OnlyWhite);

		CurrentGoBoardWrapper->SwitchChessIdGenerator(type);
	}
}

GAME_NAMESPACE_END