#ifndef __GAME_VIEW_DECORATOR_DECORATOR_BOARD_H__
#define __GAME_VIEW_DECORATOR_DECORATOR_BOARD_H__
#include <functional>
#include <QPointer>
#include <QWidget>
#include "game/macros.h"
#include "game/public/enums/game_board_type.h"

struct DrawOpInfo;
class DrawBoard;

struct MarkOpInfo;
class BaseMarkForm;

GAME_NAMESPACE_BEGIN

class UIDecoratorBoard : public QWidget
{
	Q_OBJECT

public:
	explicit UIDecoratorBoard(const GameBoardType board_type, const QSize& fixed_size, QWidget* parent = nullptr);

	inline DrawBoard*		GetDrawBoard() const { return draw_board_; }
	inline BaseMarkForm*	GetMarkBoard() const { return mark_board_; }

	void					SetCallbackForDrawBoard(const std::function<void(const DrawOpInfo&)>& draw_callback);
	void					SetCallbackForMarkBoard(const std::function<void(const MarkOpInfo&)>& mark_callback);
	void					SetGridGlobalPositionDelegateForMarkBoard(const std::function<QPoint(int,int)>& mark_delegate);

	bool					IsActive() const;
	void					SetDrawBoardActive(bool active);
	void					SetMarkBoardActive(bool active);

	void					Reset();

protected:
	void					Init(const GameBoardType board_type, const QSize& fixed_size);

	virtual void			resizeEvent(QResizeEvent* resize_event) override;
	virtual void			showEvent(QShowEvent*) override;
	virtual void			hideEvent(QHideEvent*) override;

private:
	DrawBoard*				draw_board_{ nullptr };
	BaseMarkForm*			mark_board_{ nullptr };
};

GAME_NAMESPACE_END

#endif