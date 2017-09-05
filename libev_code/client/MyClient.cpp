#include "MyClient.h"

MyClient::MyClient(Method* pmethod, std::string serverIp, std::string serverPort):
  _pmethod(pmethod),
  _server_ip_(serverIp),
  _server_port_(serverPort)
{

  pmethod->SetOwner(this);

  // SetNonblock(STDIN_FILENO);
  // SetNonblock(STDOUT_FILENO);
  int val = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, val | O_NONBLOCK);
  val = fcntl(STDOUT_FILENO, F_GETFL, 0);
  fcntl(STDOUT_FILENO, F_SETFL, val | O_NONBLOCK);
}

bool
MyClient::Init()
{
  struct addrinfo hints, *res, *p;
  int errornum;

  bzero(&hints, sizeof(struct addrinfo));
  hints.ai_family = AF_UNSPEC;  // ipv4 or ipv6
  hints.ai_socktype = SOCK_STREAM; // TCP

  errornum = getaddrinfo(_server_ip_.c_str(),
                         _server_port_.c_str(),
                         &hints,
                         &res);
  if (errornum != 0) {
    std::cout << "error:" << gai_strerror(errornum)  << "\n";
    return false;
  }

  p = res;

  do
    {
      _socketfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
      if (_socketfd < 0) {
        continue;
      }

      if (connect(_socketfd, res->ai_addr, res->ai_addrlen) == 0) {
        break;
      }

      close(_socketfd);

    } while ((res = res->ai_next) != NULL);

  if(res == NULL){
    freeaddrinfo(p);
    return false;
  }

  freeaddrinfo(p);
  std::cout << "connect!!" << "\n";

  StartStdinRead();

  SetNonblock(_socketfd);
  _sockin_watcher_.set<Method, &Method::SockinCB>(_pmethod);
  _sockin_watcher_.set(_socketfd, ev::READ);
  _sockin_watcher_.start();
  // _sockout_watcher_.set(Method, &Method::SockoutCB)(_pmethod);

  return true;


}

void
MyClient::StartStdinRead()
{
  _stdin_watcher_.set<Method, &Method::StdinCB>(_pmethod);
  _stdin_watcher_.set(STDIN_FILENO, ev::READ);
  _stdin_watcher_.start();
}

void
MyClient::StartSockWrite()
{
  if(_sockout_watcher_.is_active()){
    return;
  }
  _sockout_watcher_.set<Method, &Method::SockoutCB>(_pmethod);
  _sockout_watcher_.set(_socketfd, ev::WRITE);
  _sockout_watcher_.start();
}

void
MyClient::StartStdoutWrite()
{
  if (_stdout_watcher_.is_active()) {
    return;
  }
  _stdout_watcher_.set<Method, &Method::StdoutCB>(_pmethod);
  _stdout_watcher_.set(STDOUT_FILENO, ev::WRITE);
  _stdout_watcher_.start();
}

void
MyClient::Close()
{
  close(_socketfd);
}

void
MyClient::SetNonblock(int& fd)
{
  int val = fcntl(fd, F_GETFL, 0);
  fcntl(fd, F_SETFL, val | O_NONBLOCK);
}
