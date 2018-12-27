#ifndef STOCKFISH_ADAPTER_POSITION_ADAPTER_FWD_H_INCLUDED
#define STOCKFISH_ADAPTER_POSITION_ADAPTER_FWD_H_INCLUDED
#include <memory>
#include "stockfish/macros.h"

STK_NAMESPACE_BEGIN

class PositionAdapter;

STK_NAMESPACE_END

typedef std::shared_ptr<stk::PositionAdapter> PositionAdapterPtr;

#endif