#include <cstdio>
#include <cstdlib>
#include <functional>

#include "project.h"

namespace {

int func1(int a, Project::MyClass b)
{
  puts("Hello World!");
  return a;
}

}

using Function = void(*)(int,int);
using FalseFunction = void(*)(Project::MyClass,Project::MyClass);

#define CALLME(func, a, b) (*func)((a), (b))

void callme(Function f, int a, int b)
{
  f(a, b);
}

template<Function T>
void CallMe(int a, int b)
{
  T(a, b);
}

template<typename Func, typename...Args>
void CallMe(Func f, Args&&...args)
{
  f(std::forward<Args>(args)...);
}

template<typename T>
void CallMeSafe(int a, int b)
{
  Project::func1(a, b + sizeof(T));
}

template<typename...Args>
void CallMeUnsafe1(Args&&...args)
{
  Project::func1(std::forward<Args>(args)...);
}

template<typename...Args>
void CallMeUnsafe2(Args&&...args)
{
  using namespace Project;
  using namespace Project::False;

  func1(std::forward<Args>(args)...);
}

int main(int argc, char** argv)
{
  // variants
  Project::func1(42, 1);
  ::Project::func1(0,0);
  {
    using namespace Project;
    using namespace Project::False;
    
    Project::func1(42, 1);
    ::Project::func1(0,0);
    func1(42, 1);
    
    namespace P = Project;
    P::func1(42, 1);
  }

  // false
  Project::Extra::func1(1,2);
  Project::Extra::func2(45,45);
  {
    using namespace Project;
    using namespace Project::False;
    
    func1(Project::MyClass(), Project::MyClass());
  }

  // function pointer
  Function f = &::Project::func1;
  f = &Project::func1;
  {
    using namespace Project;
    using namespace Project::False;
    
    f = static_cast<void(*)(int,int)>(&func1);   
    f = &func1;
    f = func1;  
    
    FalseFunction notf =
      static_cast<void(*)(Project::MyClass,Project::MyClass)>(&func1);   
    notf = &func1;
    notf = func1;    
  }

  // functional
  std::function<void(int,int)> f1(&Project::func1);
  
  // in macros
  CALLME(::Project::func1, 1, 2);
  CALLME(&::Project::func1, 1, 2);

  // function parameter
  callme(&Project::func1, 1, 2);
  callme(Project::func1, 1, 2);
  CallMe(&Project::func1, 1, 2);
  CallMe(Project::func1, 1, 2);
  
  // template parameter
  CallMe<Project::func1>(1, 2);
  CallMe<&Project::func1>(1, 2);
  
  // unsafe renaming in template
  CallMeSafe<int>(0,0);
  CallMeUnsafe1(1, 2);
  CallMeUnsafe2(1, 2);
  CallMeUnsafe2(Project::MyClass(), Project::MyClass());  
  
  // lambdas
  auto lambda = [](){
    Project::func1(1,2);
  };

  // false
	return func1(1, Project::MyClass());
}
