#include "MyThread.h"
#include <iostream>
// #include <boost/bind.hpp>
#include <functional>
#include <string>
// #include <time.h>
#include <thread>
#include <chrono>         // std::chrono::seconds

using std::bind;
using std::chrono::seconds;


const int N = 5;

void ThreadFunc(int offset){
  for (int i = 0; i < N; i++) {
    // sleep(1);
    // std::this_thread::sleep_for(1s);
    std::this_thread::sleep_for(seconds(1));

    std::cout << i + offset << "\n";
  }
}

void PrintNum(int a, int b)
{
  std::cout << "the num is  " << a << " and " << b << "\n";
  // sleep(2);
  // std::this_thread::sleep_for(2s);
  std::this_thread::sleep_for(seconds(2));

}

int main(int argc, char *argv[])
{
  ThreadCall f, g;

  f = bind(&ThreadFunc, 100);
  std::string name = "thread1";
  MyThread thread1(f, name);
  thread1.Start();

  name = "thread2";
  g = bind(&PrintNum, 5, 6);
  f = bind(&ThreadFunc, 200);
  MyThread thread2(f, name);
  thread2.Start();

  std::cout << "Thread " << thread1.GetThreadName() << " id is " << thread1.GetThreadID()  << "\n";
  std::cout << "Thread " << thread2.GetThreadName() << " id is " << thread2.GetThreadID()  << "\n";

  // sleep(10);
  std::this_thread::sleep_for(seconds(10));

  thread1.SetFunction(g);
  thread1.Start();
  std::cout << "Thread " << thread1.GetThreadName() << " id is " << thread1.GetThreadID()  << "\n";

  // sleep(6);
  std::this_thread::sleep_for(seconds(6));


  return 0;
}
