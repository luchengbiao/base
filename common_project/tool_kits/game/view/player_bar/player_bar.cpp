#include "player_bar.h"
#include "game/GeneratedFiles/ui_player_bar.h"
#include "game/public/public_setting/public_setting.h"

GAME_NAMESPACE_BEGIN

PlayerBar::PlayerBar(QWidget* parent)
: QWidget(parent)
, ui_(new Ui::PlayerBar)
{
	ui_->setupUi(this);

	this->Init();
}

void PlayerBar::Init()
{
	QSize fixed_size(140, 45);
	this->setFixedSize(fixed_size);

	ui_->lb_name->setObjectName("game_lb_14_ffffff");
	ui_->head_portrait->setFixedSize(44, 44);
}

void PlayerBar::SetPlayerInfo(const VSPlayerInfo& player_info)
{
	if (player_info == player_info_) { return; }

	player_info_ = player_info;

	ui_->lb_name->setText(QString::fromStdString(player_info_.Name()));

	ui_->head_portrait->SetIsTeacher(player_info.Client() == ClientType::Teacher);
	ui_->head_portrait->SetHeadPortraitViaUrl(PlayerPortraitUrl(player_info_.PortraitUrl()));
}

GAME_NAMESPACE_END