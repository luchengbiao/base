#ifndef __GAME_VIEW_BASE_GAME_BOARD_BASE_UI_H__
#define __GAME_VIEW_BASE_GAME_BOARD_BASE_UI_H__
#include <functional>
#include <memory>
#include <deque>
#include <QPointer>
#include <QWidget>
#include "game/model/base/action/game_action_result.h"
#include "game/model/base/board/game_board_base_fwd.h"
#include "game/model/base/record/game_record.h"
#include "game/model/command_handler/command_handler_fwd.h"
#include "game/public/enums/game_board_type.h"
#include "game/public/enums/game_mode.h"
#include "game/public/enums/player_side.h"
#include "game/public/structs/vs_player_info/vs_player_info.h"
#include "game/view/animation/animation_delegate.h"
#include "game/view/base/game_board_layout_type.h"
#include "protocol/command/base/command_fwd.h"

GAME_NAMESPACE_BEGIN

class ChessInfoMatrix;
class GameActionResult;
class UIChessPieceBase;
class UIChessBasketBase;
class UIAnimationLayer;
class UIOperationalLayer;
class TipHelper;

class UIGameBoardBase : public QWidget, public UIAnimationDelegate
{
	Q_OBJECT

	typedef QPointer<UIAnimationLayer>		UIAnimationLayerPtr;
	typedef QPointer<UIChessBasketBase>		UIChessBasketPtr;
	typedef QPointer<UIOperationalLayer>	UIOperationalLayerPtr;
	typedef QPointer<TipHelper>				TipHelperPtr;
	
public:
	explicit UIGameBoardBase(QWidget* parent = nullptr);
	~UIGameBoardBase();

	UIGameBoardBase(const UIGameBoardBase&) = delete;
	UIGameBoardBase& operator=(const UIGameBoardBase&) = delete;

	virtual GameBoardType		BoardType() const = 0;
	virtual	int					RowsOfBoard() const = 0;
	virtual	int					ColumnsOfBoard() const  = 0;
	virtual bool				IsRuleBased() const = 0;
	inline	bool				IsVSMode() const { return IsRuleBased(); }
	virtual int					GridWidth() const = 0;
	virtual QPoint				PositionByGridIndex(const GridIndex& grid_index) const = 0;
	virtual UIChessPieceBase*	ChessPieceAt(const GridIndex& grid) const = 0;
	virtual bool				HasAnimationPlaying() const;
	virtual void				SetScaleLineVisible(bool visible) = 0;
	virtual void				HandleMasterCommandSpecially(const CommandSharedPtr& cmd_ptr);
	virtual void				Reset();
	virtual void				RefreshBoardWithMaster() = 0;

	void						SetCommandHandler(const CommandHandlerSharedPtr& command_handler);

	void						AssociateAnimationLayer(UIAnimationLayer* animation_layer);
	void						AssociateChessBasket(UIChessBasketBase* chess_basket);
	void						AssociateOperationalLayer(UIOperationalLayer* operational_layer);
	void						AssociateTipHelper(TipHelper* tip_helper);

	void						TryToCacheCurrentGameOfMasterModel();

	bool						TryToHandleCommandOnLocalModel(const CommandSharedPtr& cmd_ptr, std::string* failed_info = nullptr); // command should be generated locally and will be sent to server after "DoCommand"

	void						TryToRefreshLocal();
	virtual bool				TryToMoveChessLocally(ChessId chess_id, const GridIndex& grid_from, const GridIndex& grid_to, std::string* failed_info = nullptr);

	ChessId						LocalChessIdAt(const GridIndex& grid_from) const;
	void						LocalVSInfoOfCurrentGame(GameMode& game_mode,
														 VSPlayerInfo* black_player = nullptr,
														 VSPlayerInfo* white_player = nullptr,
														 PlayerSide* current_side = nullptr);

	void						HideChessAt(const GridIndex& grid_index);
	void						ShowChessAt(const GridIndex& grid_index);
		
	void						SetLayout(const GameBoardLayout layout);
	inline GameBoardLayout		GetLayout() const { return layout_; }

	inline bool					IsForcingToPlayActionResultImmediately() const { return force_to_play_action_result_immediately_; }
	inline void					SetForcingToPlayActionResultImmediately(bool forcing) { force_to_play_action_result_immediately_ = forcing; }

	void						TipOnToast(const QString& tip);
	void						TipOnMsgBox(const QString& tip);
	void						CloseTipMsgBox();

protected:
	virtual void				OnOperationalLayerAssociated() = 0;
	virtual void				OnLayoutChanged() = 0;

	void						SetMasterModel(const GameBoardPtr& model);
	void						SetLocalModel(const GameBoardPtr& model);

	virtual void				SyncLocalWithMaster();

	virtual void				OnMasterDone(const GameActionResult&) = 0;
	virtual void				OnMasterUndone(const GameActionResult&);
	virtual void				OnMasterRedone(const GameActionResult&);

	virtual void				OnLocalDone(const GameActionResult&) = 0;
	virtual void				OnLocalUndone(const GameActionResult&);
	virtual void				OnLocalRedone(const GameActionResult&);

	virtual bool				NeedToPlayActionResultImmediately(const GameActionResult&);

	virtual void				UpdateBasketWithModel(const GameBoardPtr& model);

	inline void					SetLocalDirty(bool dirty) { local_is_dirty_ = dirty; }
	inline bool					IsLocalDirty() const { return local_is_dirty_; }

	void						SendCommand(const CommandSharedPtr& cmd_ptr);

	void						ClearOperationalLayer();

	void						PlaySoundByActionResult(const GameActionResult&);
	void						PlaySoundWhenChessMovedIntoBoard();
	void						PlaySoundWhenChessMovedInsideOfBoard();
	void						PlaySoundWhenChessMovedOutOfBoard();
	void						PlaySoundWhenChessCaptured();

protected:
	GameBoardPtr				master_model_;
	GameBoardPtr				local_model_;
	bool						local_is_dirty_{ false }; // flag to lazy sync local with master when operating on board

	CommandHandlerSharedPtr		command_handler_;

	// AnimationLayer & ChessBasket & OperationalLayer is created and passed from the outside,
	// so use QPointer to guard
	UIAnimationLayerPtr			animation_layer_;
	UIChessBasketPtr			chess_basket_;
	UIOperationalLayerPtr		operational_layer_;
	TipHelperPtr				tip_helper_;

private:
	friend class MasterModelObserver;
	typedef std::unique_ptr<MasterModelObserver> MasterModelObserverPtr;
	MasterModelObserverPtr		master_model_observer_{ nullptr };

	friend class LocalModelObserver;
	typedef std::unique_ptr<LocalModelObserver> LocalModelObserverPtr;
	LocalModelObserverPtr		local_model_observer_{ nullptr };

	GameBoardLayout				layout_{ GameBoardLayout::Normal };

	bool						force_to_play_action_result_immediately_{ false };
};

GAME_NAMESPACE_END

#endif