#pragma once

void func1(int a, int b);

namespace Project {

class MyClass {
public:
  void func1(int a, int b)
  { }

  void func2(int a, int b);
};

void func1(int a, int b);
void func1(MyClass a, MyClass b) { }

namespace Extra {

void func1(int a, int b);

void
func2
  (
   int a,
   int b
  );

}

}
