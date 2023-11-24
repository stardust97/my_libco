#ifndef _MY_LIBCO_FD_EVENT_MANGER_H_
#define _MY_LIBCO_FD_EVENT_MANGER_H_

#include <unordered_map>
#include <memory>

#include "coroutine.h"

namespace stardust{
namespace libco{

enum {
  ReadEvent = 1,
  WriteEvent = 2
};

class FdEvent {
public:
  FdEvent(int32_t fd, uint32_t event, Coroutine* co);
  ~FdEvent();
  int32_t fd_;    // 事件的文件描述符
  uint32_t event_; // 关心的事件
  Coroutine *co_; // 事件所在的协程
};


class FdEventManager {
public:
  static FdEventManager& GetInstance();
  void AddToEpoll(FdEvent fd);

protected:
  FdEventManager();

private:
  void set_no_blocked(int32_t fd);

  /// @brief 用协程id记录其上的Event
  std::unordered_map<int32_t, std::weak_ptr<FdEvent>> fd_enevts_;
};




} // namespace libco
} // namespace libco
  
  



#endif // _MY_LIBCO_FD_EVENT_MANGER