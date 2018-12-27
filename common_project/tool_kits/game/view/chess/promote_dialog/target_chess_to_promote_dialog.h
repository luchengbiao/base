#ifndef __GAME_VIEW_CHESS_PROMOTE_DIALOG_TARGET_CHESS_TO_PROMOTE_DIALOG_H__
#define __GAME_VIEW_CHESS_PROMOTE_DIALOG_TARGET_CHESS_TO_PROMOTE_DIALOG_H__
#include <string>
#include <tuple>
#include <QDialog>
#include <QScopedPointer>
#include "game/model/base/chess/chess_info.h"
#include "game/model/chess/chess_trait.h"

namespace Ui { class TargetChessToPromateDialog; }

GAME_NAMESPACE_BEGIN

typedef std::tuple<std::string, std::string, std::string, std::string> FacePathOfTargetChesses; // <style_of_rook, style_of_knight, style_of_bishop, style_of_queen>

class TargetChessToPromoteDialog : public QDialog
{
	Q_OBJECT

	typedef QScopedPointer<Ui::TargetChessToPromateDialog> UiPtr;

public:
	static void		SetFacesOfTargetChesses(const FacePathOfTargetChesses& faces_of_white, const FacePathOfTargetChesses& faces_of_black);
	static ChessId  GetTargetChessToPromote(const ChessColor color, bool* interrupted = nullptr, QWidget* widget_to_align_on_center = nullptr);
	static void		Interrupt();

public:
	explicit TargetChessToPromoteDialog(QWidget* widget_to_align_on_center = nullptr);

	ChessId			DoModal(const ChessColor color, bool* interrupted = nullptr);

private:
	void			Init();

	void			CloseWithChessId(const ChessId chess_id);

private:
	UiPtr			ui_{ nullptr };

	// passed from outside, you are responsible for managing the life of the values referred
	bool*			interrupted_{ nullptr };
	ChessId*		chess_id_{ nullptr };
};

GAME_NAMESPACE_END

#endif