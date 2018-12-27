#ifndef __COMMON_WEAK_CALLBACK_QOBJECT_SUPPORT_WEAK_CALLBACK_H__
#define __COMMON_WEAK_CALLBACK_QOBJECT_SUPPORT_WEAK_CALLBACK_H__
#include <type_traits>
#include <QObject>
#include <QPointer>
#include "base/callback/callback.h" // for StdClosure
#include "weak_callback.h"

namespace wcb
{
	class QObjectWeakCallbackHelper;
	
	template<typename ObjectT, typename = std::enable_if_t<std::is_base_of<QObject, ObjectT>::value>>
	class QObjectSupportWeakCallback
	{
		typedef std::shared_ptr<QObjectWeakCallbackHelper> QObjectWeakCallbackHelperSharedPtr;

	public:
		QObjectSupportWeakCallback(ObjectT* obj_ptr) : weak_callback_helper_(std::make_shared<QObjectWeakCallbackHelper>(obj_ptr)) {}

	protected:
		QObjectWeakCallbackHelperSharedPtr weak_callback_helper_;
	};

	class QObjectWeakCallbackHelper : public QObject, public SupportWeakCallback<QObjectWeakCallbackHelper>
	{
		Q_OBJECT

	public:
		QObjectWeakCallbackHelper(QObject* holder);

	public:
		inline void			EmitClosureToHolder(const StdClosure& closure) { emit this->SignalClosure(closure); }

	signals:
		void				SignalClosure(StdClosure);

	public slots:
		void				SlotClosure(StdClosure);

	private:
		QPointer<QObject>	holder_;
	};
}

#endif