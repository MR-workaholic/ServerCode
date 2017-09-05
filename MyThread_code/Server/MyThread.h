#ifndef _MYTHREAD_H_
#define _MYTHREAD_H_

#include <pthread.h>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>
#include <string>

typedef boost::function<void()> ThreadCall;

class MyThread : boost::noncopyable
{
public:
  MyThread(const ThreadCall&, const std::string&);
  ~MyThread();
  std::string GetThreadName() const;
  pthread_t GetThreadID() const;
  bool IsThreadStart() const;
  int Start();
  void SetFunction(ThreadCall);

private:
  pthread_t _pid;
  std::string _thread_name;
  bool _thread_start;
  ThreadCall _fun;

  static void* StartThread(void*); // every Thread will call this one function because it is static
};

#endif
