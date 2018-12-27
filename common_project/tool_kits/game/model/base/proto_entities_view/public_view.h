#ifndef __GAME_MODEL_BASE_PROTO_ENTITIES_VIEW_PUBLIC_VIEW_H__
#define __GAME_MODEL_BASE_PROTO_ENTITIES_VIEW_PUBLIC_VIEW_H__
#include "game/public/enums/client_type.h"

GAME_NAMESPACE_BEGIN

struct PublicView
{
	static void	 VSClientByRoleValue(int role, ClientType& black_client, ClientType& white_client)
	{
		switch (role)
		{
		case 0:
			black_client = ClientType::Teacher;
			white_client = ClientType::Student;
			break;

		case 1:
			black_client = ClientType::Student;
			white_client = ClientType::Teacher;
			break;

		case 2:
			black_client = ClientType::Teacher;
			white_client = ClientType::Teacher;
			break;

		case 3:
			black_client = ClientType::Student;
			white_client = ClientType::Student;
			break;

		default:
			break;
		}
	}
};

GAME_NAMESPACE_END

#endif