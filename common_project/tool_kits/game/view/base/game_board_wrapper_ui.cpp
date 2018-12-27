#include "game_board_wrapper_ui.h"
#include <QPointer>
#include <QResizeEvent>
#include "base_control/painter/draw_board.h"
#include "base_control/painter/base_mark_widget.h"
#include "common/macros/assert.h"
#include "common/uiutils/ui_utils.h"
#include "game/model/command_handler/command_handler.h"
#include "game/model/message_processor/message_processor.h"
#include "game/public/chess_manaul_helper/chess_manual_helper.h"
#include "game/public/literal_stock/literal_stock.h"
#include "game/public/public_setting/public_setting.h"
#include "game/public/resourse_manager/resourse_manager.h"
#include "game/public/structs/student_info_in_class/student_info_in_class.h"
#include "game/view/animation/animation_layer.h"
#include "game/view/base/chess_basket/chess_basket_base.h"
#include "game/view/base/game_board_base_ui.h"
#include "game/view/base/operational_layer/operational_layer.h"
#include "game/view/create_new_board/create_new_board_dialog.h"
#include "game/view/decorator/decorator_board.h"
#include "game/view/tip_helper/tip_helper.h"
#include "protocol/command/base/command.h"
#include "protocol/command/commands/cmd_teacher_import_chess_manual.h"

using namespace proto;

GAME_NAMESPACE_BEGIN

class GameBoardTuple
{
	typedef  QPointer<UIGameBoardBase>	GameBoardPtr;
	typedef  QPointer<UIDecoratorBoard>	DecoratorBoardPtr;
	typedef  QPointer<UIAnimationLayer>	AnimationLayerPtr;

public:
	inline GameBoardType		BoardType() const { return game_board_ ? game_board_->BoardType() : GameBoardType::None; }
	inline UIGameBoardBase*		GetGameBoard() const { return game_board_; }
	inline DrawBoard*			GetDrawBoard() const { return decorator_board_ ? decorator_board_->GetDrawBoard() : nullptr; }
	inline BaseMarkForm*		GetMarkBoard() const { return decorator_board_ ? decorator_board_->GetMarkBoard() : nullptr; }
	inline UIDecoratorBoard*	GetDecoratorBoard() const { return decorator_board_; }
	inline UIAnimationLayer*	GetAnimationLayer() const { return animatian_layer_; }

	inline void					SetGameBoard(UIGameBoardBase* game_board) { game_board_ = game_board; }
	inline void					SetDecoratorBoard(UIDecoratorBoard* decorator_board) { decorator_board_ = decorator_board; }
	inline void					SetAnimationLayer(UIAnimationLayer* animation_layer) { animatian_layer_ = animation_layer; }

	void Show()
	{
		if (game_board_) { game_board_->show(); }

		if (decorator_board_) { decorator_board_->show(); }

		if (animatian_layer_) { animatian_layer_->show(); }
	}

	void Hide()
	{
		if (game_board_) { game_board_->hide(); }

		if (decorator_board_) { decorator_board_->hide(); }

		if (animatian_layer_) { animatian_layer_->hide(); }
	}

	void Raise()
	{
		if (game_board_) { game_board_->raise(); }

		if (animatian_layer_) { animatian_layer_->raise(); }

		if (decorator_board_) { decorator_board_->raise(); }
	}

	void Lower()
	{
		if (decorator_board_) { decorator_board_->lower(); }

		if (animatian_layer_) { animatian_layer_->lower(); }

		if (game_board_) { game_board_->lower(); }
	}

private:
	GameBoardPtr				game_board_{ nullptr };
	DecoratorBoardPtr			decorator_board_{ nullptr };
	AnimationLayerPtr			animatian_layer_{ nullptr };
};

class GameBoardTuples : private std::vector<GameBoardTuple*>
{
public:
	GameBoardTuples() = default;

	~GameBoardTuples()
	{
		for (auto truple : *this)
		{
			delete truple;
		}
	}

	GameBoardTuples(GameBoardTuples&) = delete;
	GameBoardTuples& operator=(GameBoardTuples) = delete;

	inline GameBoardTuple& Front()
	{
		return *front();
	}

	inline GameBoardTuple& Back()
	{
		return *back();
	}

	inline void Push(UIGameBoardBase* game_board, UIDecoratorBoard* decorator_board, UIAnimationLayer* animation_layer)
	{
		auto truple = new GameBoardTuple;

		truple->SetGameBoard(game_board);
		truple->SetDecoratorBoard(decorator_board);
		truple->SetAnimationLayer(animation_layer);

		this->emplace_back(truple);
	}

