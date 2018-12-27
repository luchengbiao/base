#include "qobject_support_weak_callback.h"

namespace wcb
{
	QObjectWeakCallbackHelper::QObjectWeakCallbackHelper(QObject* holder)
		: holder_(holder)
	{
		qRegisterMetaType<StdClosure>("StdClosure");
		connect(this, SIGNAL(SignalClosure(StdClosure)), this, SLOT(SlotClosure(StdClosure)));
	}

	void QObjectWeakCallbackHelper::SlotClosure(StdClosure closure)
	{
		if (closure && holder_) { closure(); }
	}
}