#ifndef __GAME_VIEW_TIP_HELPER_TIP_HELPER_H__
#define __GAME_VIEW_TIP_HELPER_TIP_HELPER_H__
#include <memory>
#include <vector>
#include <QPointer>
#include <QObject>
#include "game/macros.h"

class SToast;
class MsgBox;

namespace nim_comp { class Ring; }

GAME_NAMESPACE_BEGIN

class UIGameBoardWrapper;
class UIOnlineMsgBox;

class TipHelper : public QObject
{
	Q_OBJECT

	typedef QPointer<SToast> SToastPtr;
	typedef QPointer<MsgBox> MsgBoxPtr;
	typedef QPointer<QWidget> QWidgetPtr;
	typedef QPointer<QWidget> QWidgetPtr;
	typedef QPointer<UIOnlineMsgBox> OnlineMsgBoxPtr;
	typedef QPointer<UIGameBoardWrapper> HolderPtr;
	typedef std::unique_ptr<nim_comp::Ring> RingPtr;
	typedef std::vector<OnlineMsgBoxPtr> OnlineMsgBoxes;

public:
	explicit TipHelper(UIGameBoardWrapper* holder);
	~TipHelper();

	// tip by text
	void			TipOnToast(const QString& tip, int milliseconds_to_stay = 2500);
	void			StopToast();

	void			TipOnMsgBox(const QString& tip);
	void			CloseMsgBox();

	void			TipOnOnlineMsgBox(const QString& tip, int milliseconds_to_stay = 5000);
	void			CloseOnlineMsgBox();

	// tip by sound
	void			PlaySoundWhenChessMovedIntoBoard();
	void			PlaySoundWhenChessMovedInsideOfBoard();
	void			PlaySoundWhenChessMovedOutOfBoard();
	void			PlaySoundWhenChessCaptured();
	void			PlaySound(int ring_type); // RingType

private:
	void			LazyCreateShieldLayer();

	const QWidget*	WidgetToAlignCenter();

private:
	HolderPtr		holder_;
	SToastPtr		toast_;
	MsgBoxPtr		msg_box_;
	OnlineMsgBoxes	online_msg_boxes_;
	QWidgetPtr		shield_layer_;
	RingPtr			ring_;
};

GAME_NAMESPACE_END

#endif