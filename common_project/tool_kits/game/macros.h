#ifndef __GAME_MACROS_H__
#define __GAME_MACROS_H__

#define GAME_NAMESPACE_BEGIN namespace game {

#define GAME_NAMESPACE_END }

#ifdef _DEBUG
#define GAME_TEST_MEMORY_LEAK
#endif

#ifdef GAME_TEST_MEMORY_LEAK
#define CHECK_THE_NUMBER_OF_EXISTING_ACTIONS()  LOG_NWAR("The number of existing actions is ") << game::GameAction::live()
#else
#define CHECK_THE_NUMBER_OF_EXISTING_ACTIONS()
#endif

#define LOG_O2O_OR_O2M_WITH_ARGS(format, args) \
if (UsageTypeIs(UsageType::Online_Class)) \
{ LOG_O2M(format) << args; } \
else { LOG_O2O(format) << args; }

#define LOG_O2O_OR_O2M(format) \
if (UsageTypeIs(UsageType::Online_Class)) \
{ LOG_O2M(format); } \
else { LOG_O2O(format); }

#endif