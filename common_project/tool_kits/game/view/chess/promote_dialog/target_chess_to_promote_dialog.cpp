#include "target_chess_to_promote_dialog.h"
#include <QPointer>
#include <QPixmap>
#include "common/uiutils/ui_utils.h"
#include "game/GeneratedFiles/ui_target_chess_to_promote.h"

GAME_NAMESPACE_BEGIN

static FacePathOfTargetChesses& FacesOfWhiteChesses() 
{
	static FacePathOfTargetChesses styles{
	std::string(":/chess/res/images/chess/rook_red.png"),
	std::string(":/chess/res/images/chess/knight_red.png"),
	std::string(":/chess/res/images/chess/bishop_red.png"),
	std::string(":/chess/res/images/chess/queen_red.png"),
};

	return styles; 
}

static FacePathOfTargetChesses& FacesOfBlackChesses() 
{ 
	static FacePathOfTargetChesses styles{
	std::string(":/chess/res/images/chess/rook_black.png"),
	std::string(":/chess/res/images/chess/knight_black.png"),
	std::string(":/chess/res/images/chess/bishop_black.png"),
	std::string(":/chess/res/images/chess/queen_black.png"),
};

	return styles; 
}

static QPointer<TargetChessToPromoteDialog> s_instance;

void TargetChessToPromoteDialog::SetFacesOfTargetChesses(const FacePathOfTargetChesses& styles_of_white, const FacePathOfTargetChesses& styles_of_black)
{
	FacesOfWhiteChesses() = styles_of_white;
	FacesOfBlackChesses() = styles_of_black;
}

ChessId TargetChessToPromoteDialog::GetTargetChessToPromote(const ChessColor color, bool* interrupted, QWidget* parent)
{
	auto chess_id_to_promote = ChessTrait::IdOfBlackQueen;

	if (!s_instance)
	{
		s_instance = new TargetChessToPromoteDialog(parent);

		chess_id_to_promote = s_instance->DoModal(color, interrupted);
	}

	return chess_id_to_promote;
}

void TargetChessToPromoteDialog::Interrupt()
{
	if (s_instance)
	{
		if (s_instance->interrupted_)
		{
			*s_instance->interrupted_ = true;
		}

		s_instance->close();
	}
}

TargetChessToPromoteDialog::TargetChessToPromoteDialog(QWidget* widget_to_align_on_center)
: QDialog()
, ui_(new Ui::TargetChessToPromateDialog)
{
	ui_->setupUi(this);

	this->Init();

	if (widget_to_align_on_center)
	{
		UiUtils::AlignWidgetOnCenter(this, widget_to_align_on_center);
	}
}

void TargetChessToPromoteDialog::Init()
{
	setWindowTitle(QString::fromLocal8Bit("Éý±ä"));
	setWindowIcon(QIcon(":/icon/res/images/icon/logo.ico"));
	this->setWindowFlags(Qt::FramelessWindowHint);
	this->setAttribute(Qt::WA_DeleteOnClose);
	this->setAttribute(Qt::WA_TranslucentBackground);
	this->setAutoFillBackground(true);

	this->setFixedSize(300, 116);
	ui_->fr_main->setStyleSheet("QFrame{background:rgba(0,0,0,60%); border-radius:4px;}");
	
	this->raise();
}

ChessId TargetChessToPromoteDialog::DoModal(const ChessColor color, bool* interrupted)
{
	auto chess_id_to_promote = ChessTrait::IdOfBlackQueen;

	interrupted_ = interrupted;
	chess_id_ = &chess_id_to_promote;

	const auto& faces = color == ChessColor::White ? FacesOfWhiteChesses() : FacesOfBlackChesses();

	struct  
	{
		QPushButton*	chess_;
		ChessType		type_;
		std::string		face_path_;
		QPixmap			pixmap_;
	}
	chesses[4]
	{
		{ ui_->chess_rook, ChessType::Rook, std::get<0>(faces), QPixmap(QString::fromStdString(std::get<0>(faces))) },
		{ ui_->chess_knight, ChessType::Knight, std::get<1>(faces), QPixmap(QString::fromStdString(std::get<1>(faces))) },
		{ ui_->chess_bishop, ChessType::Bishop, std::get<2>(faces), QPixmap(QString::fromStdString(std::get<2>(faces))) },
		{ ui_->chess_queen, ChessType::Queen, std::get<3>(faces), QPixmap(QString::fromStdString(std::get<3>(faces))) },
	};
	
	for (const auto& item : chesses)
	{
		connect(item.chess_, &QPushButton::clicked, [=]{ this->CloseWithChessId(ChessTrait::IdByColorAndType(color, item.type_)); });

		item.chess_->setStyleSheet(QString("QPushButton{ border:none; background-image:url(%1); }").arg(QString::fromStdString(item.face_path_)));
		item.chess_->setFixedSize(item.pixmap_.size());
		item.chess_->setCursor(Qt::CursorShape::PointingHandCursor);
	}

	this->setWindowModality(Qt::ApplicationModal);
	this->exec();

	return chess_id_to_promote;
}

void TargetChessToPromoteDialog::CloseWithChessId(const ChessId chess_id)
{
	if (chess_id_) { *chess_id_ = chess_id; }

	this->close();
}

GAME_NAMESPACE_END