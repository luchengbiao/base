#ifndef __GAME_VIEW_BASE_CHESS_PIECE_CHESS_PIECE_WITH_MIXINS_H__
#define __GAME_VIEW_BASE_CHESS_PIECE_CHESS_PIECE_WITH_MIXINS_H__
#include <type_traits>
#include "game/view/base/chess_piece/chess_piece_base.h"

GAME_NAMESPACE_BEGIN

template<typename Base = UIChessPieceBase, typename... Mixins>
class UIChessPieceWithMixins : public Base, public Mixins...
{
public:
	explicit UIChessPieceWithMixins(QWidget* parent = nullptr) : Base(parent) 
	{
		static_assert(std::is_base_of<UIChessPieceBase, Base>::value, "UIChessPieceBase should be base of Base");
	}
};

GAME_NAMESPACE_END

#endif