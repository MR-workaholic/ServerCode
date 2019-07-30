// g++ TestBoost.cpp -o Test

#include <iostream>
// #include <boost/function.hpp>
#include <functional>
// #include <boost/bind.hpp>
// #include <boost/noncopyable.hpp>

class Noncopyable {

protected:
  Noncopyable(){}
  ~Noncopyable(){}
private:
  Noncopyable(const Noncopyable&);
  Noncopyable& operator=(const Noncopyable&);

};

class Test : private Noncopyable
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

using std::function;
using std::bind;
using std::placeholders::_1;
using std::placeholders::_2;


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
