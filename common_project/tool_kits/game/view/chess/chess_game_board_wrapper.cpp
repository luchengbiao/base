#include "chess_game_board_wrapper.h"
#include "common/uiutils/ui_utils.h"
#include "game/model/chess/chess_game_board.h"
#include "game/model/chess/chess_trait.h"
#include "game/model/command_handler/command_handler.h"
#include "game/model/command_handler/game_board_wrapper.h"
#include "game/public/literal_stock/literal_stock.h"
#include "game/public/public_setting/public_setting.h"
#include "game/public/structs/student_info_in_class/student_info_in_class.h"
#include "game/view/animation/animation_layer.h"
#include "game/view/chess/baskets/chess_basket_o2o.h"
#include "game/view/chess/operational/chess_operational_layer.h"
#include "game/view/chess/chess_piece.h"
#include "protocol/command/base/command.h"
#include "chess_game_board_ui.h"

using namespace proto;

GAME_NAMESPACE_BEGIN

class UIChessGameBoardWrapper::OperationalDelegate : public ChessOperationalDelegate
{
public:
	virtual bool BoardIsWithRule() override
	{
		return board_wrapper_->game_board_->LocalGameBoard()->IsRuleBased();
	}

	virtual bool TestToOperateOnBoardNow(const QPoint& global_mouse_pos) override
	{
		bool ret = false;
		QString info_to_hint;

		do
		{
			auto& game_board = board_wrapper_->game_board_;

			if (game_board->HasAnimationPlaying()) { break; }

			if (UsageTypeIsClass())
			{
				if (ClientTypeIsStudent())
				{
					if (!BothStuInClass.IsAdded(AccountId()))
					{
						break;
					}
				}
			}

			game_board->TryToRefreshLocal();

			bool is_vs_mode = game_board->IsVSMode();

			if (is_vs_mode)
			{
				if (game_board->IsLocalGameOver(nullptr, &info_to_hint))
				{
					if (info_to_hint.isEmpty())
					{
						info_to_hint = QStringByKey(LK_GAME_IS_OVER);
					}

					break;
				}

				GameMode game_mode = GameMode::VS;
				VSPlayerInfo black_player;
				VSPlayerInfo white_player;
				PlayerSide current_side = PlayerSide::None;
				game_board->LocalVSInfoOfCurrentGame(game_mode, &black_player, &white_player, &current_side);

				if (UsageTypeIsClass())
				{
					black_player = BothStuInClass.BlackSide();
					white_player = BothStuInClass.WhiteSide();
				}

				auto& current_player = (PlayerSide::Black == current_side) ? black_player : white_player;
				auto& opposide_player = (PlayerSide::Black == current_side) ? white_player : black_player;
				if (current_player.Client() != GetClientType())
				{
					if (UiUtils::WidgetContainsGlobalPosition(game_board, global_mouse_pos))
					{
						if (opposide_player.Client() == GetClientType())
						{
							info_to_hint = QStringByKey(LK_WAITING_OPPOSITE_TO_MOVE);
						}
						else
						{
							info_to_hint = QStringByKey(LK_IS_NOT_A_VS_PLAYER);
						}
					}
					
					break;
				}
			}
			else
			{
				if (ClientTypeIsStudent() && !board_wrapper_->IsGameBoardEnabled()) // teaching mode & student & forbidden to operate
				{
					break;
				}
			}

			ret = true;
		} while (false);

		if (!info_to_hint.isEmpty())
		{
			board_wrapper_->TipOnToast(info_to_hint);
		}

		return ret;
	}

	virtual bool TestToOperateOnThisChess(const GridIndex& grid, ChessId chess_id) override
	{
		bool ret = false;

		do
		{
			auto& game_board = board_wrapper_->game_board_;

			game_board->TryToRefreshLocal();

			bool is_vs_mode = game_board->IsVSMode();

			if (is_vs_mode)
			{
				if (!grid.IsValid()) { break; } // forbid to move chess from basket under vs mode

				GameMode game_mode = GameMode::VS;
				VSPlayerInfo black_player;
				VSPlayerInfo white_player;
				PlayerSide current_side = PlayerSide::None;
				game_board->LocalVSInfoOfCurrentGame(game_mode, &black_player, &white_player, &current_side);

				const auto chess_color = ChessTrait::ColorById(chess_id);
				if (PlayerSide::Black == current_side && ChessColor::Black != chess_color)
				{
					break;
				}

				if (PlayerSide::White == current_side && ChessColor::White != chess_color)
				{
					break;
				}
			}

			ret = true;
		} while (false);

		return ret;
	}

	virtual GridRectInBoardMap GridsGlobalRectInBoard() override
	{
		auto game_board = board_wrapper_->game_board_;
		auto grid_width = game_board->GridWidth();
		QSize grid_size(grid_width, grid_width);

		int rows = game_board->RowsOfBoard();
		int cols = game_board->ColumnsOfBoard();

		std::map<GridIndex, QRect> map_grid_global_rect;
		for (int x = 0; x < cols; ++x)
		{
			for (int y = 0; y < rows; ++y)
			{
				GridIndex grid_index(x, y);
				map_grid_global_rect.emplace(grid_index, QRect(game_board->mapToGlobal(game_board->PositionByGridIndex(grid_index)), grid_size));
			}
		}

		return map_grid_global_rect;
	}

