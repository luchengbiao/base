#ifndef __PROTO_MACROS_H__
#define __PROTO_MACROS_H__

#define PROTO_NAMESPACE_BEGIN namespace proto {

#define PROTO_NAMESPACE_END }

#ifdef _DEBUG
#define PROTO_TEST_MEMORY_LEAK
#endif

#ifdef PROTO_TEST_MEMORY_LEAK
#define CHECK_THE_NUMBER_OF_EXISTING_COMMANDS()  LOG_NWAR("The number of existing commands is ") << proto::Command::live()
#else
#define CHECK_THE_NUMBER_OF_EXISTING_COMMANDS()
#endif

#endif