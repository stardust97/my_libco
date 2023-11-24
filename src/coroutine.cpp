#include "coroutine.h"

#include <cstring>
#include <cassert>

namespace stardust{
namespace libco{

Coroutine::Coroutine(): coroutine_id_{g_coroutine_id_++}, stack_{nullptr},
    stack_size_{0} {
  // 主协程不需要创建栈，直接使用线程的栈
  main_coroutine_ = this;
  cur_coroutine_ = this;
  memset(&contex_, 0, sizeof(coctx));
  // 不需要为主协程的m_coctx设置初值，
  // 因为后面协程切换的时候会保存当前寄存器上下文到主协程的contex_中的。
}

Coroutine::~Coroutine(){
  printf("cor id %d end\n",coroutine_id_);
}

Coroutine::Coroutine(int stack_size, Task callback) : coroutine_id_{g_coroutine_id_++},
    stack_{nullptr}, stack_size_{stack_size}, callback_(callback){
  if(!main_coroutine_) {
    main_coroutine_ = new Coroutine();
  }
  assert(main_coroutine_ != nullptr);
  stack_ = static_cast<char*>(malloc(stack_size));
  char* top = stack_ + stack_size_; // 注意栈是向下增长的,堆相反
  // 由于64位系统对栈的操作(push、pop)都是8字节对齐的
  // 所以需要对栈顶进行8字节对齐
  top = reinterpret_cast<char*> ( 
      (reinterpret_cast<unsigned long>(top) & -16LL) );
  memset(&contex_, 0, sizeof(coctx));
  contex_.regs[KRSP] = top;
  contex_.regs[KRBP] = top;
  // 从协程首先执行co_functino函数
  contex_.regs[KRETAddr] = reinterpret_cast<void*>(&co_fuction); // TODO 能否使用std::bind，就不用使用static了
  contex_.regs[KRDI] = reinterpret_cast<void*>(this);
  
}

void Coroutine::Resume(Coroutine* co) {
  assert(co!=nullptr);
  // 只有在主线程中才能执行Resume
  if(cur_coroutine_ != main_coroutine_) {
    perror("non main resume!\n");
    exit(-1);
  }
  if(main_coroutine_ == nullptr) {
    perror("main cor null\n");
    exit(-1);
  }
  cur_coroutine_ = co;
  coctx_swap(&(main_coroutine_-> contex_), &(co->contex_));
  printf("swap to co id: %d\n", co->coroutine_id_);
}

void Coroutine::Yield() {
  // 只有在从线程中才能执行Yeild
  assert(main_coroutine_ != nullptr);
  assert(main_coroutine_ != cur_coroutine_);
  auto tmp_co = cur_coroutine_;
  cur_coroutine_ = main_coroutine_;
  coctx_swap(&(tmp_co->contex_), &(main_coroutine_->contex_));
}

Coroutine *Coroutine::GetCurrentCoroutine() {
  return cur_coroutine_;
};

void Coroutine::co_fuction(Coroutine* co) {
  assert( co!= nullptr);
  if(co -> callback_) {
    co -> callback_(); // 执行从协程的任务
  }
  Yield(); // 执行完从协程的任务后，将CPU还给主协程
}


} // namespace libco
} // namespace stardust


