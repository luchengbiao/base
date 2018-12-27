#ifndef __GAME_VIEW_BASE_CHESS_PIECE_CHESS_PIECE_WITH_TRAIT_H__
#define __GAME_VIEW_BASE_CHESS_PIECE_CHESS_PIECE_WITH_TRAIT_H__
#include <type_traits>
#include "game/view/base/chess_piece/chess_piece_base.h"

GAME_NAMESPACE_BEGIN

template<typename Triat, typename Base = UIChessPieceBase, typename = std::enable_if_t<std::is_base_of<UIChessPieceBase, Base>::value>>
class UIChessPieceWithTrait : public Base
{
public:
	explicit UIChessPieceWithTrait(QWidget* parent = nullptr) : Base(parent) {}

	typename Triat::Color	Color() const { return Triat::ColorById(GetChessId()); }
	typename Triat::Type	Type() const { return Triat::TypeById(GetChessId()); }

	virtual std::string		ImagePath() const override { return Triat::NormalImagePathById(GetChessId()); }
};

GAME_NAMESPACE_END

#endif