#ifndef __COMMON_MACROS_ASSERT_H__
#define __COMMON_MACROS_ASSERT_H__
#include <assert.h>

#if defined(_DEBUG) || defined(DEBUG)
#include "log/log.h"

#define D_ASSERT(cond_expr, err_msg) if (!(cond_expr)) { LOG_ERR(L"Assert Failed: ") << err_msg; assert(false); }
#else
#define D_ASSERT(...)
#endif

#endif