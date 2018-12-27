#ifndef __GAME_MODEL_BASE_RECORD_CUSTOM_RECORD_H__
#define __GAME_MODEL_BASE_RECORD_CUSTOM_RECORD_H__
#include <memory>
#include "game/macros.h"
#include "game/public/enums/game_mode.h"
#include "game/public/enums/player_side.h"
#include "game/public/structs/vs_player_info/vs_player_info.h"

GAME_NAMESPACE_BEGIN

class CustomRecord;
typedef std::unique_ptr<CustomRecord> CustomRecordUniquePtr;

class CustomRecord
{
public:
	CustomRecord(bool rule_based = false) : rule_based_(rule_based) {}

	virtual ~CustomRecord() = default;

	virtual CustomRecordUniquePtr	Clone() const = 0;

	virtual void					CopyFrom(const CustomRecord&) = 0;

	virtual	void					Reset() = 0;

	inline bool						IsRuleBased() const { return rule_based_; }
	virtual void					SetRuleBased(bool rule_based) = 0;

	inline bool						IsVSMode() const { return rule_based_; }
	void							GetCurrentVSInfo(GameMode& game_mode, VSPlayerInfo* black_player = nullptr, VSPlayerInfo* white_player = nullptr, PlayerSide* current_side = nullptr);
	virtual PlayerSide				GetCurrentSide() = 0;

	void							SetCurrentVSPlayerInfo(const VSPlayerInfo& black_player, const VSPlayerInfo& white_player);

protected:
	bool							rule_based_{ false };

	VSPlayerInfo					black_player_;
	VSPlayerInfo					white_player_;
};

GAME_NAMESPACE_END

#endif