	GameBoardTuple* TrupleByType(const GameBoardType board_type) const
	{
		for (auto truple : *this)
		{
			if (truple->BoardType() == board_type)
			{
				return truple;
			}
		}

		return nullptr;
	}

	UIGameBoardBase* GameBoardByType(const GameBoardType board_type) const 
	{ 
		for (auto truple : *this)
		{
			if (truple->BoardType() == board_type)
			{
				return truple->GetGameBoard();
			}
		}

		return nullptr;
	}

	DrawBoard* DrawBoardByType(const GameBoardType board_type) const
	{
		for (auto truple : *this)
		{
			if (truple->BoardType() == board_type)
			{
				return truple->GetDrawBoard();
			}
		}

		return nullptr;
	}

	BaseMarkForm* MarkBoardByType(const GameBoardType board_type) const
	{
		for (auto truple : *this)
		{
			if (truple->BoardType() == board_type)
			{
				return truple->GetMarkBoard();
			}
		}

		return nullptr;
	}

	UIAnimationLayer* AnimationLayerdByType(const GameBoardType board_type) const
	{
		for (auto truple : *this)
		{
			if (truple->BoardType() == board_type)
			{
				return truple->GetAnimationLayer();
			}
		}

		return nullptr;
	}

	inline void Foreach(const std::function<void(GameBoardTuple&)>& callback)
	{
		if (!empty())
		{
			for (auto truple : *this)
			{
				callback(*truple);
			}
		}
	}
};

class UIGameBoardWrapper::MessageCommandObserver : public MessageProcessor::CommandObserver
{
public:
	MessageCommandObserver(UIGameBoardWrapper* board_wrapper)
		: board_wrapper_(board_wrapper)
	{}

	virtual void OnCommand(const CommandSharedPtr& cmd_ptr, bool sender_is_self) override
	{
		if (board_wrapper_->command_handler_)
		{
			board_wrapper_->command_handler_->HandleMasterCommand(cmd_ptr);
		}
	}

	virtual void OnMessage(const std::string& msg, bool sender_is_self) override
	{
		if (board_wrapper_->command_handler_)
		{
			board_wrapper_->command_handler_->HandleChannelMessgae(msg, sender_is_self);
		}
	}

	UIGameBoardWrapper* board_wrapper_{ nullptr };
};

UIGameBoardWrapperPtr CurrentGameBoardWrapper;

UIGameBoardWrapper::UIGameBoardWrapper(ClientType client_type, UsageType usage_type, QWidget* parent)
: QWidget(parent)
, game_board_tuples_(new GameBoardTuples)
{
	CurrentGameBoardWrapper = this;

	ResourceManager::LoadResource();

	D_ASSERT(MyObjectCounter::live() == 1, "only support single one instance of UIGameBoardWrapper-based at one time");

	PublicSetting::SetClientType(client_type);
	PublicSetting::SetUsageType(usage_type);
	BothStuInClass.Reset();

	this->setWindowFlags(Qt::FramelessWindowHint);
	this->setAutoFillBackground(false);
	this->setAttribute(Qt::WA_TranslucentBackground, true);

	qRegisterMetaType<CommandSharedPtr>("CommandSharedPtr");
}

UIGameBoardWrapper::~UIGameBoardWrapper()
{
	if (message_processor_)
	{
		message_processor_->RemoveCommandObserver(command_observer_.get());
	}

	command_handler_->SetGameBoardWrapper(nullptr);

	CHESS_MANAUL_HELPER->ClearGameRecords();
}

