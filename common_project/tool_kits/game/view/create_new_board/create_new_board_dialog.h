#ifndef __GAME_VIEW_CREATE_NEW_BOARD_CREATE_NEW_BOARD_DIALOG_H__
#define __GAME_VIEW_CREATE_NEW_BOARD_CREATE_NEW_BOARD_DIALOG_H__
#include <memory>
#include <QDialog>
#include "game/macros.h"

namespace Ui { class CreateNewBoardDialog; }

GAME_NAMESPACE_BEGIN

class UICreateNewBoardDialog : public QDialog
{
	Q_OBJECT

	typedef std::unique_ptr<Ui::CreateNewBoardDialog> MyUiPtr;

public:
	explicit UICreateNewBoardDialog(QWidget* parent = nullptr, bool fixed_vs_mode = false);

private:
	void		Init(bool fixed_vs_mode);

	void		SetUiMode(bool fixed_vs_mode);
	void		FillOptions();

	void		SetUiGeometry();
	void		SetUiObjectName();
	void		RegisterSlots();

public slots:
	void		OnCloseBtnClicked();

private:
	MyUiPtr		ui_;
};

GAME_NAMESPACE_END

#endif