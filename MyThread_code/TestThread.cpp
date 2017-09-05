#include "MyThread.h"
#include <iostream>
#include <boost/bind.hpp>
#include <string>
#include <time.h>

const int N = 5;

void ThreadFunc(int offset){
  for (int i = 0; i < N; i++) {
    sleep(1);
    std::cout << i + offset << "\n";
  }
}

void PrintNum(int a, int b)
{
  std::cout << "the num is  " << a << " and " << b << "\n";
  sleep(2);
}

int main(int argc, char *argv[])
{
  ThreadCall f, g;

  f = boost::bind(&ThreadFunc, 100);
  std::string name = "thread1";
  MyThread thread1(f, name);
  thread1.Start();

  name = "thread2";
  g = boost::bind(&PrintNum, 5, 6);
  f = boost::bind(&ThreadFunc, 200);
  MyThread thread2(f, name);
  thread2.Start();

  std::cout << "Thread " << thread1.GetThreadName() << " id is " << thread1.GetThreadID()  << "\n";
  std::cout << "Thread " << thread2.GetThreadName() << " id is " << thread2.GetThreadID()  << "\n";

  sleep(10);
  thread1.SetFunction(g);
  thread1.Start();
  std::cout << "Thread " << thread1.GetThreadName() << " id is " << thread1.GetThreadID()  << "\n";

  sleep(6);

  return 0;
}
