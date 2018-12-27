#pragma once
#include "base_mark_widget.h"

class GoMarkForm : public BaseMarkForm
{
public:
	explicit GoMarkForm(QSize size, QWidget *parent = 0);
	~GoMarkForm();

private:
	virtual void OnInitValidPoint() override;
	virtual void OnGetRealPoint(QPoint pt, QPoint& des_pt) override;
	virtual bool OnGetVirtualPoint(QPoint pt, QPoint& des_pt) override;
	virtual MarkLabel* OnCreateIconLabel(MarkIcon icon, QPoint des_pt) override;
	virtual std::string OnGetIconQss(MarkIcon icon) override;
	virtual void OnUpdateIconLabelPos(MarkLabel* mark_icon, QPoint des_pt) override;
	void ClearPosButtons();
	void InitPosButtons();

private:
	int item_distance_;
	QPoint* go_coordinate[20][20];   //Î§ÆåÆå×Ó×ø±ê
};