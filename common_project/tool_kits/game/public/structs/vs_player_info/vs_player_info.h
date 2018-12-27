#ifndef __GAME_PUBLIC_STRUCTS_VS_PLAYER_INFO_VS_PLAYER_INFO_H__
#define __GAME_PUBLIC_STRUCTS_VS_PLAYER_INFO_VS_PLAYER_INFO_H__
#include "game/public/enums/client_type.h"
#include <string>

GAME_NAMESPACE_BEGIN

struct VSPlayerInfo
{
public:
	VSPlayerInfo() = default;

	VSPlayerInfo(ClientType client, const std::string& name, const std::string& portrait_url);

	VSPlayerInfo(VSPlayerInfo&& rhs);

	inline ClientType			Client() const { return client_; }
	inline const std::string&	Name() const { return name_; }
	inline const std::string&	PortraitUrl() const { return portrait_url_; }

	void						Reset();

public:
	ClientType					client_{ ClientType::None };
	std::string					name_;
	std::string					portrait_url_;
};

inline bool operator==(const VSPlayerInfo& lhs, const VSPlayerInfo& rhs)
{
	return (lhs.client_ == rhs.client_) && (lhs.name_ == rhs.name_) && (lhs.portrait_url_ == rhs.portrait_url_);
}

GAME_NAMESPACE_END

#endif