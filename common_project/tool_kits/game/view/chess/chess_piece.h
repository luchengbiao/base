#ifndef __GAME_VIEW_CHESS_CHESS_PIECE_H__
#define __GAME_VIEW_CHESS_CHESS_PIECE_H__
#include "game/model/chess/chess_trait.h"
#include "game/view/base/chess_piece/chess_piece_grid.h"
#include "game/view/base/chess_piece/chess_piece_with_mixins.h"
#include "game/view/base/chess_piece/chess_piece_with_trait.h"

GAME_NAMESPACE_BEGIN

class UIChessPiece : public UIChessPieceWithTrait<ChessTrait, UIChessPieceWithMixins<UIChessPieceBase, ChessPieceGrid>>
{
	Q_OBJECT

public:
	explicit UIChessPiece(QWidget* parent = nullptr) : UIChessPieceWithTrait(parent) {}
};

GAME_NAMESPACE_END

#endif