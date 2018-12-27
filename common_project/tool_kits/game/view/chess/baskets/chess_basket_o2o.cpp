#include "chess_basket_o2o.h"
#include "common/uiutils/ui_utils.h"
#include "game/GeneratedFiles/ui_chess_basket_o2o.h"
#include "game/public/literal_stock/literal_stock.h"
#include "game/public/public_setting/public_setting.h"
#include "game/view/widget_with_pixmap/widget_with_pixmap.h"
#include "chess_piece_in_basket.h"

GAME_NAMESPACE_BEGIN

UIChessBasketO2O::UIChessBasketO2O(QWidget* parent) 
: UIChessBasketBase(parent)
, ui_(new Ui::ChessBasketO2O)
{
	ui_->setupUi(this);
}

void UIChessBasketO2O::Init()
{
	UIChessBasketBase::Init();

	this->setFixedSize(InitialSize());

	ui_->lb_title->setObjectName("game_lb_16_ffffff");
	ui_->lb_title->setText(QStringByKey(LK_TEACHING_MODE));

	ui_->wdgt_space_under_titile->setFixedHeight(10);

	ui_->top_player_wrapper->setFixedHeight(ui_->top_player->height());
	ui_->wdgt_space_under_top_player->setFixedHeight(10);
	ui_->wdgt_space_over_bottom_player->setFixedHeight(10);
	ui_->bottom_player_wrapper->setFixedHeight(ui_->bottom_player->height());

	ui_->wdgt_space_between_side->setFixedHeight(2);

	ui_->top_player->hide();
	ui_->bottom_player->hide();

	ChessId black_chess_ids[2][3] = 
	{
		{ ChessTrait::IdOfBlackPawn, ChessTrait::IdOfBlackRook, ChessTrait::IdOfBlackKnight },
		{ ChessTrait::IdOfBlackBishop, ChessTrait::IdOfBlackQueen, ChessTrait::IdOfBlackKing },
	};
	ChessId white_chess_ids[2][3] =
	{
		{ ChessTrait::IdOfWhitePawn, ChessTrait::IdOfWhiteRook, ChessTrait::IdOfWhiteKnight },
		{ ChessTrait::IdOfWhiteBishop, ChessTrait::IdOfWhiteQueen, ChessTrait::IdOfWhiteKing },
	};

	for (int c = 0; c < 3; ++c) // row
	{
		for (int r = 0; r < 2; ++r) // column
		{
			auto black_chess_piece = new UIChessPieceInBasket(this);
			black_chess_piece->SetChessId(black_chess_ids[r][c]);

			auto white_chess_piece = new UIChessPieceInBasket(this);
			white_chess_piece->SetChessId(white_chess_ids[r][c]);

			ui_->gridLayout_top_chesses->addWidget(black_chess_piece, r, c);
			ui_->gridLayout_bottom_chesses->addWidget(white_chess_piece, r, c);

			chess_pieces_.emplace(black_chess_piece->GetChessId(), black_chess_piece);
			chess_pieces_.emplace(white_chess_piece->GetChessId(), white_chess_piece);
		}
	}

	QString mask_object_name("game_wgt_000a45p_r5");
	black_side_wrapper_.Reset(ui_->top_player, CreateTurnMask(mask_object_name), UsageTypeIsOnline() ? CreateOperationalHookLayer() : nullptr);
	white_side_wrapper_.Reset(ui_->bottom_player, CreateTurnMask(mask_object_name), UsageTypeIsOnline() ? CreateOperationalHookLayer() : nullptr);
}

void UIChessBasketO2O::SetBasketEnabled(bool enabled)
{
	UIChessBasketBase::SetBasketEnabled(enabled);

	this->SetDisabledFlagVisible(!enabled);
}

void UIChessBasketO2O::SetDisabledFlagVisible(bool visible)
{
	for (const auto& pair : chess_pieces_)
	{
		pair.second->SetDisabledFlagVisible(visible);
	}
}

std::map<ChessId, QRect>  UIChessBasketO2O::ChessesGlobalRect() const
{
	std::map<ChessId, QRect> id2Rects;

	for (const auto& pair : chess_pieces_)
	{
		id2Rects.emplace(pair.first, pair.second->GlobalRectOfChessPiece());
	}

	return id2Rects;
}

QPoint  UIChessBasketO2O::GlobalPositionOfChessCenter(ChessId chess_id) const
{
	QPoint pos = this->mapToGlobal(QPoint(0, 0));

	auto it = chess_pieces_.find(chess_id);
	if (it != chess_pieces_.cend())
	{
		pos = it->second->GlobalPositionOfChessPieceCenter();
	}

	return pos;
}

void UIChessBasketO2O::SetGameVSInfo(GameMode game_mode, 
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

		black_side_wrapper_.SetPlayeBarVisible(is_vs_mode);
		white_side_wrapper_.SetPlayeBarVisible(is_vs_mode);
		black_side_wrapper_.HideTurnMask();
		white_side_wrapper_.HideTurnMask();

		if (!is_vs_mode) { break; }
		
		if (black_player) { black_side_wrapper_.SetPlayerInfo(*black_player); }
		if (white_player) { white_side_wrapper_.SetPlayerInfo(*white_player); }

		if (current_side)
		{
			black_side_wrapper_.SetTurnMaskVisible(*current_side == PlayerSide::White);
			white_side_wrapper_.SetTurnMaskVisible(*current_side == PlayerSide::Black);
		}

	} while (false);
}

void UIChessBasketO2O::Reset()
{
	this->SetBasketEnabled(false);

	this->SetGameVSInfo(GameMode::Teaching);
}

void UIChessBasketO2O::resizeEvent(QResizeEvent*)
{
	auto black_chess_lt = ui_->gridLayout_top_chesses->itemAtPosition(0, 0)->widget();
	auto black_chess_rb = ui_->gridLayout_top_chesses->itemAtPosition(1, 2)->widget();
	auto white_chess_lt = ui_->gridLayout_bottom_chesses->itemAtPosition(0, 0)->widget();
	auto white_chess_rb = ui_->gridLayout_bottom_chesses->itemAtPosition(1, 2)->widget();

	auto black_pos_lt = UiUtils::WidgetPosOfLeftTop(black_chess_lt);
	auto black_pos_rb = UiUtils::WidgetPosOfRightBottom(black_chess_rb);
	auto white_pos_lt = UiUtils::WidgetPosOfLeftTop(white_chess_lt);
	auto white_pos_rb = UiUtils::WidgetPosOfRightBottom(white_chess_rb);

	auto black_delta = black_pos_rb - black_pos_lt;
	auto black_geometry = QRect(black_pos_lt, QSize(black_delta.x(), black_delta.y()));
	black_side_wrapper_.SetGeometryOfTurnMask(black_geometry);
	black_side_wrapper_.SetGeometryOfOperationalHookLayer(black_geometry);

	auto white_delta = white_pos_rb - white_pos_lt;
	auto white_geometry = QRect(white_pos_lt, QSize(white_delta.x(), white_delta.y()));
	white_side_wrapper_.SetGeometryOfTurnMask(white_geometry);
	white_side_wrapper_.SetGeometryOfOperationalHookLayer(white_geometry);
}

GAME_NAMESPACE_END