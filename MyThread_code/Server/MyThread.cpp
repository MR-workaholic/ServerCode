#include <pthread.h>
#include <string>
#include "MyThread.h"

MyThread::MyThread(const ThreadCall& func, const std::string& threadName):
  _thread_name(threadName),
  _fun(func)
{
  _pid = 0;
 if (pthread_mutex_init(&mutex_flag, NULL) != 0) {
   // 互斥锁初始化失败
   cout << "init mutex fail" << endl;
 }else {
   // std::cout << "init mutex success" << "\n";
 }
  _thread_start = false;
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
  bool res;
  {
    MutexGuard guard(mutex_flag);
    res = _thread_start;
  }
  return res;
}

int
MyThread::Start()
{
  //cout << "Start " << _thread_name  << endl;
  {
    MutexGuard guard(mutex_flag);
    if (!_thread_start) {
      int result = pthread_create(&_pid, NULL, &StartThread, this); // success return 0
      if (result == 0) {
        _thread_start = true;
        return result;
      }
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
  {
    MutexGuard guard(pthis->mutex_flag);
    pthis->_thread_start = false;
  }
  return NULL;
}
void
MyThread::Join() {
  while (true) {
    {
      MutexGuard guard(mutex_flag);
      if (_thread_start == false) {
        return ;
      }
    }
  }
}