void UIGameBoardWrapper::Init()
{
	//1: create command handler
	command_handler_ = CommandHandler::Create();
	command_handler_->SetGameBoardWrapper(this);

	//2: create chess basket
	chess_basket_ = this->CreateChessBasket();
	D_ASSERT(chess_basket_, L"chess_basket_ should not be null");
	if (!chess_basket_->parent())
	{
		chess_basket_->setParent(this);
	}

	//3: create tip helper
	tip_helper_ = new TipHelper(this);

	//4: create game boards
	auto vec_game_board = this->CreateGameBoards();
	D_ASSERT(!vec_game_board.empty(), L"vec_game_board should not be empty");
	for (auto game_board : vec_game_board)
	{
		if (!game_board->parent())
		{
			game_board->setParent(this);
		}

		// create decorator board for each game board
		UIDecoratorBoard* decorator_board = nullptr;
		{
			PaintBaseInfo paint;
			paint.width = game_board->width();
			paint.height = game_board->height();
			paint.offset_x_ = 0;
			paint.offset_y_ = 0;
			paint.b_stu_ = ClientTypeIsStudent();
			paint.b_accept_event_ = false;

			decorator_board = new UIDecoratorBoard(game_board->BoardType(), game_board->size(), this);

			if (!UsageTypeIsPlayback())
			{
				decorator_board->SetCallbackForDrawBoard([=](const DrawOpInfo& draw_info){
					if (command_handler_)
					{
						command_handler_->SendDrawCommand(draw_info);
					}
				});

				decorator_board->SetCallbackForMarkBoard([=](const MarkOpInfo& mark_info){
					if (command_handler_)
					{
						command_handler_->SendMarkCommand(mark_info);
					}
				});
			}

			QPointer<UIGameBoardBase> weak_game_board(game_board);
			decorator_board->SetGridGlobalPositionDelegateForMarkBoard([=](int grid_x, int grid_y){
				QPoint global_pos(INT_MIN, INT_MIN);

				if (!weak_game_board.isNull())
				{
					global_pos = weak_game_board->mapToGlobal(weak_game_board->PositionByGridIndex(GridIndex(grid_x, grid_y)));
				}

				return global_pos;
			});
		}

		// create animation layer for each game board
		auto animation_layer = new UIAnimationLayer(this);
		animation_layer->setAttribute(Qt::WA_TransparentForMouseEvents, true);
		animation_layer->SetAnimationDelegate(game_board);

		game_board->AssociateAnimationLayer(animation_layer);
		game_board->AssociateChessBasket(chess_basket_);
		game_board->AssociateTipHelper(tip_helper_);

		game_board_tuples_->Push(game_board, decorator_board, animation_layer);

		game_board_tuples_->Back().Hide();
	}

	//5: calculate size
	const QSize size = this->CalculteSize();
	this->setFixedSize(size);

	this->SwitchMasterToGameBoard(game_board_tuples_->Front().BoardType());
}

QSize UIGameBoardWrapper::CalculteSize()
{
	QSize sz_game_board(0, 0);
	const QSize sz_chess_basket = chess_basket_->size();
	const int space = 0;

	game_board_tuples_->Foreach([&sz_game_board](const GameBoardTuple& truple){
		auto game_board = truple.GetGameBoard();
		if (game_board->width() > sz_game_board.width())
		{
			sz_game_board.setWidth(game_board->width());
		}
		
		if (game_board->height() > sz_game_board.height())
		{
			sz_game_board.setHeight(game_board->height());
		}
	});

	const QSize size(sz_game_board.width() + sz_chess_basket.width() + space, max(sz_game_board.height(), sz_chess_basket.height()));

	return QSize(sz_game_board.width() + sz_chess_basket.width() + space, max(sz_game_board.height(), sz_chess_basket.height()));
}

void  UIGameBoardWrapper::SetMessageProcessor(const MessageProcessorPtr& msg_processor)
{
	if (message_processor_ && command_observer_)
	{
		message_processor_->RemoveCommandObserver(command_observer_.get());
	}

	message_processor_ = msg_processor;

	if (!command_observer_)
	{
		command_observer_.reset(new MessageCommandObserver(this));
	}

	if (message_processor_)
	{
		message_processor_->AddCommandObserver(command_observer_.get());
	}
}

void  UIGameBoardWrapper::SetCommandSender(const CommandSender& sender)
{
	command_handler_->SetCommandSender(sender);

	game_board_tuples_->Foreach([this](const GameBoardTuple& truple){
		truple.GetGameBoard()->SetCommandHandler(command_handler_);
	});
}

void UIGameBoardWrapper::AcceptMasterCommandFromOutside(const CommandSharedPtr& cmd_ptr)
{
	if (command_handler_)
	{
		command_handler_->HandleMasterCommand(cmd_ptr);
	}
}

void  UIGameBoardWrapper::SetDrawBoardActive(bool active)
{
	game_board_tuples_->Foreach([=](GameBoardTuple& truple){
		truple.GetDecoratorBoard()->SetDrawBoardActive(active && IsGameBoardEnabled());
	});

	this->ZOrderUi();
}

