#include <coroutine.h>

#include <thread>
#include <iostream>
#include <pthread.h>
#include <iostream>
#include <chrono>
#include <mutex>
#include <memory>


namespace mylibco{

std::shared_ptr<Coroutine> cor;
std::shared_ptr<Coroutine> cor2;
class Test {

 public:
  std::mutex m_coroutine_mutex;
  int a = 1;
};
Test test_;

void fun1() {
  std::cout << "cor1 ---- now fitst resume fun1 coroutine by thread 1" << std::endl;
  std::cout << "cor1 ---- now begin to yield fun1 coroutine" << std::endl;

  test_.m_coroutine_mutex.lock();

  std::cout << "cor1 ---- coroutine lock on test_, sleep 5s begin" << std::endl;

  std::this_thread::sleep_for(std::chrono::milliseconds(5000));
  std::cout << "cor1 ---- sleep 5s end, now back coroutine lock" << std::endl;

  test_.m_coroutine_mutex.unlock();

  Coroutine::Yield();
  std::cout << "cor1 ---- fun1 coroutine back, now end" << std::endl;

}

void fun2() {
  std::cout << "cor222 ---- now fitst resume fun1 coroutine by thread 1" << std::endl;
  std::cout << "cor222 ---- now begin to yield fun1 coroutine" << std::endl;

  std::this_thread::sleep_for(std::chrono::milliseconds(2000));
  std::cout << "cor222 ---- coroutine2 want to get coroutine lock of test_" << std::endl;
  test_.m_coroutine_mutex.lock();

  std::cout << "cor222 ---- coroutine2 get coroutine lock of test_ succ" << std::endl;

}


void* thread1_func(void*) {
  std::cout << "thread 1 begin" << std::endl;
  std::cout << "now begin to resume fun1 coroutine in thread 1" << std::endl;
  Coroutine::Resume(cor.get());
  std::cout << "now fun1 coroutine back in thread 1"<< std::endl;
  std::cout << "thread 1 end" << std::endl;
  return NULL;
}

void* thread2_func(void*) {
  Coroutine::GetCurrentCoroutine();
  std::cout << "thread 2 begin" << std::endl;
  std::cout << "now begin to resume fun1 coroutine in thread 2" << std::endl;
  Coroutine::Resume(cor2.get());
  std::cout << "now fun1 coroutine back in thread 2" << std::endl;
  std::cout << "thread 2 end" << std::endl;
  return NULL;
}

} // namespace mylibc


void callback() {
  for(int i=0 ;i<10;++i) {
    printf("hello %d\n",i);
    // TODO hook sleep
  }
}

int main(int argc, char* argv[]) {
  // tinyrpc::SetHook(false);
  // std::cout << "main begin" << std::endl;
  // int stack_size = 128 * 1024;
  // char* sp = reinterpret_cast<char*>(malloc(stack_size));
  // cor = std::make_shared<tinyrpc::Coroutine>(stack_size, sp, fun1);

  // char* sp2 = reinterpret_cast<char*>(malloc(stack_size));
  // cor2 = std::make_shared<tinyrpc::Coroutine>(stack_size, sp2, fun2);


  // pthread_t thread2;
  // pthread_create(&thread2, NULL, &thread2_func, NULL);

  // thread1_func(NULL);

  // pthread_join(thread2, NULL);
  mylibco::cor = std::make_shared<mylibco::Coroutine>(); //main
  mylibco::cor2 = std::make_shared<mylibco::Coroutine>(128 * 1024, callback);
  mylibco::Coroutine::Resume(mylibco::cor2.get());

  std::cout << "main end" << std::endl;
}
