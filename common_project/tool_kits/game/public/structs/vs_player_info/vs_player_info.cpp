#include "vs_player_info.h"

GAME_NAMESPACE_BEGIN

VSPlayerInfo::VSPlayerInfo(ClientType client, const std::string& name, const std::string& portrait_url)
: client_(client), name_(name), portrait_url_(portrait_url)
{}

VSPlayerInfo::VSPlayerInfo(VSPlayerInfo&& rhs)
: client_(rhs.client_), name_(std::move(rhs.name_)), portrait_url_(std::move(rhs.portrait_url_))
{}

void VSPlayerInfo::Reset()
{
	client_ = ClientType::None;
	name_.clear();
	portrait_url_.clear();
}

GAME_NAMESPACE_END