void UIGameBoardWrapper::SetMarkBoardActive(bool active)
{
	game_board_tuples_->Foreach([=](GameBoardTuple& truple){
		truple.GetDecoratorBoard()->SetMarkBoardActive(active && IsGameBoardEnabled());
	});

	this->ZOrderUi();
}

void  UIGameBoardWrapper::SetGameBoardEnabled(bool game_board_enabled)
{
	if (state_set_.IsGameBoardEnabled() == game_board_enabled) { return; }

	state_set_.SetGameBoardEnabled(game_board_enabled);

	chess_basket_->SetBasketEnabled(IsGameBoardEnabled());

	this->ZOrderUi();
}

void UIGameBoardWrapper::CreateOperationalLayer(QWidget* presentation_layer)
{
	if (!operational_layer_)
	{
		if (!presentation_layer && !operational_presentation_layer_)
		{
			presentation_layer = operational_presentation_layer_ = new QWidget(this);
			operational_presentation_layer_->setFixedSize(size());
		}

		presentation_layer->setAttribute(Qt::WA_TransparentForMouseEvents);

		operational_layer_ = this->InnerCreateOperationalLayer(presentation_layer);
		if (!operational_layer_->parent())
		{
			operational_layer_->setParent(this);
		}

		operational_layer_->setFixedSize(size());

		game_board_tuples_->Foreach([=](GameBoardTuple& truple){
			truple.GetGameBoard()->AssociateOperationalLayer(operational_layer_);
		});

		this->ZOrderUi();

		ChessBasket()->AssociateOperationalLayer(operational_layer_);
	}
}

UIGameBoardBase* UIGameBoardWrapper::CurrentGameBoard() const
{
	D_ASSERT(game_board_tuple_active_, L"game_board_tuple_active_ should not be null");

	return game_board_tuple_active_->GetGameBoard();
}

DrawBoard* UIGameBoardWrapper::CurrentDrawBoard() const
{
	D_ASSERT(game_board_tuple_active_, L"game_board_tuple_active_ should not be null");

	return game_board_tuple_active_->GetDrawBoard();
}

BaseMarkForm* UIGameBoardWrapper::CurrentMarkBoard() const
{
	D_ASSERT(game_board_tuple_active_, L"game_board_tuple_active_ should not be null");

	return game_board_tuple_active_->GetMarkBoard();
}

UIDecoratorBoard* UIGameBoardWrapper::CurrentDecoratorBoard() const
{
	D_ASSERT(game_board_tuple_active_, L"game_board_tuple_active_ should not be null");

	return game_board_tuple_active_->GetDecoratorBoard();
}

UIAnimationLayer* UIGameBoardWrapper::CurrentAnimationLayer() const
{
	D_ASSERT(game_board_tuple_active_, L"game_board_tuple_active_ should not be null");

	return game_board_tuple_active_->GetAnimationLayer();
}

void UIGameBoardWrapper::SwitchMasterToGameBoard(GameBoardType target_board_type)
{
	this->TryToCacheCurrentGameOfMasterModel(); // important for multi-separated-grid game board like GO!

	this->SwitchToGameBoard(target_board_type);

	command_handler_->SwitchMasterToGameBoard(target_board_type);
	command_handler_->SetDrawBoardByTeacher(target_board_type);
	command_handler_->SetDrawBoardByStudent(target_board_type);
}

void UIGameBoardWrapper::TryToCacheCurrentGameOfMasterModel()
{
	game_board_tuples_->Foreach([=](GameBoardTuple& truple){
		truple.GetGameBoard()->TryToCacheCurrentGameOfMasterModel();
	});
}

void UIGameBoardWrapper::SwitchToGameBoard(GameBoardType target_board_type)
{
	if (!game_board_tuple_active_ || game_board_tuple_active_->BoardType() != target_board_type)
	{
		if (game_board_tuple_active_) { game_board_tuple_active_->Hide(); }

		game_board_tuple_active_ = game_board_tuples_->TrupleByType(target_board_type);
		D_ASSERT(game_board_tuple_active_, L"game_board_tuple_active_ should not be null");

		game_board_tuple_active_->Show();

		this->ZOrderUi();

		chess_basket_->SwitchToGameBoard(target_board_type);
	}
}

BaseMarkForm* UIGameBoardWrapper::MarkBoardOnGameBoard(GameBoardType board_type) const
{
	return game_board_tuples_->MarkBoardByType(board_type);
}

