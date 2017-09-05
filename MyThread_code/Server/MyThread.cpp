#include <pthread.h>
#include <string>
#include <iostream>
#include "MyThread.h"

MyThread::MyThread(const ThreadCall& func, const std::string& threadName):
  _thread_name(threadName),
  _fun(func)
{
  _pid = 0;
  _thread_start = false;
}

MyThread::~MyThread()
{
  std::cout << "delete " << _thread_name << "\n";
}

std::string
MyThread::GetThreadName() const
{
  return _thread_name;
}

pthread_t
MyThread::GetThreadID() const
{
  return _pid;
}

bool
MyThread::IsThreadStart() const
{
  return _thread_start;
}

int
MyThread::Start()
{
  if (!_thread_start) {
    int result = pthread_create(&_pid, NULL, &StartThread, this); // success return 0
    if (result == 0) {
      _thread_start = true;
      return result;
    }
  }
  return 0;
}

void
MyThread::SetFunction(ThreadCall newFun)
{
  _fun = newFun;
}

void*
MyThread::StartThread(void* obj)
{
  MyThread* pthis = static_cast<MyThread*>(obj);
  pthis->_fun();
  pthis->_pid = 0;
  pthis->_thread_start = false;
  return NULL;
}
