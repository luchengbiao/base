#include "custom_record.h"

GAME_NAMESPACE_BEGIN

void CustomRecord::CopyFrom(const CustomRecord& rhs)
{
	if (&rhs != this)
	{
		rule_based_ = rhs.rule_based_;
		black_player_ = rhs.black_player_;
		white_player_ = rhs.white_player_;
	}
}

void CustomRecord::Reset()
{
	rule_based_ = false;

	black_player_.Reset();
	white_player_.Reset();
}

void CustomRecord::GetCurrentVSInfo(GameMode& game_mode, VSPlayerInfo* black_player, VSPlayerInfo* white_player, PlayerSide* current_side)
{
	game_mode = IsRuleBased() ? GameMode::VS : GameMode::Teaching;
	if (GameMode::VS == game_mode)
	{
		if (black_player) { *black_player = black_player_; }
		if (white_player) { *white_player = white_player_; }
		if (current_side) { *current_side = GetCurrentSide(); }
	}
}

void CustomRecord::SetCurrentVSPlayerInfo(const VSPlayerInfo& black_player, const VSPlayerInfo& white_player)
{
	black_player_ = black_player;
	white_player_ = white_player;
}

GAME_NAMESPACE_END