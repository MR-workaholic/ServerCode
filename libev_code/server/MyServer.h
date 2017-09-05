#ifndef _MYSERVER_H_
#define _MYSERVER_H_

#include <sys/socket.h>           // socket bind listen
#include <sys/types.h>
#include <unistd.h>             // close
#include <fcntl.h>
#include <netinet/in.h>         // sockaddr_in
#include <strings.h>            // bzero
#include <arpa/inet.h>          // htonl inet_aton
#include <fcntl.h>
#include <ev++.h>
#include <string>
#include <iostream>
#include "Method.h"
#define LISTENQ (1000)

class Method;

struct Client
{
  struct sockaddr_in _clientaddr;
  int _socketfd;
  ev::io* _psockin_watcher;
  ev::io* _psockout_watcher;
  Client* _next;

  Client():
    _socketfd(-1),
    _psockin_watcher(NULL),
    _psockout_watcher(NULL),
    _next(NULL)
  {}

};

class MyServer{
public:
  MyServer(Method* pmethod = NULL, std::string host = "127.0.0.1", std::string port = "54321");
  bool Init();
  bool StartSockWrite(int);
  void ServerCleanClient(int);
  ev::default_loop _loop;

private:
  Method* _pmethod;
  std::string _host;
  std::string _port;
  Client* _client_link;


  int _listenfd;
  ev::io _listen_watcher;
  struct sockaddr_in _serveraddr;


  void SetNonblock(int &);
  void ListenCB(ev::io &, int);
};


#endif
