#include "project.h"

void func1(int a, int b)
{

}

namespace Project {

void MyClass::func2(int a, int b)
{
  func1(1, 4);
}

void func1(int a, int b) { }

namespace Extra {

void func1(int a, int b) { }

void func2(int a, int b) { }

}

}
