#ifndef __GAME_MODEL_BASE_PROTO_ENTITIES_VIEW_IMPORT_CHESS_MANUAL_VIEW_H__
#define __GAME_MODEL_BASE_PROTO_ENTITIES_VIEW_IMPORT_CHESS_MANUAL_VIEW_H__
#include "game/public/enums/client_type.h"
#include "protocol/command/entities/import_chess_manual.h"
#include "public_view.h"

GAME_NAMESPACE_BEGIN

struct ImportChessManualView
{
	typedef proto::ImportChessManual ViewSrc;

	ImportChessManualView(const ViewSrc& src) : src_(src){}

	inline void				VSClient(ClientType& black_client, ClientType& white_client);

private:
	const ViewSrc&			src_;
};

inline void ImportChessManualView::VSClient(ClientType& black_client, ClientType& white_client)
{
	PublicView::VSClientByRoleValue(src_.role_, black_client, white_client);
}

GAME_NAMESPACE_END

#endif