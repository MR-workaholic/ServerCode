#include "Method.h"

Method::Method():
  _owner(NULL),
  _to_in_index(0),
  _to_out_index(0),
  _fr_in_index(0),
  _fr_out_index(0),
  _eof_stdin(false)
{}

void
Method::SetOwner(MyClient* owner)
{
  _owner = owner;
}

void
Method::StdinCB(ev::io &w, int revents)
{
  int fd = w.fd;
  int n = read(fd, &_to[_to_in_index], MAXLINE - _to_in_index);
  if (n < 0 && errno != EWOULDBLOCK) {
    std::cout << "read error on stdin"  << "\n";
    w.stop();
    _owner->_loop.break_loop();
  }else if(n == 0){
    std::cout << "EOF on stdin "  << "\n";
    _eof_stdin = true;
    w.stop();
    _owner->StartSockWrite();  // to shutdown
    // if (_to_in_index == _to_out_index) {
    //   shutdown(fd, );
    // }
  }else{
    _to_in_index += n;
    _owner->StartSockWrite();
  }
}

void
Method::SockinCB(ev::io& w, int revents)
{
  int fd = w.fd;
  int n = read(fd, &_fr[_fr_in_index], MAXLINE - _fr_in_index);
  if (n < 0 && errno != EWOULDBLOCK) {
    std::cout << "read error on socket" << "\n";
    w.stop();
    _owner->_loop.break_loop();
  }else if(n == 0){
    std::cout << "EOF on socket" << "\n";
    if (_eof_stdin) {
      std::cout << "Finish the communication" << "\n";
    }else{
      std::cout << "server wrong" << "\n";
    }
    w.stop();
    _owner->_loop.break_loop();
  }else{
    _fr_in_index += n;
    _owner->StartStdoutWrite();
  }
}

void
Method::StdoutCB(ev::io& w,int revents)
{
  int sum = _fr_in_index - _fr_out_index;
  if (sum <= 0) {
    w.stop();
    return;
  }
  int fd = w.fd;
  int n = write(fd, &_fr[_fr_out_index], sum);
  if (n < 0 && errno != EWOULDBLOCK) {
    std::cout << "write error on stdout"  << "\n";
    w.stop();
    _owner->_loop.break_loop();
  }else{
    _fr_out_index += n;
    if (_fr_out_index == _fr_in_index) {
      _fr_out_index = _fr_in_index = 0;
      w.stop();
    }
  }
}

void
Method::SockoutCB(ev::io& w,int revents)
{
  int fd = w.fd;
  int sum = _to_in_index - _to_out_index;
  if (sum <= 0) {
    w.stop();
    if (_eof_stdin) {
      shutdown(fd, SHUT_WR);
    }
    return;
  }
  int n = write(fd, &_to[_to_out_index], sum);
  if (n < 0 && errno != EWOULDBLOCK) {
    std::cout << "write error on socket out" << "\n";
    w.stop();
    _owner->_loop.break_loop();
  }else{
    _to_out_index += n;
    if (_to_out_index == _to_in_index) {
      _to_out_index = _to_in_index = 0;
      w.stop();
      if (_eof_stdin) {
        shutdown(fd, SHUT_WR);
      }
    }
  }
}
