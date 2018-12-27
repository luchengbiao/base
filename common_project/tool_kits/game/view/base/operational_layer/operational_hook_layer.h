#ifndef __GAME_VIEW_BASE_OPERATIONAL_LAYER_OPERATIONAL_HOOK_LAYER_H__
#define __GAME_VIEW_BASE_OPERATIONAL_LAYER_OPERATIONAL_HOOK_LAYER_H__
#include <QPointer>
#include <QWidget>
#include "game/macros.h"

GAME_NAMESPACE_BEGIN

class UIOperationalLayer;

class UIOperationalHookLayer : public QWidget
{
	typedef QPointer<UIOperationalLayer> OperationalLayerPtr;

public:
	explicit UIOperationalHookLayer(QWidget* parent = nullptr);

	void			AssociateOperationalLayer(UIOperationalLayer*);

protected:
	virtual void	mousePressEvent(QMouseEvent*) override;
	virtual void	mouseMoveEvent(QMouseEvent*) override;
	virtual void	mouseReleaseEvent(QMouseEvent*) override;

private:
	OperationalLayerPtr	operational_layer_{ nullptr };
};

GAME_NAMESPACE_END

#endif