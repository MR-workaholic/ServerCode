#include "MyServer.h"

MyServer::MyServer(Method* pmethod, std::string host, std::string port):
  _pmethod(pmethod),
  _host(host),
  _port(port),
  _client_link(NULL)
{
  _pmethod->SetOwner(this);
}

bool
MyServer::Init()
{
  // socket
  _listenfd = socket(AF_INET, SOCK_STREAM, 0);
  // bind
  bzero(&_serveraddr, sizeof(_serveraddr));
  _serveraddr.sin_family = AF_INET;
  inet_aton(_host.c_str(), &_serveraddr.sin_addr);
  _serveraddr.sin_port = htons(static_cast<uint16_t>(atoi(_port.c_str())));
  bind(_listenfd, (struct sockaddr *)&_serveraddr, sizeof(_serveraddr));
  // listen
  listen(_listenfd, LISTENQ);

  SetNonblock(_listenfd);
  _listen_watcher.set<MyServer, &MyServer::ListenCB>(this);
  _listen_watcher.set(_listenfd, ev::READ);
  _listen_watcher.start();

  return true;
}

bool
MyServer::StartSockWrite(int fd)
{
  Client *pClient = _client_link;
  while (pClient != NULL && pClient->_socketfd != fd) {
    pClient = pClient->_next;
  }
  if (pClient == NULL) {
    return false;
  }

  if (pClient->_psockout_watcher == NULL) {
    pClient->_psockout_watcher = new ev::io;
  }
  if (pClient->_psockout_watcher->is_active()) {
    return true;
  }
  pClient->_psockout_watcher->set<Method, &Method::SocketWriteCB>(_pmethod);
  pClient->_psockout_watcher->set(pClient->_socketfd, ev::WRITE);
  pClient->_psockout_watcher->start();

  return true;
}

void
MyServer::ServerCleanClient(int fd)
{
  Client* curClient = _client_link;
  if (curClient->_socketfd == fd) {
    _client_link = curClient->_next;
  }else{
    Client* preClient;
    do
      {
        preClient = curClient;
        curClient = curClient->_next;
      } while (curClient != NULL && curClient->_socketfd != fd);

    if (curClient == NULL) {
      return;
    }

    preClient->_next = curClient->_next;
  }

  if (curClient->_psockin_watcher != NULL) {
    curClient->_psockin_watcher->stop();
    delete curClient->_psockin_watcher;
    curClient->_psockin_watcher = NULL;
  }

  if (curClient->_psockout_watcher != NULL) {
    curClient->_psockout_watcher->stop();
    delete curClient->_psockout_watcher;
    curClient->_psockout_watcher = NULL;
  }

  std::cout << "close the socket-" << fd << "\n";
  close(fd);
  delete curClient;
  curClient = NULL;
}

void
MyServer::SetNonblock(int& fd)
{
  int val = fcntl(fd, F_GETFL, 0);
  fcntl(fd, F_SETFL, val | O_NONBLOCK);
}


void
MyServer::ListenCB(ev::io &w, int revents)
{
  int fd = w.fd;
  Client* client = new Client();
  socklen_t addrlen = sizeof(client->_clientaddr);
  client->_socketfd = accept(fd, (struct sockaddr *)&(client->_clientaddr), &addrlen);
  SetNonblock(client->_socketfd);

  client->_psockin_watcher = new ev::io;
  client->_psockin_watcher->set<Method, &Method::SocketReadCB>(_pmethod);
  client->_psockin_watcher->set(client->_socketfd, ev::READ);
  client->_psockin_watcher->start();

  // client->_psockout_watcher = new ev::io;
  // client->_psockout_watcher->set<Method, &Method::SocketWriteCB>(_pmethod);
  // client->_psockout_watcher->set(client->_socketfd, ev::WRITE);
  // client->_psockout_watcher->start();

  if (_client_link == NULL) {
    _client_link = client;
  }else{
    client->_next = _client_link;
    _client_link = client;
  }
}
