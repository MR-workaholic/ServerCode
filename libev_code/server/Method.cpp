#include "Method.h"

Method::Method():
  _owner(NULL)
{}

void
Method::SetOwner(MyServer* owner)
{
  _owner = owner;
}

void
Method::SocketReadCB(ev::io& w, int revents)
{
  int fd = w.fd;
  if (_buffer.find(fd) == _buffer.end()) {
    DataBuffer dataBuffer;
    _buffer.insert(std::make_pair(fd, dataBuffer));
  }
  struct DataBuffer *pTempBuffer = &(_buffer.at(fd));
  int n = read(fd, &(*pTempBuffer)._data[(*pTempBuffer)._read_index], MAXLINE - (*pTempBuffer)._read_index);
  if (n < 0 && errno != EWOULDBLOCK ) {
    std::cout << "read error on socket-" << fd << "\n";
    CleanClient(fd);
  }else if(n == 0){
    std::cout << "read EOF on socket-" << fd << "\n";
    pTempBuffer->_eof_read = true;
    w.stop();
    if(_owner->StartSockWrite(fd) == false) // for close the connect
    {
      CleanClient(fd);
    }

  }else {
    pTempBuffer->_read_index += n;
    if(_owner->StartSockWrite(fd) == false)
    {
      CleanClient(fd);
    }
  }
}

void
Method::SocketWriteCB(ev::io& w, int revents)
{
  int fd = w.fd;
  DataBuffer* pTempBuffer = &(_buffer.at(fd));
  int sum = pTempBuffer->_read_index - pTempBuffer->_write_index;
  if (sum <= 0) {
    w.stop();
    if (pTempBuffer->_eof_read) {
      CleanClient(fd);
    }
    return;
  }
  int n = write(fd, &((*pTempBuffer)._data[pTempBuffer->_write_index]) ,sum);
  if (n < 0 && errno != EWOULDBLOCK) {
    std::cout << "write error on socket-" << fd << "\n";
    CleanClient(fd);
  }
  if (n > 0) {
    pTempBuffer->_write_index += n;
    if (pTempBuffer->_read_index == pTempBuffer->_write_index) {
      pTempBuffer->_read_index = pTempBuffer->_write_index = 0;
      w.stop();
      if (pTempBuffer->_eof_read) {
        CleanClient(fd);
      }
    }
  }

}

void
Method::CleanClient(int fd)
{
  _buffer.erase(fd);            // clean buffer
  _owner->ServerCleanClient(fd);
}
