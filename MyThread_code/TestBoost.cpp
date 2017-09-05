// g++ TestBoost.cpp -o Test

#include <iostream>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>

class Test : boost::noncopyable
{
public:
  void ClassPrint()
  {
    std::cout << "class print!" << "\n";
  }

  void ClassPrintNum(int a, int b)
  {
    std::cout << "Class Print " << a << " " << b << "\n";
  }

  int ReturnInt(int a)
  {
    return a;
  }
};

void FunctionPrint()
{
  std::cout << "function print" << "\n";
}

void FunctionPrintNum(int a, int b)
{
  std::cout << "Function Print" << b << " " << a << "\n";
}

using boost::function;
using boost::bind;



int main(int argc, char *argv[])
{
  function<void()> f;
  Test test;

  f = bind(&Test::ClassPrint, &test);
  f();

  function<int()> g;
  g = bind(&Test::ReturnInt, &test, 10);
  std::cout << g() << "\n";

  function<void(int, int)> k;
  k = bind(&Test::ClassPrintNum, &test, _1, _2);
  k(3, 4);

  f = bind(&FunctionPrint);
  f();
  function<void(int)> r;
  r = bind(&FunctionPrintNum, _1, 5);
  r(10);

  r = bind(&FunctionPrintNum, 2, _1);
  r(10);

  return 0;
}
