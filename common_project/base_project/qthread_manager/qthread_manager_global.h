#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(QTHREAD_MANAGER_LIB)
#  define QTHREAD_MANAGER_EXPORT Q_DECL_EXPORT
# else
#  define QTHREAD_MANAGER_EXPORT Q_DECL_IMPORT
# endif
#else
# define QTHREAD_MANAGER_EXPORT
#endif
