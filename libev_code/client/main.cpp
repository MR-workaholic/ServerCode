#include "MyClient.h"
#include "Method.h"

int main(int argc, char *argv[])
{
  Method method;
  MyClient client(&method, "127.0.0.1", "54321");
  client.Init();
  client._loop.run(0);
  client.Close();

  return 0;
}
