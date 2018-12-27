#include "mark_label.h"

MarkLabel::MarkLabel(QWidget *parent) :QLabel(parent)
{

}

MarkLabel::~MarkLabel()
{

}

void MarkLabel::OnSetMarkIcon(MarkIcon icon_id)
{
	icon_id_ = icon_id;
}

MarkIcon MarkLabel::OnGetMarkIcon()
{
	return icon_id_;
}