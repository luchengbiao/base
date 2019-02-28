#ifndef __COMMON_WEAK_CALLBACK_WEAK_CALLBACK_WITH_QOBJECT_H__
#define __COMMON_WEAK_CALLBACK_WEAK_CALLBACK_WITH_QOBJECT_H__
#include <QObject>
#include <QPointer>
#include "base/callback/callback.h" // for StdClosure
#include "weak_callback.h"

/*
*
* use WeakCallbackWithQObject like below:
*
* step 1 -- declare:
* class MyClass : public QObject[or sub class of QObject such as QWidget etc]
* {
*	...
*	wcb::WeakCallbackWithQObject weak_callback_;
*	...
* };
*
* step 2 -- initialize:
* MyClass::MyClass(...)
* : ...
* , weak_callback_(this) // construct WeakCallbackWithQObject with this pointer which is the context object of a connection
* , ...
* {
*	...
* }
*
* step 3 -- use:
* auto weak = weak_callback_.WeakPtr();
* someObject->RegisterCallback(weak_callback_.ToWeakCallback([=](...){
* weak.lock()->EmitClosureToContext([=]{
*	// do something in the thread of MyClass's host.
*	});
* }));
* 
* explanation:
* auto weak = weak_callback_.WeakPtr();
* someObject->RegisterCallback(weak_callback_.ToWeakCallback([=](...){ // here must capture the weak variable by value[=]or[weak] NOT by reference[&]or[&weak]
* // maybe called in a thread not same as MyClass's host.
* // weak.lock() definitely is a valid shared_ptr since the callback has been called and performed here.
* // emit a closure which will be called in the thread of MyClass's host.
* weak.lock()->EmitClosureToContext([=]{
*	// do something in the thread of MyClass's host.
*	});
* }));
*
*/

namespace wcb
{
	class QObjectHelper;

	class WeakCallbackWithQObject
	{
		typedef std::shared_ptr<QObjectHelper> HelperStrongPtr;
		typedef std::weak_ptr<QObjectHelper> HelperWeakPtr;

	public:
		WeakCallbackWithQObject(QObject* context) : helper_(std::make_shared<QObjectHelper>(context)) {}

		inline HelperStrongPtr StrongPtr() const { return helper_; }
		inline HelperWeakPtr WeakPtr() const { return helper_; }

		template<typename Callee>
		WeakCallback<Callee, QObjectHelper> ToWeakCallback(const Callee& callee)
		{
			return WeakCallback<Callee, QObjectHelper>(WeakPtr(), callee);
		}

	protected:
		HelperStrongPtr helper_;
	};

	class QObjectHelper : public QObject, public SupportWeakCallback<QObjectHelper>
	{
		Q_OBJECT

	public:
		QObjectHelper(QObject* context)
		{
			qRegisterMetaType<StdClosure>("StdClosure");

			//connect to a functor, with a "context" object defining in which event loop is going to be executed
			connect(this, &QObjectHelper::Closure, context, [](const StdClosure& closure){ if (closure) { closure(); } });
		}

		inline void			EmitClosureToContext(const StdClosure& closure) { emit this->Closure(closure); }

	private:
		Q_SIGNAL void		Closure(const StdClosure&);
	};
}

#endif