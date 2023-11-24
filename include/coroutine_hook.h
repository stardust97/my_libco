#ifndef _MY_LIBCO_COROUTINE_HOOK_H_
#define _MY_LIBCO_COROUTINE_HOOK_H_

#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>

// 协程调用IO时，需要调用异步调用，防止线程被阻塞，
// 而系统IO大多为同步调用，因此需要hook系统调用
// 实现同名的IO函数调用，让协程使用者对IO调用无感

// 在linux下，如果多个动态库有同名的全局符号的情况下，
// 会加载第一个动态库中的符号，忽略后面的动态库中的同名符号
// 改动态链接顺序，把自己的IO操作库放在前面，
// 保证自己的库在glibc库之前被加载，就能实现异步IO

// 在我们自己的IO库中，调用对应的系统IO执行真正的操作
// 由于hook了系统的IO函数，我们直接使用read等函数
// 也会调用到自己的函数，因此我们调用系统IO函数时，
// 需要借助cpp文件中定义的HOOK_SYS_FUNC宏调用
// 获取系统调用的符号，利用这个符号去调用系统IO库
using read_fun_ptr_t = ssize_t(*)(int fd, void *buf, size_t count);
using write_fun_ptr_t = ssize_t(*)(int fd, const void *buf, size_t count);
using connect_fun_ptr_t = int (*)(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
using accept_fun_ptr_t = int (*)(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
using socket_fun_ptr_t = int (*)(int domain, int type, int protocol);
using sleep_fun_ptr_t = int (*)(unsigned int seconds);

namespace stardust{
namespace libco{
  int accept_hook(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

  ssize_t read_hook(int fd, void *buf, size_t count);

  ssize_t write_hook(int fd, const void *buf, size_t count);

  int connect_hook(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

  unsigned int sleep_hook(unsigned int seconds);

  void SetHook(bool);

}
}

extern "C"
{

  int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

  ssize_t read(int fd, void *buf, size_t count);

  ssize_t write(int fd, const void *buf, size_t count);

  int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

  unsigned int sleep(unsigned int seconds);
}

#endif // _MY_LIBCO_COROUTINE_HOOK_H_

