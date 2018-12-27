#include "create_new_board_dialog.h"
#include <QListWidget>
#include <QStringList>
#include "game/GeneratedFiles/ui_create_new_board.h"
#include "game/public/literal_stock/literal_stock.h"
#include "game/public/public_setting/public_setting.h"

GAME_NAMESPACE_BEGIN

UICreateNewBoardDialog::UICreateNewBoardDialog(QWidget* parent, bool fixed_vs_mode)
: QDialog(parent)
, ui_(new Ui::CreateNewBoardDialog)
{
	this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
	this->setModal(true);
	this->setAttribute(Qt::WA_TranslucentBackground);
	this->setAttribute(Qt::WA_DeleteOnClose);

	ui_->setupUi(this);

	this->Init(fixed_vs_mode);
}

void UICreateNewBoardDialog::Init(bool fixed_vs_mode)
{
	this->SetUiMode(fixed_vs_mode);

	this->SetUiGeometry();

	this->SetUiObjectName();

	this->RegisterSlots();

	this->FillOptions();

	ui_->com_black_operator->setEnabled(false);
	ui_->com_white_operator->setEnabled(false);
	ui_->com_first_operator->setEnabled(false);

	if (fixed_vs_mode)
	{
		ui_->com_mode->setCurrentIndex(1);
		ui_->com_mode->setEnabled(false);
	}

	//ui_->lineEdit->setText(QString::fromStdWString(cur_result_.chess_name_));
	ui_->lineEdit->setFocus(Qt::MouseFocusReason);
}

void UICreateNewBoardDialog::SetUiMode(bool fixed_vs_mode)
{
	if (UsageTypeIsClass())
	{
		ui_->lb_white_operator->hide();
		ui_->lb_black_operator->hide();
		ui_->com_white_operator->hide();
		ui_->com_black_operator->hide();

		ui_->formLayout->removeWidget(ui_->com_white_operator);
		ui_->formLayout->removeWidget(ui_->com_black_operator);
		ui_->formLayout->removeWidget(ui_->lb_white_operator);
		ui_->formLayout->removeWidget(ui_->lb_black_operator);
	}

	if (GameTypeIsChess()) // chess game
	{
		ui_->lb_grids->hide();
		ui_->com_grids->hide();

		ui_->formLayout->removeWidget(ui_->lb_grids);
		ui_->formLayout->removeWidget(ui_->com_grids);

		if (UsageTypeIsO2O()) // one 2 one
		{
			this->setFixedSize(424, 524);
		}
		else // class
		{
			this->setFixedSize(424, 400);
		}
	}
	else // go game
	{
		ui_->lb_first_operator->hide();
		ui_->com_first_operator->hide();

		ui_->formLayout->removeWidget(ui_->lb_first_operator);
		ui_->formLayout->removeWidget(ui_->com_first_operator);

		if (UsageTypeIsO2O()) // one 2 one
		{
			this->setFixedSize(424, 524);
		}
		else // class
		{
			this->setFixedSize(424, 400);
		}
	}

	ui_->formLayout->setVerticalSpacing(12);
}

void UICreateNewBoardDialog::FillOptions()
{
	QStringList mode_list;
	mode_list.push_back(QStringByKey(LK_TEACHING_MODE));
	mode_list.push_back(QStringByKey(LK_VS_MODE));
	ui_->com_mode->SetStringList(mode_list);

	QStringList status_list;
	status_list.push_back(QStringByKey(LK_INITIAL_BOARD));
	status_list.push_back(QStringByKey(LK_CURRENT_BOARD));
	ui_->com_status->SetStringList(status_list);

	QStringList grids_list;
	grids_list.push_back(QStringByKey(LK_GO9));
	grids_list.push_back(QStringByKey(LK_GO13));
	grids_list.push_back(QStringByKey(LK_GO19));
	ui_->com_grids->SetStringList(grids_list);

	QStringList role_list;
	role_list.push_back(QStringByKey(LK_TEACHER));
	role_list.push_back(QStringByKey(LK_STUDENT));
	ui_->com_black_operator->SetStringList(role_list);
	ui_->com_white_operator->SetStringList(role_list);

	QStringList first_list;
	if (GameTypeIsChess())
	{
		first_list.push_back(QStringByKey(LK_WHITE_FIRST));
		first_list.push_back(QStringByKey(LK_BLACK_FIRST));
	}
	else
	{
		first_list.push_back(QStringByKey(LK_BLACK_FIRST));
		first_list.push_back(QStringByKey(LK_WHITE_FIRST));
	}

	ui_->com_first_operator->SetStringList(first_list);

}

