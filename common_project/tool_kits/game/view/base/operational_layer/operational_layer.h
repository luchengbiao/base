#ifndef __GAME_VIEW_BASE_OPERATIONAL_LAYER_OPERATIONAL_LAYER_H__
#define __GAME_VIEW_BASE_OPERATIONAL_LAYER_OPERATIONAL_LAYER_H__
#include <QWidget>
#include "game/macros.h"

GAME_NAMESPACE_BEGIN

class UIOperationalLayer : public QWidget
{
public:
	explicit UIOperationalLayer(QWidget* parent = nullptr);

	virtual void		Interrupt() = 0;
	virtual void		Clear() = 0;
	virtual QPoint		CurrentGlobalPosOfChessFaceCenter() const = 0;

	void				HandleMousePressEvent(const QPoint& global_pos);
	void				HandleMouseMoveEvent(const QPoint& global_pos);
	void				HandleMouseReleaseEvent(const QPoint& global_pos);

protected:
	virtual void		mousePressEvent(QMouseEvent*) override final;
	virtual void		mouseMoveEvent(QMouseEvent*) override final;
	virtual void		mouseReleaseEvent(QMouseEvent*) override final;

	virtual void		HandleMousePressEvent(const QPoint& local_pos, const QPoint& global_pos) = 0;
	virtual void		HandleMouseMoveEvent(const QPoint& local_pos, const QPoint& global_pos) = 0;
	virtual void		HandleMouseReleaseEvent(const QPoint& local_pos, const QPoint& global_pos) = 0;
};

GAME_NAMESPACE_END

#endif