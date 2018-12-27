#pragma once
#include "qwidget.h"
#include "paint_data.h"
#include <QMouseEvent>
#include "mark_label.h"

typedef std::function<void(MarkOpInfo info)> MarkOpCallback;
typedef std::function<QPoint(int,int)> GridGlobalPositionDelegate;

class BaseMarkForm : public QWidget
{
public:
	explicit BaseMarkForm(QSize size,QWidget *parent = 0);
	~BaseMarkForm();

public:
	bool GetAcceptEvent(){ return b_accept_event_; }
	void SetAcceptEvent(bool bAccept);
	void OnSetSign(MarkIcon icon);
	MarkIcon OnGetSign(){ return mark_icon_; }
	void OnSendCommandCallBack(MarkOpCallback cb);
	void OnSetParamCalValidPoint(QPoint origin_pt, int width, int gridCount=0, int inside = 0);//为了计算有效点需要传递的参数
	void OnHideAllMarkIcons();
	virtual void OnUpdateMarkIcon(int mark_icon, QPoint pt);
	void OnMouseReleaseFromParent(QPoint pt);

	inline void SetGridGlobalPositionDelegate(const GridGlobalPositionDelegate& delegate) { grid_global_position_delegate_ = delegate; }

private:
	virtual void OnInitValidPoint();
	virtual void OnMarkSign(QPoint pt);
	virtual void OnGetRealPoint(QPoint pt,QPoint& des_pt);
	virtual bool OnGetVirtualPoint(QPoint pt, QPoint& des_pt);
	virtual MarkLabel* OnCreateIconLabel(MarkIcon icon, QPoint des_pt);
	virtual std::string OnGetIconQss(MarkIcon icon);
	virtual void OnUpdateIconLabelPos(MarkLabel* mark_icon,QPoint des_pt);

protected:
	bool eventFilter(QObject *watched, QEvent *event);//事件过滤器  
	MarkLabel* OnFindMarkIconByPos(QPoint pt);

protected:
	QPoint origin_pt_;
	int width_;
	int grid_count_;
	int inside_;
	MarkIcon mark_icon_;
	MarkOpCallback mark_send_cb_;  //发送命令信息回调	
	std::map<int, MarkLabel*> mark_icon_map_;		//保存所有标记的Label

private:
	bool b_accept_event_;

	GridGlobalPositionDelegate	grid_global_position_delegate_;
};