	virtual ChessRectInBasketMap ChessesGlobalRectInBasket() override
	{
		return board_wrapper_->ChessBasket()->ChessesGlobalRect();
	}

	virtual ChessReachableGridMap GridsReachableFrom(const GridIndex& grid_from) override
	{
		auto local_board_model = board_wrapper_->game_board_->LocalGameBoard();

		return local_board_model->GridsReachableFrom(grid_from);
	}

	virtual QPixmap	MovingPixmapOfChessId(ChessId chess_id) override
	{
		return QPixmap(QString::fromStdString(ChessTrait::MovingImagePathById(chess_id)));
	}

	virtual QPixmap	SelectedPixmapOfChessId(ChessId chess_id) override
	{
		return QPixmap(QString::fromStdString(ChessTrait::SelectedImagePathById(chess_id)));
	}

	virtual bool GridContainsChessInBoard(const GridIndex& grid_index, ChessId* chess_id) override
	{
		auto chess_piece = board_wrapper_->game_board_->ChessPieceAt(grid_index);
		
		if (chess_piece)
		{
			auto target_chess_id = chess_piece->GetChessId();
			if (ChessIdIsNotEmpty(target_chess_id))
			{
				if (chess_id) { *chess_id = target_chess_id; }

				return true;
			}
		}

		return false;
	}

	virtual void HideChessInBoardGrid(const GridIndex& grid_index) override
	{
		board_wrapper_->game_board_->HideChessAt(grid_index);
	}

	virtual void ShowChessInBoardGrid(const GridIndex& grid_index) override
	{
		board_wrapper_->game_board_->ShowChessAt(grid_index);
	}

	virtual void OnMouseWillMove(const GridIndex&, ChessId) override
	{
	}

	virtual void HighlightGridInBoard(const GridIndex& grid_index) override
	{
		auto chess_piece = board_wrapper_->game_board_->ChessPieceAt(grid_index);

		if (chess_piece)
		{
			chess_piece->Highlight();
		}
	}

	virtual void UnhighlightGridInBoard(const GridIndex& grid_index) override
	{
		auto chess_piece = board_wrapper_->game_board_->ChessPieceAt(grid_index);

		if (chess_piece)
		{
			chess_piece->Unhighlight();
		}
	}

	virtual bool TryToMoveChessLocally(ChessId chess_id, const GridIndex& grid_from, const GridIndex& grid_to) override
	{
		return board_wrapper_->game_board_->TryToMoveChessLocally(chess_id, grid_from, grid_to);
	}

	OperationalDelegate(UIChessGameBoardWrapper* board_wrapper)
		: board_wrapper_(board_wrapper)
	{}

	UIChessGameBoardWrapper* board_wrapper_{ nullptr };
};

UIChessGameBoardWrapper* UIChessGameBoardWrapper::Create(ClientType client_type, UsageType usage_type, QWidget* parent)
{
	UIChessGameBoardWrapper* board_wrapper = new (std::nothrow) UIChessGameBoardWrapper(client_type, usage_type, parent);

	if (board_wrapper)
	{
		board_wrapper->Init();
	}

	return board_wrapper;
}

UIChessGameBoardWrapperPtr CurrentChessBoardWrapper;

UIChessGameBoardWrapper::UIChessGameBoardWrapper(ClientType client_type, UsageType usage_type, QWidget* parent)
: UIGameBoardWrapper(client_type, usage_type, parent)
{
	CurrentChessBoardWrapper = this;

	PublicSetting::SetGameType(GameType::Chess);
}

UIChessGameBoardWrapper::~UIChessGameBoardWrapper() = default;

void UIChessGameBoardWrapper::Init()
{
	UIGameBoardWrapper::Init();

	game_board_ = static_cast<UIChessGameBoard*>(this->CurrentGameBoard());
	animation_layer_ = this->CurrentAnimationLayer();

	this->ZOrderUi();
}

GameBoardVector UIChessGameBoardWrapper::CreateGameBoards()
{
	GameBoardVector vec;

	GameRecordsPtr all_games_to_be_cached = std::make_shared<GameRecords>();
	vec.emplace_back(UIChessGameBoard::Create(all_games_to_be_cached, this));

	return vec;
}

UIChessBasketBase* UIChessGameBoardWrapper::CreateChessBasket()
{
	UIChessBasketBase* chess_basket = nullptr;

	if (UsageTypeIsO2O())
	{
		chess_basket = game::CreateChessBasket<UIChessBasketO2O>(this);
	}

	return chess_basket;
}

UIOperationalLayer* UIChessGameBoardWrapper::InnerCreateOperationalLayer(QWidget* presentation_layer)
{
	auto operational_layer = new UIChessOperationalLayer(this, presentation_layer);

	operational_delegate_.reset(new OperationalDelegate(this));

	operational_layer->SetOperationalDelegate(operational_delegate_.get());

	return operational_layer;
}

void UIChessGameBoardWrapper::HandleMasterCommandSpecially(const CommandSharedPtr& cmd_ptr)
{
	UIGameBoardWrapper::HandleMasterCommandSpecially(cmd_ptr);
}

GAME_NAMESPACE_END