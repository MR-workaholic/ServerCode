#include "Method.h"
#include "MyServer.h"

int main(int argc, char *argv[])
{
  Method method;
  MyServer server(&method);

  server.Init();
  server._loop.run(0);

  return 0;
}
