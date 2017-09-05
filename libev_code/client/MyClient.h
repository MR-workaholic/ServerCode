#ifndef _MYCLIENT_H_
#define _MYCLIENT_H_

#include <sys/socket.h>         // socket
#include <sys/types.h>
#include <netdb.h>                // getaddrinfo struct addrinfo gai_strerror freeaddrinfo
#include <unistd.h>             // close
#include <fcntl.h>              // fcntl
#include <ev++.h>
#include <string>
#include <strings.h>            // bzero
#include <iostream>
#include "Method.h"

class Method;

class MyClient{

public:
  MyClient(Method* pmethod = NULL, std::string serverIp = "127.0.0.1", std::string serverPort = "54321");
  bool Init();
  void StartStdinRead();
  void StartSockWrite();
  void StartStdoutWrite();
  //ev::default_loop GetLoop(){return _loop;}
  void Close();
  ev::default_loop _loop;


private:
  Method* _pmethod;
  std::string _server_ip_;
  std::string _server_port_;
  // ev_io _stdin_watcher_;
  // ev_io _stdout_watcher_;
  // ev_io _sockin_watcher_;
  // ev_io _sockout_watcher_;
  ev::io _stdin_watcher_;
  ev::io _stdout_watcher_;
  ev::io _sockin_watcher_;
  ev::io _sockout_watcher_;
  int _socketfd;
  void SetNonblock(int&);

};


#endif
