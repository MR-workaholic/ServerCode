#ifndef _METHOD_H_
#define _METHOD_H_

#include <unistd.h>             // read write
#include <errno.h>              // errno
#include <iostream>
#include <ev++.h>
#include "MyClient.h"

#define MAXLINE (2048)

class MyClient;  //  前置声明

class Method{
public:
  Method();
  virtual void StdinCB(ev::io &, int);
  virtual void StdoutCB(ev::io &, int);
  virtual void SockinCB(ev::io &, int);
  virtual void SockoutCB(ev::io &, int);
  void SetOwner(MyClient* owner);

private:
  MyClient* _owner; 
  char _to[MAXLINE], _fr[MAXLINE];
  int _to_in_index, _to_out_index, _fr_in_index, _fr_out_index;
  bool _eof_stdin;

};

#endif
