#ifndef __GAME_VIEW_HEAD_PORTRAIT_HEAD_PORTRAIT_H__
#define __GAME_VIEW_HEAD_PORTRAIT_HEAD_PORTRAIT_H__
#include <functional>
#include <QColor>
#include <QWidget>
#include "base/memory/objectcounter.h"
#include "common/weak_callback/qobject_support_weak_callback.h"
#include "game/macros.h"

class WidgetWithPixmap;
class QImage;

GAME_NAMESPACE_BEGIN

class HeadPortrait : public QWidget, public wcb::QObjectSupportWeakCallback<QWidget>
{
	Q_OBJECT

	struct MyObjectCounter : nbase::ObjectCounter<MyObjectCounter> {};

public:
	explicit HeadPortrait(QWidget* parent = nullptr);
	explicit HeadPortrait(bool is_teahcer, QWidget* parent = nullptr);
	~HeadPortrait();

	void				SetHeadPortraitViaUrl(const std::string& file_url);
	void				SetHeadPortraitViaFile(const std::wstring& file_path);

	void				SetIsTeacher(bool is_teacher);
	
private:
	void				Init();

	void				UpdateHeadPortrait();

	virtual void		resizeEvent(QResizeEvent *) override;

private:
	WidgetWithPixmap*	portrait_{ nullptr };

	bool				is_teacher_{ true };

	std::string			file_url_;
	std::wstring		file_path_;

	MyObjectCounter		counter_;
};

GAME_NAMESPACE_END

#endif