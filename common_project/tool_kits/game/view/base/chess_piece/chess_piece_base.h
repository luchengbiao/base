#ifndef __GAME_VIEW_BASE_CHESS_PIECE_CHESS_PIECE_BASE_H__
#define __GAME_VIEW_BASE_CHESS_PIECE_CHESS_PIECE_BASE_H__
#include <memory>
#include <QWidget>
#include "game/model/base/chess/chess_info.h"
#include "game/view/base/chess_face_fwd.h"

class QPixmap;

GAME_NAMESPACE_BEGIN

class UIChessPieceBase : public QWidget
{
	Q_OBJECT

	typedef ChessFace* ChessFacePtr;
	typedef std::unique_ptr<QSize>  QSizeUniquePtr;

public:
	enum class UpdateReason
	{
		NoReason,
		ChessIdChanged,
		Forcely,
	};

public:
	explicit UIChessPieceBase(QWidget* parent = nullptr);

	void				SetChessId(const ChessInfo& chess_info);
	void				SetChessId(ChessId chess_id);
	inline ChessId		GetChessId() const { return chess_info_.GetId(); }
	inline bool			ChessIdIsEmpty() const { return chess_info_.IsEmpty(); }
	inline bool			ChessIdIsNotEmpty() const { return chess_info_.IsNotEmpty(); }

	void				HideFace();
	void				ShowFace();
	
	void				SetFaceFixedSize(const QSize& size);

	const QPixmap*		ChessPixmap() const;

	virtual void		UpdateChess(UpdateReason reason);

	virtual std::string ImagePath() const = 0;

	virtual void		Highlight();
	virtual void		Unhighlight();

protected:
	ChessInfo			chess_info_;
	ChessFacePtr		face_{ nullptr };
	QSizeUniquePtr		face_fixed_size_{ nullptr };
	bool				highlighted_{ false };
};

GAME_NAMESPACE_END

#endif