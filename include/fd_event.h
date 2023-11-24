#ifndef _MY_LIBCO_FD_EVENT_H_
#define _MY_LIBCO_FD_EVENT_H_


#include "coroutine.h"

namespace stardust{
namespace libco{

class FdEvent {
public:
  FdEvent();
  ~FdEvent();

private:
  int32_t fd_; // 事件的文件描述符
  Coroutine co_; // 事件所在的协程
};

} // namespace libco
} // namespace libco

#endif // _MY_LIBCO_FD_EVENT_H_