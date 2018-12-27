#include "go_game_board_wrapper.h"
#include "common/uiutils/ui_utils.h"
#include "game/model/go/go_game_board.h"
#include "game/model/command_handler/command_handler.h"
#include "game/model/command_handler/game_board_wrapper.h"
#include "game/public/literal_stock/literal_stock.h"
#include "game/public/public_setting/public_setting.h"
#include "game/public/structs/student_info_in_class/student_info_in_class.h"
#include "game/view/animation/animation_layer.h"
#include "game/view/go/baskets/go_basket_o2o.h"
#include "game/view/go/chess_id_generator/chess_id_generator.h"
#include "game/view/go/operational/go_operational_layer.h"
#include "game/view/go/go_piece/go_piece.h"
#include "go_game_board_ui.h"
#include "protocol/command/base/command.h"

using namespace proto;

GAME_NAMESPACE_BEGIN

class UIGoGameBoardWrapper::OperationalDelegate : public GoOperationalDelegate
{
public:
	virtual bool TestToOperateOnBoardNow(const QPoint& global_mouse_pos) override
	{
		bool ret = false;
		QString info_to_hint;

		do
		{
			auto game_board = board_wrapper_->CurrentGameBoard();

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
			auto game_board = board_wrapper_->CurrentGameBoard();

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

				if (PlayerSide::Black == current_side && chess_id != GoTrait::IdOfBlackPiece)
				{
					break;
				}

				if (PlayerSide::White == current_side && chess_id != GoTrait::IdOfWhitePiece)
				{
					break;
				}
			}

			ret = true;
		} while (false);

		return ret;
	}

	virtual int GridsOfBoard() override
	{
		return board_wrapper_->CurrentGameBoard()->RowsOfBoard();
	}

	virtual GridRectInBoardMap GridsGlobalRectInBoard() override
	{
		auto game_board = board_wrapper_->CurrentGameBoard();
		auto grid_width = game_board->GridWidth();
		QSize grid_size(grid_width, grid_width);
		QPoint offset(grid_width / 2, grid_width  / 2);

		int rows = game_board->RowsOfBoard();
		int cols = game_board->ColumnsOfBoard();

		std::map<GridIndex, QRect> map_grid_global_rect;
		for (int x = 0; x < cols; ++x)
		{
			for (int y = 0; y < rows; ++y)
			{
				GridIndex grid_index(x, y);
				auto pos = game_board->PositionByGridIndex(grid_index) - offset;
				map_grid_global_rect.emplace(grid_index, QRect(game_board->mapToGlobal(pos), grid_size));
			}
		}

		return map_grid_global_rect;
	}

	virtual ChessRectInBasketMap ChessesGlobalRectInBasket() override
	{
		return board_wrapper_->ChessBasket()->ChessesGlobalRect();
	}

	virtual bool GridContainsChessInBoard(const GridIndex& grid_index, ChessId* chess_id) override
	{
		auto go_piece = board_wrapper_->CurrentGameBoard()->ChessPieceAt(grid_index);

		if (go_piece && go_piece->ChessIdIsNotEmpty())
		{
			if (chess_id) { *chess_id = go_piece->GetChessId(); }

			return true;
		}

		return false;
	}

	virtual ChessId NextChessIdToMoveIntoBoard() override
	{
		return board_wrapper_->NextChessIdToMoveIn();
	}

	virtual bool TryToMoveChessLocally(ChessId chess_id, const GridIndex& grid_from, const GridIndex& grid_to, std::string* failed_info = nullptr) override
	{
		return board_wrapper_->CurrentGameBoard()->TryToMoveChessLocally(chess_id, grid_from, grid_to, failed_info);
	}

	virtual	void TipOnToast(const QString& tip) override
	{
		board_wrapper_->TipOnToast(tip);
	}

	OperationalDelegate(UIGoGameBoardWrapper* board_wrapper)
		: board_wrapper_(board_wrapper)
	{}

	UIGoGameBoardWrapper* board_wrapper_{ nullptr };
};

UIGoGameBoardWrapper* UIGoGameBoardWrapper::Create(ClientType client_type, UsageType usage_type, QWidget* parent)
{
	UIGoGameBoardWrapper* board_wrapper = new (std::nothrow) UIGoGameBoardWrapper(client_type, usage_type, parent);

	if (board_wrapper)
	{
		board_wrapper->Init();
	}

	return board_wrapper;
}

UIGoGameBoardWrapperPtr CurrentGoBoardWrapper;