DrawBoard* UIGameBoardWrapper::DrawBoardOnGameBoard(GameBoardType board_type) const
{
	return game_board_tuples_->DrawBoardByType(board_type);
}

void UIGameBoardWrapper::DownloadChessManual(const std::string& file_url, const FileDownloadedCallback& callback)
{
	CHESS_MANAUL_HELPER->Download(file_url, callback);
}

void UIGameBoardWrapper::HandleMasterCommandSpecially(const CommandSharedPtr& cmd_ptr)
{
	emit MasterCommandToOutside(cmd_ptr);

	const auto cmd_type = cmd_ptr->Type();
	switch (cmd_type)
	{
	case CommandType::TeacherForbiddenStudentOper:
		if (ClientTypeIsStudent() || UsageTypeIsPlayback())
		{
			this->SetGameBoardEnabled(!cmd_ptr->IntAt(0));
		}
		break;

	case CommandType::TeacherNewGameBoard:
	case CommandType::TeacherNewGameBoardEx:
	case CommandType::TeacherSwitchToHistoryBoard:
	case CommandType::TeacherImportChessManual:
		CurrentDrawBoard()->ClearDrawedUnits();
		CurrentMarkBoard()->OnHideAllMarkIcons();
		this->SetDrawBoardActive(false);
		break;

	case CommandType::TeacherStartCourse:
		if (ClientTypeIsStudent() && UsageTypeIsOnline() && state_set_.IsNotInRecovery())
		{
			this->TipOnOnlineMsgBox(QStringByKeyWithArgs(LK_START_COURSE_MSG_FORMAT_STU, O2OCallInfo().tea_price_));
		}
		break;

	default:
		break;
	}

	// test create new board dialog
	if (cmd_type == CommandType::TeacherNewGameBoardEx && state_set_.IsNotInRecovery())
	{
		this->TriggerToCreateNewBoardByTeacher();
	}

	game_board_tuples_->Foreach([=](GameBoardTuple& truple){
		truple.GetGameBoard()->HandleMasterCommandSpecially(cmd_ptr);
	});

	chess_basket_->HandleMasterCommandSpecially(cmd_ptr);
}

void UIGameBoardWrapper::RefreshGameBoardWithMaster(GameBoardType board_type)
{
	auto game_board = game_board_tuples_->GameBoardByType(board_type);
	if (game_board)
	{
		game_board->RefreshBoardWithMaster();
	}
}

void UIGameBoardWrapper::OnLocalCommandConflictWithMaster()
{
	this->TipOnToast(QStringByKey(LK_COMMAND_CONFLICT));
}

void UIGameBoardWrapper::OnWaitingRecovery()
{
	state_set_.SetInRecovery(true);

	this->ShowShieldLayer();

	this->TipOnOnlineMsgBox(QStringByKey(LK_SYNC_CLASS_DATA), 1000 * 30);
}

void UIGameBoardWrapper::OnStartingRecovery()
{
	this->ResetForRecovery();

	game_board_tuples_->Foreach([=](GameBoardTuple& truple){
		truple.GetGameBoard()->SetForcingToPlayActionResultImmediately(true);
	});
}

void UIGameBoardWrapper::OnRecoveryDone()
{
	game_board_tuples_->Foreach([=](GameBoardTuple& truple){
		truple.GetGameBoard()->SetForcingToPlayActionResultImmediately(false);
	});

	this->CloseOnlineMsgBox();
	this->TipOnOnlineMsgBox(QStringByKey(LK_CLASS_DATA_SYNC_OK), 1000 * 2);

	this->HideShieldLayer();

	state_set_.SetInRecovery(false);
}

void UIGameBoardWrapper::ZOrderUi()
{
	D_ASSERT(game_board_tuple_active_, L"game_board_tuple_active_ should not be null");

	auto board_type_active = game_board_tuple_active_->BoardType();

	game_board_tuples_->Foreach([=](GameBoardTuple& truple){
		(truple.BoardType() == board_type_active) ? truple.Raise() : truple.Lower();
	});

	auto current_decorator_board = CurrentDecoratorBoard();

	bool decorator_over_operational_layer = current_decorator_board->IsActive() && IsGameBoardEnabled();

	if (decorator_over_operational_layer)
	{
		if (operational_layer_)
		{
			operational_layer_->raise();
		}

		current_decorator_board->raise();
	}
	else
	{
		current_decorator_board->raise();

		if (operational_layer_)
		{
			operational_layer_->raise();
		}
	}

	ChessBasket()->raise();

	CurrentAnimationLayer()->raise(); // important that animation layer is WA_TransparentForMouseEvents

	if (operational_presentation_layer_) // important that animation layer is WA_TransparentForMouseEvents
	{
		operational_presentation_layer_->raise();
	}
	
	if (shield_layer_)
	{
		shield_layer_->raise();
	}

	this->ZOrderChildrenAlwaysOnTop();
}

