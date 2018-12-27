#ifndef __GAME_VIEW_ANIMATION_ANIMATION_LAYER_H__
#define __GAME_VIEW_ANIMATION_ANIMATION_LAYER_H__
#include <functional>
#include <memory>
#include <vector>
#include <QColor>
#include <QWidget>
#include "game/macros.h"
#include "game/view/animation/animation_delegate.h"

class QPropertyAnimation;

GAME_NAMESPACE_BEGIN

class ChessVariation;
class UIArrowLine;
class UIGridMark;

class UIAnimationLayer : public QWidget
{
	Q_OBJECT

	struct ChessFaceInfo;
	typedef std::vector<ChessFaceInfo*> ChessFaceInfoVector;

	typedef std::vector<UIGridMark*> GridMarkVector;

public:
	explicit UIAnimationLayer(QWidget* parent = nullptr);
	~UIAnimationLayer();

	void					SetAnimationDelegate(UIAnimationDelegate* animation_delegate);

	void					PlayChessVariation(const ChessVariation& chess_var, 
											   const std::function<void()>& moved_callback = nullptr, 
											   bool show_arrow_line = false);

	void					PlayChessVariationWithPassingPoint(const ChessVariation& chess_var,
															   const QPoint& passing_pos_global,
															   const std::function<void()>& moved_callback = nullptr,
															   bool show_arrow_line = false);

	void					MoveChess(ChessId chess_id,
									  const GridIndex& grid_from,
									  const GridIndex& grid_to,
									  const std::function<void()>& moved_callback = nullptr,
									  bool show_arrow_line = false);

	void					MoveChessWithPassingPoint(ChessId chess_id,
													  const GridIndex& grid_from,
													  const QPoint& passing_pos_global,
													  const GridIndex& grid_to,
													  const std::function<void()>& moved_callback = nullptr,
													  bool show_arrow_line = false);

	void					MarkOnGridsInBoard(const std::vector<GridIndex>& grids, const QSize& size, const QColor& color = Qt::GlobalColor::green);

	bool					HasAnimationPlaying() const;

	void					StopAnimations();

	void					HideGridMarks();

	template<typename GridMotionT>
	inline void				ShowArrowLine(ChessId chess_id, const GridMotionT& motion) { ShowArrowLine(chess_id, motion.From(), motion.To()); }
	void					ShowArrowLine(ChessId chess_id, const GridIndex& grid_from, const GridIndex& grid_to);
	void					ShowArrowLine(const QPoint& pos_start_global, const QPoint& pos_end_global);
	void					HideArrowLine();

	void					Clear(); // StopAnimations & HideArrowLine

protected:
	ChessFaceInfo*			GetOrCreateChessFace();
	UIGridMark*				GetOrCreateGridMark();
	UIArrowLine*			GetOrCreateArrowLine();

private:
	UIAnimationDelegate*	animation_delegate_{ nullptr };

	ChessFaceInfoVector		vec_chess_face_;

	GridMarkVector			vec_grid_mark_;

	UIArrowLine*			arrow_line_{ nullptr };
	QPropertyAnimation*		animation_traced_by_arrow_line_{ nullptr };
};

GAME_NAMESPACE_END

#endif