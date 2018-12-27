#pragma once
#include "base_mark_widget.h"

class ChessMarkForm : public BaseMarkForm
{
public:
	explicit ChessMarkForm(QSize size, QWidget *parent = 0);
	~ChessMarkForm();

private:
	virtual void OnInitValidPoint() override;
	virtual void OnGetRealPoint(QPoint pt, QPoint& des_pt) override;
	virtual bool OnGetVirtualPoint(QPoint pt, QPoint& des_pt) override;
	virtual MarkLabel* OnCreateIconLabel(MarkIcon icon, QPoint des_pt) override;
	virtual void OnUpdateIconLabelPos(MarkLabel* mark_icon, QPoint des_pt) override;
	virtual std::string OnGetIconQss(MarkIcon icon) override;
};