void UIGameBoardWrapper::ZOrderChildrenAlwaysOnTop()
{
	if (!children_always_on_top_.empty())
	{
		children_always_on_top_.erase(std::remove_if(children_always_on_top_.begin(), children_always_on_top_.end(), [](const QWidgetPtr& ptr){ return ptr.isNull(); }),
			children_always_on_top_.end());

		if (!children_always_on_top_.empty())
		{
			for (auto& child : children_always_on_top_)
			{
				child->raise();
			}
		}
	}
}

void UIGameBoardWrapper::KeepChildAlwaysOnTop(QWidget* child)
{
	D_ASSERT(child->parentWidget() == this, "parent of child should be me");

	auto it = std::find_if(children_always_on_top_.cbegin(), children_always_on_top_.cend(), [=](const QWidgetPtr& ptr){ return ptr.data() == child; });
	if (it != children_always_on_top_.cend())
	{
		children_always_on_top_.erase(it);
	}

	children_always_on_top_.emplace_back(child);

	child->raise();
}

void  UIGameBoardWrapper::resizeEvent(QResizeEvent* resize_event)
{
	const QSize size = resize_event->size();

	UiUtils::MoveWidgetLeftTopTo(chess_basket_, QPoint(0, 0));

	game_board_tuples_->Foreach([&](GameBoardTuple& truple){
		UiUtils::MoveWidgetRightTopTo(truple.GetGameBoard(), QPoint(size.width(), 0));

		truple.GetDecoratorBoard()->setGeometry(truple.GetGameBoard()->geometry());
		truple.GetAnimationLayer()->setGeometry(0, 0, size.width(), size.height());
	});

	if (operational_layer_)
	{
		operational_layer_->setFixedSize(size);
	}

	if (operational_presentation_layer_)
	{
		operational_presentation_layer_->setFixedSize(size);
	}

	if (shield_layer_)
	{
		shield_layer_->setFixedSize(size);
	}
}

void UIGameBoardWrapper::ResetForRecovery()
{
	this->SetGameBoardEnabled(false);

	game_board_tuples_->Foreach([=](GameBoardTuple& truple){
		truple.GetGameBoard()->Reset();
		truple.GetDecoratorBoard()->Reset();
	});

	if (game_board_tuple_active_) { game_board_tuple_active_->Hide(); }
	game_board_tuple_active_ = nullptr;
	this->SwitchMasterToGameBoard(game_board_tuples_->Front().BoardType());

	chess_basket_->Reset();

	if (operational_layer_) { operational_layer_->Interrupt(); }
}

void UIGameBoardWrapper::TipOnToast(const QString& tip, int milliseconds_to_stay)
{
	if (tip_helper_)
	{
		tip_helper_->TipOnToast(tip, milliseconds_to_stay);
	}
}

void UIGameBoardWrapper::TipOnOnlineMsgBox(const QString& tip, int milliseconds_to_stay)
{
	if (tip_helper_)
	{
		tip_helper_->TipOnOnlineMsgBox(tip, milliseconds_to_stay);
	}
}

void UIGameBoardWrapper::CloseOnlineMsgBox()
{
	if (tip_helper_)
	{
		tip_helper_->CloseOnlineMsgBox();
	}
}

void UIGameBoardWrapper::ShowShieldLayer()
{
	if (!shield_layer_)
	{
		shield_layer_ = new QWidget(this);

		shield_layer_->setFixedSize(size());

		this->ZOrderUi();
	}

	shield_layer_->show();
}

void UIGameBoardWrapper::HideShieldLayer()
{
	if (shield_layer_)
	{
		shield_layer_->hide();
	}
}

void UIGameBoardWrapper::TriggerToCreateNewBoardByTeacher()
{
	//if (ClientTypeIsTeacher())
	{
		auto dialog = new UICreateNewBoardDialog(UiUtils::GetAncestorWidget(this));

		dialog->show();

		//UiUtils::AlignWidgetOnCenter(dialog, CurrentGameBoard());
	}
}

GAME_NAMESPACE_END