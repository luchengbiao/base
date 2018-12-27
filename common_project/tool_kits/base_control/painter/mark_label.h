#pragma once
#include <QLabel>
#include "paint_data.h"

class MarkLabel :public QLabel
{
public:
	explicit MarkLabel(QWidget *parent = 0);
	~MarkLabel();
	void OnSetMarkIcon(MarkIcon icon_id);
	MarkIcon OnGetMarkIcon();

private:
	MarkIcon icon_id_;
};