#include "tip_helper.h"
#include <QEvent>
#include <QResizeEvent>
#include "base_control/messagebox/msgbox.h"
#include "base_control/toast/stoast.h"
#include "common/uiutils/ui_utils.h"
#include "game/public/literal_stock/literal_stock.h"
#include "game/public/public_setting/public_setting.h"
#include "game/view/base/game_board_base_ui.h"
#include "game/view/base/game_board_wrapper_ui.h"
#include "game/view/msg_box/online_msg_box.h"
#include "usound/ring/ring.h"

GAME_NAMESPACE_BEGIN

TipHelper::TipHelper(UIGameBoardWrapper* holder)
: QObject(holder)
, holder_(holder)
{}

TipHelper::~TipHelper()
{
	CloseMsgBox();

	if (shield_layer_)
	{
		shield_layer_->deleteLater();
	}
}

void TipHelper::TipOnToast(const QString& tip, int milliseconds_to_stay)
{
	if (!toast_)
	{
		toast_ = new SToast();
		toast_->setDurationOfFade(500);
		toast_->setWordCountUpperToWrap(20);
	}

	if (toast_)
	{
		toast_->setMessageVDuration(tip, milliseconds_to_stay, WidgetToAlignCenter());
	}
}

void TipHelper::StopToast()
{
	if (toast_)
	{
		toast_->stop();
	}
}

void TipHelper::TipOnMsgBox(const QString& tip)
{
	if (!msg_box_)
	{
		this->LazyCreateShieldLayer();

		QPointer<QWidget> widget_shield(shield_layer_);

		if (widget_shield)
		{
			widget_shield->setFixedSize(widget_shield->parentWidget()->size());
			widget_shield->raise();
			widget_shield->show();
		}

		msg_box_ = new MsgBox();

		if (msg_box_)
		{
			msg_box_->setWindowFlags(msg_box_->windowFlags() | Qt::WindowStaysOnTopHint);

			msg_box_->SetCallback([=](MsgBoxRet){
				if (widget_shield) { widget_shield->hide(); }
			});
		}
	}

	msg_box_->SetInfo(QString(), tip, QStringByKey(LK_CONFIRM), QString(), false);
	msg_box_->show();
	msg_box_->raise();

	UiUtils::AlignWidgetOnCenter(msg_box_, WidgetToAlignCenter());
}

void TipHelper::CloseMsgBox()
{
	if (msg_box_)
	{
		msg_box_->close();
	}
}

void TipHelper::TipOnOnlineMsgBox(const QString& tip, int milliseconds_to_stay)
{
	if (!holder_.isNull())
	{
		auto msg_box = new UIOnlineMsgBox(holder_);

		msg_box->SetTitle(QStringByKey(LK_HINT));
		msg_box->SetMessage(tip);
		if (milliseconds_to_stay > 0)
		{
			msg_box->DelayToClose(milliseconds_to_stay);
		}

		msg_box->show();
		msg_box->raise();

		holder_->KeepChildAlwaysOnTop(msg_box);

		UiUtils::AlignWidgetOnCenter(msg_box, WidgetToAlignCenter());

		online_msg_boxes_.emplace_back(msg_box);
	}
}

void TipHelper::CloseOnlineMsgBox()
{
	for (const auto& msg_box : online_msg_boxes_)
	{
		if (!msg_box.isNull())
		{
			msg_box->close();
		}
	}

	online_msg_boxes_.clear();
}

void TipHelper::LazyCreateShieldLayer()
{
	if (!shield_layer_ && holder_)
	{
		shield_layer_ = new QWidget(UiUtils::GetAncestorWidget(holder_));
		shield_layer_->hide();
	}
}

// conflict with PlaySound which is a macro of win32
#ifdef PlaySound 
#undef PlaySound
#endif

void TipHelper::PlaySoundWhenChessMovedIntoBoard()
{
	this->PlaySound(GameTypeIsGo() ? RING_GO_MOVE : RING_CHESS_NEW_DOWN);
}

void TipHelper::PlaySoundWhenChessMovedInsideOfBoard()
{
	this->PlaySound(GameTypeIsGo() ? RING_GO_MOVE : RING_CHESS_MOVE);
}

void TipHelper::PlaySoundWhenChessMovedOutOfBoard()
{
	this->PlaySound(GameTypeIsGo() ? RING_GO_MOVE : RING_CHESS_EAT);
}

void TipHelper::PlaySoundWhenChessCaptured()
{
	this->PlaySound(GameTypeIsGo() ? RING_GO_DEAD_STONE : RING_CHESS_EAT);
}

void TipHelper::PlaySound(int ring_type)
{
	if (!ring_ && holder_)
	{
		ring_.reset(new nim_comp::Ring);
		ring_->Init((HWND)UiUtils::GetAncestorWidget(holder_)->winId()); // be careful of native window, maybe cause some side-effect
	}

	if (ring_)
	{
		ring_->Play(static_cast<RingType>(ring_type));
	}
}

const QWidget* TipHelper::WidgetToAlignCenter()
{
	return holder_.isNull() ?  nullptr : (holder_->CurrentGameBoard() ? static_cast<QWidget*>(holder_->CurrentGameBoard()) : holder_);
}

GAME_NAMESPACE_END