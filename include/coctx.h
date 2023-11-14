#ifndef _MY_LIBCO_COCTX_H_
#define _MY_LIBCO_COCTX_H_

enum {
  KRBP = 6, // 栈底指针寄存器
  KRDI = 7, // DI寄存器，保存函数调用的第一个参数
  KRSI = 8, // SI寄存器，保存函数调用的第二个参数
  KRETAddr = 9, // 存放于栈底的，函数栈应该返回的调用方地址
  KRSP = 13 // 栈顶指针寄存器
};

struct coctx {
  void* regs[14]; // 寄存器数组
};


extern "C" {
  // 汇编函数coctx_swap， 保存旧协程寄存器的值到coctx结构体中，
  // 并将新协程寄存器的值从coctx中恢复到寄存器中
  extern void coctx_swap(coctx *, coctx *) asm("coctx_swap");

};





#endif