void UICreateNewBoardDialog::SetUiGeometry()
{
	ui_->btn_close->setCursor(QCursor(Qt::PointingHandCursor));
	ui_->btn_new->setCursor(QCursor(Qt::PointingHandCursor));
	ui_->btn_cancel->setCursor(QCursor(Qt::PointingHandCursor));
	ui_->fr_title->setFixedSize(424, 50);
	ui_->btn_new->setFixedSize(90, 36);
	ui_->btn_cancel->setFixedSize(90, 36);
	ui_->btn_close->setFixedSize(15, 15);
	ui_->lineEdit->setFixedHeight(50);

	ui_->lb_black_operator->setFixedHeight(50);
	ui_->lb_chess_name->setFixedHeight(50);
	ui_->lb_chess_mode->setFixedHeight(50);
	ui_->lb_chess_status->setFixedHeight(50);
	ui_->lb_first_operator->setFixedHeight(50);
	ui_->lb_grids->setFixedHeight(50);
	ui_->lb_white_operator->setFixedHeight(50);

	ui_->fr_button->setFixedSize(424, 86);
	ui_->lineEdit->setPlaceholderText(QStringByKey(LK_INPUT_NEW_BOARD_NAME));
	ui_->lineEdit->setMaxLength(10);
	ui_->com_black_operator->setFixedHeight(50);
	ui_->com_white_operator->setFixedHeight(50);

	ui_->com_first_operator->setFixedHeight(50);
	ui_->com_mode->setFixedHeight(50);
	ui_->com_status->setFixedHeight(50);
	ui_->com_grids->setFixedHeight(50);
	ui_->com_status->setMaxVisibleItems(5);

	ui_->fr_contents->setFixedSize(424, height() - ui_->fr_button->height() - ui_->fr_title->height());
}

void UICreateNewBoardDialog::SetUiObjectName()
{
	ui_->fr_title->setObjectName("game_fr_f7f7f7_r4_bottom");
	ui_->btn_close->setObjectName("game_btn_with_black_image");
	ui_->btn_cancel->setObjectName("game_btn_orange_white_r18");
	ui_->btn_new->setObjectName("game_btn_white_orange_r18");
	ui_->lineEdit->setObjectName("game_le_14_666666");
	ui_->lb_black_operator->setObjectName("game_lb_14_222222");
	ui_->lb_chess_mode->setObjectName("game_lb_14_222222");
	ui_->lb_chess_name->setObjectName("game_lb_14_222222");
	ui_->lb_chess_status->setObjectName("game_lb_14_222222");
	ui_->lb_first_operator->setObjectName("game_lb_14_222222");
	ui_->lb_grids->setObjectName("game_lb_14_222222");
	ui_->lb_title->setObjectName("game_lb_14_222222");
	ui_->lb_white_operator->setObjectName("game_lb_14_222222");
	ui_->com_black_operator->setObjectName("game_cmb_common");
	ui_->com_first_operator->setObjectName("game_cmb_common");
	ui_->com_mode->setObjectName("game_cmb_common");
	ui_->com_status->setObjectName("game_cmb_common");
	ui_->com_grids->setObjectName("game_cmb_common");
	ui_->fr_contents->setObjectName("game_fr_ffffff");
	ui_->fr_button->setObjectName("game_fr_ffffff_r4_bottom");
	ui_->com_white_operator->setObjectName("game_cmb_common");
}

void UICreateNewBoardDialog::RegisterSlots()
{
	connect(ui_->btn_close, SIGNAL(clicked()), this, SLOT(OnCloseBtnClicked()));
	connect(ui_->btn_cancel, SIGNAL(clicked()), this, SLOT(OnCloseBtnClicked()));
}

void UICreateNewBoardDialog::OnCloseBtnClicked()
{
	this->close();
}

GAME_NAMESPACE_END