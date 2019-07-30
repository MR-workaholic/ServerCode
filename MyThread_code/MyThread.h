#ifndef _MYTHREAD_H_
#define _MYTHREAD_H_

#include <pthread.h>
// #include <boost/function.hpp>
// #include <boost/bind.hpp>
// #include <boost/noncopyable.hpp>
#include <string>
#include <functional>
#include <iostream>

using std::function;
using std::bind;
using std::placeholders::_1;
using std::placeholders::_2;
using std::cout;
using std::endl;

typedef function<void()> ThreadCall;

class Noncopyable {

protected:
  Noncopyable(){}
  ~Noncopyable(){}
private:
  Noncopyable(const Noncopyable&);
  Noncopyable& operator=(const Noncopyable&);

};

  class MutexGuard {
  public:
    MutexGuard(pthread_mutex_t lock) : _mutex(lock){
      if (pthread_mutex_lock(&_mutex) != 0){
        cout << "lock failed"  << endl;
      }
    }
    ~MutexGuard() {
      pthread_mutex_unlock(&_mutex);
    }
  private:
    pthread_mutex_t _mutex;
  };


class MyThread : Noncopyable
{
public:
  MyThread(const ThreadCall&, const std::string&);
  std::string GetThreadName() const;
  pthread_t GetThreadID() const;
  bool IsThreadStart() const;
  int Start();
  void SetFunction(ThreadCall);
  void Join();
  pthread_mutex_t mutex_flag;

private:
  pthread_t _pid;
  std::string _thread_name;
  bool _thread_start;
  ThreadCall _fun;

  static void* StartThread(void*); // every Thread will call this one function because it is static
};

#endif
