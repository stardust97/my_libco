#include "coroutine_hook.h"

#include <cstring>
#include <assert.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <unistd.h>

#include "coroutine.h"
#include "fdevnet_manger.h"


// 获取系统IO操作的符号 命名为 g_sys_##name##_fun
#define HOOK_SYS_FUNC(name) name##_fun_ptr_t g_sys_##name##_fun = \
    (name##_fun_ptr_t)dlsym(RTLD_NEXT, #name);

HOOK_SYS_FUNC(accept);
HOOK_SYS_FUNC(read);
HOOK_SYS_FUNC(write);
HOOK_SYS_FUNC(connect);
HOOK_SYS_FUNC(sleep);


namespace stardust {
namespace libco {

static bool KEnableHook = true;

ssize_t read_hook(int fd, void *buf, size_t count){
  if(cur_coroutine_ == main_coroutine_) {
    return g_sys_read_fun(fd, buf, count);
  }

  // add to epoll
  auto& fd_manger = FdEventManager::GetInstance();
  FdEvent fd_event(fd, ReadEvent, cur_coroutine_);
  fd_manger.AddToEpoll(fd_event);

  Coroutine::Yield();
  return g_sys_read_fun(fd, buf, count);
}

ssize_t write_hook(int fd, const void *buf, size_t count)
{
}

int accept_hook(int sockfd, struct sockaddr *addr, socklen_t *addrlen) {
}

int connect_hook(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
}

unsigned int sleep_hook(unsigned int seconds) {
}

} // namespace libco
} // namespace stardust



extern "C" {

ssize_t read(int fd, void *buf, size_t count) {
  if(!stardust::libco::KEnableHook) {
    return g_sys_read_fun(fd, buf, count);
  } else {
    return stardust::libco::read_hook(fd, buf, count);
  }
}

ssize_t write(int fd, const void *buf, size_t count) {
  if(!stardust::libco::KEnableHook) {
    return g_sys_write_fun(fd, buf, count);
  } else {
    return stardust::libco::write_hook(fd, buf, count);
  }
}
}



