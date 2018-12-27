#ifndef __GAME_VIEW_BASE_GAME_BOARD_WRAPPER_UI_H__
#define __GAME_VIEW_BASE_GAME_BOARD_WRAPPER_UI_H__
#include <functional>
#include <memory>
#include <vector>
#include <QPointer>
#include <QWidget>
#include "base/memory/objectcounter.h"
#include "game/model/command_handler/command_handler_fwd.h"
#include "game/model/command_handler/game_board_wrapper.h"
#include "game/public/enums/client_type.h"
#include "game/public/enums/usage_type.h"
#include "game/view/base/game_board_wrapper_state_set.h"
#include "protocol/command/base/command_fwd.h"

class DrawBoard;
class BaseMarkForm;

GAME_NAMESPACE_BEGIN

class GameBoardTuple;
class GameBoardTuples;
class MessageProcessor;
class UIDecoratorBoard;
class UIGameBoardBase;
class UIAnimationLayer;
class UIOperationalLayer;
class TipHelper;

typedef std::vector<UIGameBoardBase*> GameBoardVector;

class UIChessBasketBase;

class UIGameBoardWrapper : public QWidget, public GameBoardWrapper
{
	Q_OBJECT

	class MessageCommandObserver;
	typedef std::unique_ptr<MessageCommandObserver>			MessageCommandObserverPtr;
	typedef std::unique_ptr<GameBoardTuples>				GameBoardTuplesPtr;
	typedef std::shared_ptr<MessageProcessor>				MessageProcessorPtr;
	typedef std::function<void(const CommandSharedPtr&)>	CommandSender;

	typedef QPointer<QWidget> QWidgetPtr;
	typedef std::vector<QWidgetPtr> QWidgetPtrs;

	struct MyObjectCounter : public nbase::ObjectCounter<MyObjectCounter> {};
	
public:
	~UIGameBoardWrapper();

	void						SetMessageProcessor(const MessageProcessorPtr& msg_processor);

	void						SetCommandSender(const CommandSender& sender);

	void						AcceptMasterCommandFromOutside(const CommandSharedPtr& cmd_ptr); // mainly used in playback
signals: void					MasterCommandToOutside(CommandSharedPtr); // give the outside a choice to handle a MasterCommand

public:
	void						SetDrawBoardActive(bool active);
	void						SetMarkBoardActive(bool active);

	void						SetGameBoardEnabled(bool game_board_enabled);
	inline bool					IsGameBoardEnabled() const { return state_set_.IsGameBoardEnabled(); }

	void						CreateOperationalLayer(QWidget* presentation_layer = nullptr);

	UIGameBoardBase*			CurrentGameBoard() const;
	DrawBoard*					CurrentDrawBoard() const;
	BaseMarkForm*				CurrentMarkBoard() const;
	UIDecoratorBoard*			CurrentDecoratorBoard() const;
	UIAnimationLayer*			CurrentAnimationLayer() const;
	inline UIChessBasketBase*	ChessBasket() const { return chess_basket_; }

	void						TipOnToast(const QString& tip, int milliseconds_to_stay = 2500);
	void						TipOnOnlineMsgBox(const QString& tip, int milliseconds_to_stay = 5000);
	void						CloseOnlineMsgBox();

	void						KeepChildAlwaysOnTop(QWidget* child);

	// methods invoked by user from the outside to trigger some activities
	void						TriggerToCreateNewBoardByTeacher();

protected:
	explicit UIGameBoardWrapper(ClientType client_type, UsageType usage_type, QWidget* parent = nullptr);

	virtual void				Init();

	virtual GameBoardVector		CreateGameBoards() = 0;
	virtual UIChessBasketBase*	CreateChessBasket() = 0;
	virtual QSize				CalculteSize();
	virtual UIOperationalLayer*	InnerCreateOperationalLayer(QWidget* presentation_layer) = 0;

	void						TryToCacheCurrentGameOfMasterModel();
	void						SwitchToGameBoard(GameBoardType);
	virtual	void				ZOrderUi();
	void						ZOrderChildrenAlwaysOnTop();

	virtual void				resizeEvent(QResizeEvent* resize_event) override;

	virtual	void				ResetForRecovery();

	void						ShowShieldLayer();
	void						HideShieldLayer();

	// override GameBoardWrapper
	virtual void				SwitchMasterToGameBoard(GameBoardType) override final;
	virtual DrawBoard*			DrawBoardOnGameBoard(GameBoardType) const override;
	virtual BaseMarkForm*		MarkBoardOnGameBoard(GameBoardType) const override;
	virtual void				DownloadChessManual(const std::string& file_url, const FileDownloadedCallback& callback) override;
	virtual void				HandleMasterCommandSpecially(const CommandSharedPtr& cmd_ptr) override;
	virtual void				RefreshGameBoardWithMaster(GameBoardType) override;
	virtual void				OnLocalCommandConflictWithMaster() override;
	virtual void				OnWaitingRecovery() override;
	virtual void				OnStartingRecovery() override;
	virtual	void				OnRecoveryDone() override;

protected:
	GameBoardTuplesPtr			game_board_tuples_{ nullptr };
	GameBoardTuple*				game_board_tuple_active_{ nullptr };

	UIChessBasketBase*			chess_basket_{ nullptr };
	UIOperationalLayer*			operational_layer_{ nullptr };
	QWidget*					operational_presentation_layer_{ nullptr };
	QWidget*					shield_layer_{ nullptr };  // cover on top to prevent operation in such situation as recovery
	QWidgetPtrs					children_always_on_top_;

	TipHelper*					tip_helper_{ nullptr };

	CommandHandlerSharedPtr		command_handler_;
	MessageProcessorPtr			message_processor_;
	MessageCommandObserverPtr	command_observer_{ nullptr };

	StateSet					state_set_;

	MyObjectCounter				counter_; // counter for instance of UIGameBoardWrapper-based
};

typedef QPointer<UIGameBoardWrapper> UIGameBoardWrapperPtr;
extern UIGameBoardWrapperPtr CurrentGameBoardWrapper;

GAME_NAMESPACE_END

#endif