#ifndef __GAME_MODEL_BASE_PROTO_ENTITIES_VIEW_NEW_GAME_BOARD_EX_VIEW_H__
#define __GAME_MODEL_BASE_PROTO_ENTITIES_VIEW_NEW_GAME_BOARD_EX_VIEW_H__
#include "game/public/enums/client_type.h"
#include "game/public/enums/game_mode.h"
#include "game/public/enums/game_board_type.h"
#include "game/public/enums/player_side.h"
#include "protocol/command/entities/new_game_board_ex.h"
#include "public_view.h"

GAME_NAMESPACE_BEGIN

struct NewGameBoardExView
{
	typedef proto::NewGameBoardEx ViewSrc;

	NewGameBoardExView(const ViewSrc& src) : src_(src){}

	inline GameBoardType	BoardType() const { return static_cast<GameBoardType>(src_.board_type_); }

	inline GameMode			GetGameMode() const { return static_cast<GameMode>(src_.game_mode_); }

	inline bool				IsRuleBased() const { return GetGameMode() == GameMode::VS; }

	inline void				VSClient(ClientType& black_client, ClientType& white_client);

	inline PlayerSide		FirstHand() const;

	inline bool				FirstHandIsWhiteSide() const { return FirstHand() == PlayerSide::White; }

	inline bool				IsCreatingInitialBoard() const { return src_.layout_ == 0; }

	inline bool				IsCreatingWithCurrentBoard() const { return src_.layout_ == 1; }

	inline bool				IsCreatingWithHistoryBoard() const { return src_.layout_ == 2; }

	inline int				IndexOfHistoryBoard() const { return src_.layout_index_; }

private:
	const ViewSrc&			src_;
};

inline void NewGameBoardExView::VSClient(ClientType& black_client, ClientType& white_client)
{
	PublicView::VSClientByRoleValue(src_.role_, black_client, white_client);
}

inline PlayerSide NewGameBoardExView::FirstHand() const
{
	PlayerSide side{ PlayerSide::None };

	switch (src_.order_)
	{
	case 0:
		side = PlayerSide::Black;
		break;

	case 1:
		side = PlayerSide::White;
		break;

	default:
		break;
	}

	return side;
}


GAME_NAMESPACE_END

#endif