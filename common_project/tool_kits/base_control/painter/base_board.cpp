#include "base_board.h"

BaseBoard::BaseBoard(PaintBaseInfo info, QWidget *parent /*= 0*/) : PaintImage(info, parent)
{
}

BaseBoard::~BaseBoard()
{

}

void BaseBoard::SetVisibleEx(bool bvisible)
{
	__super::SetVisibleEx(bvisible);
}