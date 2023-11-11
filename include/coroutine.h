#ifndef _MY_LIBCO_COROUTINE_H_
#define _MY_LIBCO_COROUTINE_H_

#include <functional>

#include "coctx.h"


namespace mylibco{
/// @brief 线程中的主协程，每个线程只有一个（thread_local）
static thread_local Coroutine* main_coroutine_{nullptr};
/// @brief 线程中当前正在运行的协程，每个线程只有一个
static thread_local Coroutine* cur_coroutine_{nullptr};

  
// 非对称有栈协程
class Coroutine{
  using Task = std::function<void()>;
public:
  /// @brief 用于构造一个从协程
  Coroutine(int stack_size, Task callback); 
  ~Coroutine();
  void SetCallback(Task t) {callback_ = t;};

public:
  /// @brief 主线程用于唤醒一个从线程
  static void Resume(Coroutine* co);
  /// @brief 从线程让出CPU
  static void Yeild();
  
private:
  /// @brief 默认构造函数用于构造一个主协程
  Coroutine(); 
  /// @brief 从协程创建后执行的函数
  static void co_fuction(Coroutine* co);

private:
  // NOTE 是否应该是thread_local
  /// @brief 自增的协程id
  static int32_t g_coroutine_id_;
  /// @brief 当前协程的数量
  static int32_t g_coroutine_nums_;

private:
  int32_t coroutine_id_; // 此协程id
  char* stack_; // 申请堆区协程栈时返回的初始地址
  int32_t stack_size_; // 申请堆区协程栈时返回的内存大小
  Task callback_; //协程执行的任务
  coctx contex_; // 协程寄存器上下文

};

int32_t Coroutine::g_coroutine_id_ = 0;
int32_t Coroutine:: g_coroutine_nums_ = 0;


} // namespace mylibco


#endif