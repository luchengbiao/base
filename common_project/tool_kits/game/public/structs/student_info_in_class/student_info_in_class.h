#ifndef __GAME_PUBLIC_STRUCTS_STUDENT_INFO_IN_CLASS_STUDENT_INFO_IN_CLASS_H__
#define __GAME_PUBLIC_STRUCTS_STUDENT_INFO_IN_CLASS_STUDENT_INFO_IN_CLASS_H__
#include "game/public/enums/player_side.h"
#include "game/public/structs/vs_player_info/vs_player_info.h"

GAME_NAMESPACE_BEGIN

// in terms of VSPlayerInfo
struct StuInfoInClass : public VSPlayerInfo
{
public:
	StuInfoInClass();

	inline const std::string&		AccountId() const { return account_id_; }
	inline bool						IsAdded() const { return added_; }
	inline bool						IsAdded(const std::string& account_id) const { return added_ && account_id == account_id_; }
	inline void						Cancel() { added_ = false; }

	void							Add(const std::string&	account_id, const std::string& name, const std::string& portrait_url);
	void							Reset();

public:
	bool							added_{ false };
	std::string						account_id_;
};

struct BothStuInfoInClass
{
public:
	BothStuInfoInClass() = default;

	inline const StuInfoInClass&	StuOfSide(const PlayerSide play_side) const { return play_side == PlayerSide::Black? black_side_ : white_side_; }
	inline StuInfoInClass&			StuOfSide(const PlayerSide play_side) { return play_side == PlayerSide::Black ? black_side_ : white_side_; }

	inline const StuInfoInClass&	BlackSide() const { return black_side_; }
	inline StuInfoInClass&			BlackSide() { return black_side_; }

	inline const StuInfoInClass&	WhiteSide() const { return white_side_; }
	inline StuInfoInClass&			WhiteSide() { return white_side_; }

	inline bool						IsBothAdded() const { return black_side_.IsAdded() && white_side_.IsAdded(); }

	inline bool						IsAdded(const std::string& account_id) const { return black_side_.IsAdded(account_id) || white_side_.IsAdded(account_id); }

	void							Reset();

public:
	StuInfoInClass					black_side_;
	StuInfoInClass					white_side_;
};

extern BothStuInfoInClass BothStuInClass;

GAME_NAMESPACE_END

#endif