UIGoGameBoardWrapper::UIGoGameBoardWrapper(ClientType client_type, UsageType usage_type, QWidget* parent)
: UIGameBoardWrapper(client_type, usage_type, parent)
, chess_id_generator_(new ChessIdGeneratorHelper(GoTrait::IdOfBlackPiece))
{
	CurrentGoBoardWrapper = this;

	PublicSetting::SetGameType(GameType::Go);
}

UIGoGameBoardWrapper::~UIGoGameBoardWrapper() = default;

void UIGoGameBoardWrapper::Init()
{
	UIGameBoardWrapper::Init();

	this->SwitchMasterToGameBoard(GameBoardType::Go19);

	this->ZOrderUi();
}

GameBoardVector UIGoGameBoardWrapper::CreateGameBoards()
{
	GameBoardVector vec;

	GameRecordsPtr all_games_to_be_cached = std::make_shared<GameRecords>();

	int grids[3]{9, 13, 19};
	for (int i = 0; i < 3; ++i)
	{
		vec.emplace_back(UIGoGameBoard::Create(grids[i], all_games_to_be_cached, this));
	}

	return vec;
}

UIChessBasketBase* UIGoGameBoardWrapper::CreateChessBasket()
{
	if (UsageTypeIsO2O())
	{
		go_basket_ = game::CreateChessBasket<UIGoBasketO2O>(this);
	}

	return go_basket_;
}

UIOperationalLayer* UIGoGameBoardWrapper::InnerCreateOperationalLayer(QWidget* presentation_layer)
{
	auto operational_layer = new UIGoOperationalLayer(this, presentation_layer);

	operational_delegate_.reset(new OperationalDelegate(this));

	operational_layer->SetOperationalDelegate(operational_delegate_.get());

	return operational_layer;
}

void UIGoGameBoardWrapper::HandleMasterCommandSpecially(const CommandSharedPtr& cmd_ptr)
{
	UIGameBoardWrapper::HandleMasterCommandSpecially(cmd_ptr);

	const auto cmd_tpye = cmd_ptr->Type();

	switch (cmd_tpye)
	{
	case CommandType::TeacherChangeGameBoard:
	case CommandType::TeacherNewGameBoard:
	case CommandType::TeacherNewGameBoardEx:
	case CommandType::TeacherSwitchToHistoryBoard:
	case CommandType::TeacherImportChessManual:
		if (ClientTypeIsStudent())
		{
			if (operational_layer_)
			{
				operational_layer_->Interrupt();
			}

			this->SwitchChessIdGenerator(ChessIdGeneratorType::Alternate);
		}
		break;

	default:
		break;
	}
}

void UIGoGameBoardWrapper::ResetForRecovery()
{
	UIGameBoardWrapper::ResetForRecovery();

	this->SwitchMasterToGameBoard(GameBoardType::Go19);
}

int UIGoGameBoardWrapper::CurrentBoardGrids() const
{
	auto go_board = static_cast<UIGoGameBoard*>(CurrentGameBoard());

	return go_board ? go_board->RowsOfBoard() : 0;
}

ChessIdGeneratorType UIGoGameBoardWrapper::CurrentChessIdGeneratorType() const
{
	return chess_id_generator_->CurrentGeneratorType();
}

void UIGoGameBoardWrapper::SwitchChessIdGenerator(ChessIdGeneratorType type)
{
	switch (type)
	{
	case game::ChessIdGeneratorType::OnlyBlack:
		go_basket_->FixChessId(CurrentBoardGrids(), GoTrait::IdOfBlackPiece);
		break;

	case game::ChessIdGeneratorType::OnlyWhite:
		go_basket_->FixChessId(CurrentBoardGrids(), GoTrait::IdOfWhitePiece);
		break;

	default:
		go_basket_->CancelFixedChessId();
		break;
	}

	chess_id_generator_->Reset(GoTrait::IdOfWhitePiece, type);
}

ChessId UIGoGameBoardWrapper::NextChessIdToMoveIn()
{
	ChessId chess_id = ChessInfo::EmptyId;
	auto go_board = static_cast<UIGoGameBoard*>(CurrentGameBoard());

	if (go_board->IsRuleBased())
	{
		go_board->TryToRefreshLocal();

		chess_id = go_board->LocalGameBoard()->NextChessIdToMoveInWithRule();
	}
	else
	{
		chess_id = chess_id_generator_->NextChessId();
	}

	return chess_id;
}

GAME_NAMESPACE_END