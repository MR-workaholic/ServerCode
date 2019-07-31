#include "MyThread.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <functional>
#include <netinet/in.h>         // struct sockaddr_in
#include <strings.h>            // bzero
#include <arpa/inet.h>          // htonl inet_aton
#include <stdlib.h>             // atoi
#include <unordered_map>
#include <errno.h>
#include <iostream>
#include <unistd.h>             // close
#include <time.h>
#include <memory>

// typedef std::unordered_map<pthread_t, int> unordered_map;
typedef std::shared_ptr<MyThread> MyThreadPtr;
typedef std::unordered_map<int, MyThreadPtr> MapToThreadPtr;
int listenfd;
struct sockaddr_in serverAddr;
// unordered_map clientfdMap;
MapToThreadPtr clientThreadMap;
const int LISTENQ = 100;
const int BUFFERSIZE = 1024;


void TcpBind(const char* host, const uint16_t port)
{
  bzero(&serverAddr, sizeof(serverAddr));
  serverAddr.sin_family = AF_INET;
  if (host == NULL) {
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  }else {
    inet_aton(host, &serverAddr.sin_addr);
  }
  serverAddr.sin_port = htons(port);
  bind(listenfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
}

void ThreadCallFun(int myFd)
{
  pthread_t myID = pthread_self();
  pthread_detach(myID);
  // unordered_map::iterator iFind = clientfdMap.find(myID);
  // if (iFind == clientfdMap.end()) {
  //   return;
  // }
  // int myFd = iFind->second;
  char data[BUFFERSIZE];

  while (true) {
    int index = 0;
    int res;
    res = read(myFd, &data[index], BUFFERSIZE - index);
      if (res < 0 && errno != EWOULDBLOCK) {
        std::cout << "read error" << "\n";
        close(myFd);
        // clientfdMap.erase(myID);
        return;
      }else if (res == 0) {
        std::cout << "read eof" << "\n";
        close(myFd);
        break;
      }
      index += res;

    if (index) {
      write(myFd, data, index);
    }
  }

  clientThreadMap.erase(myFd);
  // std::cout << clientThreadMap.size()  << "\n";

  return;
}

int main(int argc, char *argv[])
{

  listenfd = socket(AF_INET, SOCK_STREAM, 0);

  if (argc == 2) {              // port
    TcpBind(NULL, static_cast<uint16_t>(atoi(argv[1])));
  }else if(argc == 3) {
    TcpBind(argv[1] , static_cast<uint16_t>(atoi(argv[2])));
  }else {
    std::cout << "host or port"  << "\n";
    return 0;
  }

  listen(listenfd, LISTENQ);
  while (true) {
    int clientfd = accept(listenfd, NULL, NULL);
    ThreadCall f = std::bind(&ThreadCallFun, clientfd);
    MyThreadPtr thread(new MyThread(f, "thread")); //MyThreadptr is RAII and will delete when left the scope
    thread->Start();
    clientThreadMap.insert(MapToThreadPtr::value_type(clientfd, thread));
    // clientfdMap.insert(unordered_map::value_type(thread->GetThreadID(), clientfd));
  }

  return 0;
}
