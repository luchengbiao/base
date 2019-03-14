#ifndef __COMMON_WEAK_CALLBACK_WEAK_CALLBACK_WITH_QOBJECT_H__
#define __COMMON_WEAK_CALLBACK_WEAK_CALLBACK_WITH_QOBJECT_H__
#include <QObject>
#include <QPointer>
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
	typedef std::function<void()> StdClosure;

	class QObjectSupportWeakCallback;

	class WeakCallbackWithQObject
	{
		typedef std::shared_ptr<QObjectSupportWeakCallback> QObjectSupportWeakCallbackSharedPtr;
		typedef std::weak_ptr<QObjectSupportWeakCallback> QObjectSupportWeakCallbackWeakPtr;

	public:
		WeakCallbackWithQObject(QObject* context) : helper_(std::make_shared<QObjectSupportWeakCallback>(context)) {}

		inline QObjectSupportWeakCallbackSharedPtr SharedPtr() const { return helper_; }
		inline QObjectSupportWeakCallbackWeakPtr WeakPtr() const { return helper_; }

		template<typename Callee>
		WeakCallback<Callee, QObjectSupportWeakCallback> ToWeakCallback(const Callee& callee)
		{
			return WeakCallback<Callee, QObjectSupportWeakCallback>(WeakPtr(), callee);
		}

	protected:
		QObjectSupportWeakCallbackSharedPtr helper_;
	};

	class QObjectSupportWeakCallback : public QObject, public SupportWeakCallback<QObjectSupportWeakCallback>
	{
		Q_OBJECT

	public:
		QObjectSupportWeakCallback(QObject* context)
		{
			qRegisterMetaType<StdClosure>("StdClosure");

			//connect to a functor, with a "context" object defining in which event loop is going to be executed
			connect(this, &QObjectSupportWeakCallback::Closure, context, [](const StdClosure& closure){ if (closure) { closure(); } });
		}

		QObjectSupportWeakCallback() 
			: QObjectSupportWeakCallback(this) 
		{}

		inline void			EmitClosureToContext(const StdClosure& closure) { emit this->Closure(closure); }

	private:
		// it is safe that the parameter type of Closure signal is a reference -- const StdClosure&,
		// because the parameter will be copied(QMetaType::create->QMetaType::construct->QMetaTypeFunctionHelper::Construct) if in a async/queued call(queued_activate),
		// in a word: by reference, it is efficient in a sync call and Qt makes it safe in a async/queued call. 
		Q_SIGNAL void		Closure(const StdClosure&);
	};
}

#endif