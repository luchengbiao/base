#ifndef __GAME_VIEW_PLAYER_BAR_PLAYER_BAR_H__
#define __GAME_VIEW_PLAYER_BAR_PLAYER_BAR_H__
#include <memory>
#include <QWidget>
#include "game/public/structs/vs_player_info/vs_player_info.h"

namespace Ui { class PlayerBar; }

GAME_NAMESPACE_BEGIN

class PlayerBar : public QWidget
{
	typedef std::unique_ptr<Ui::PlayerBar> MyUiPtr;

public:
	explicit PlayerBar(QWidget* parent = nullptr);

	void				SetPlayerInfo(const VSPlayerInfo& player_info);

private:
	void				Init();

private:
	MyUiPtr				ui_;
	VSPlayerInfo		player_info_;
};

GAME_NAMESPACE_END

#endif