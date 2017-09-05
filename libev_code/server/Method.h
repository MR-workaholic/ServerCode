#ifndef _METHOD_H_
#define _METHOD_H_

#include <unistd.h>             // read write
#include <errno.h>              // errno
#include <map>
#include <ev++.h>
#include <iostream>
#include "MyServer.h"

#define MAXLINE (2048)

class MyServer;

struct DataBuffer
{
  int _read_index;
  int _write_index;
  bool _eof_read;
  char _data[MAXLINE];

  DataBuffer():
    _read_index(0),
    _write_index(0),
    _eof_read(false)
  {}
};

class Method{

public:
  Method();
  void SetOwner(MyServer*);
  virtual void SocketReadCB(ev::io &, int);
  virtual void SocketWriteCB(ev::io &, int);

private:
  MyServer* _owner;
  std::map<int, DataBuffer> _buffer;

  void CleanClient(int);

};

#endif
