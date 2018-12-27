#include "go_basket_base.h"
#include "game/model/go/go_enum.h"
#include "game/model/go/go_trait.h"
#include "go_color_basket.h"

GAME_NAMESPACE_BEGIN

UIGoBasketBase::UIGoBasketBase(QWidget* parent) 
: UIChessBasketBase(parent) 
{}

void UIGoBasketBase::Init()
{
	UIChessBasketBase::Init();

	white_basket_ = new UIGoColorBasket(GoColor::White, this);
	black_basket_ = new UIGoColorBasket(GoColor::Black, this);

	connect(white_basket_, &UIGoColorBasket::Clicked, this, [=]{ this->OnColorBasketClicked(GoTrait::IdOfWhitePiece); });
	connect(black_basket_, &UIGoColorBasket::Clicked, this, [=]{ this->OnColorBasketClicked(GoTrait::IdOfBlackPiece); });
}

void UIGoBasketBase::FixChessId(const int grid, const ChessId chess_id)
{
	chess_id_fixed_ = chess_id;

	white_basket_->LazyCreateGoPieces(this);
	black_basket_->LazyCreateGoPieces(this);

	white_basket_->HideFixedChesses();
	black_basket_->HideFixedChesses();

	if (GoTrait::IdOfWhitePiece == chess_id)
	{
		white_basket_->ShowFixedChess(grid);
	}
	else
	{
		black_basket_->ShowFixedChess(grid);
	}
}

void UIGoBasketBase::CancelFixedChessId()
{
	chess_id_fixed_ = ChessInfo::EmptyId;

	white_basket_->HideFixedChesses();
	black_basket_->HideFixedChesses();
}

void UIGoBasketBase::SwitchToGameBoard(GameBoardType board_type)
{
	UIChessBasketBase::SwitchToGameBoard(board_type);
	
	switch (CurrentGameBoardType())
	{
	case GameBoardType::Go9:
		current_board_grids_ = 9;
		break;

	case GameBoardType::Go13:
		current_board_grids_ = 13;
		break;

	case GameBoardType::Go19:
		current_board_grids_ = 19;
		break;

	default:
		break;
	}
}

void UIGoBasketBase::resizeEvent(QResizeEvent *)
{
	auto black_rect = black_basket_->ChessGlobalArea();
	black_side_wrapper_.SetGeometryOfTurnMask(QRect(this->mapFromGlobal(black_rect.topLeft()), black_rect.size()));

	auto white_rect = white_basket_->ChessGlobalArea();
	white_side_wrapper_.SetGeometryOfTurnMask(QRect(this->mapFromGlobal(white_rect.topLeft()), white_rect.size()));

	white_basket_->AdjustGeometryOfGoPieces();
	black_basket_->AdjustGeometryOfGoPieces();
}

void UIGoBasketBase::OnColorBasketClicked(ChessId)
{}

GAME_NAMESPACE_END