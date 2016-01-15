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

#define CONCAT_(a,b) a ## b
#define CONCAT(a,b) CONCAT_(a,b)
#define CALLME_BAD(f1, f2, a,b) CONCAT(f1,f2)(a,b)

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
  Project::RENAMED(42, 1);
  ::Project::RENAMED(0,0);
  {
    using namespace Project;
    using namespace Project::False;
    
    Project::RENAMED(42, 1);
    ::Project::RENAMED(0,0);
    RENAMED(42, 1);    
    
    namespace P = Project;
    P::RENAMED(42, 1);
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
  Function f = &::Project::RENAMED;
  f = &Project::RENAMED;
  {
    using namespace Project;
    using namespace Project::False;
    
    f = static_cast<void(*)(int,int)>(&RENAMED);   
    f = &RENAMED;
    f = RENAMED;  
    
    FalseFunction notf =
      static_cast<void(*)(Project::MyClass,Project::MyClass)>(&func1);   
    notf = &func1;
    notf = func1;    
  }

  // functional
  std::function<void(int,int)> f1(&Project::RENAMED);
  
  // in macros
  CALLME(::Project::RENAMED, 1, 2);
  CALLME(&::Project::RENAMED, 1, 2);
  //CALLME_BAD(::Project::, func1, 0, 1);
  CALLME_BAD(::Project::func, 1, 0, 1);

  // function parameter
  callme(&Project::RENAMED, 1, 2);
  callme(Project::RENAMED, 1, 2);
  CallMe(&Project::RENAMED, 1, 2);
  CallMe(Project::RENAMED, 1, 2);
  
  // template parameter
  CallMe<Project::RENAMED>(1, 2);
  CallMe<&Project::RENAMED>(1, 2);
  
  // unsafe renaming in template
  CallMeSafe<int>(0,0);
  CallMeUnsafe1(1, 2);
  CallMeUnsafe2(1, 2);
  CallMeUnsafe2(Project::MyClass(), Project::MyClass());  
  
  // lambdas
  auto lambda = [](){
    Project::RENAMED(1,2);
  };

  // false
	return func1(1, Project::MyClass());
}
