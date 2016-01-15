#include <cstdio>
#include <cstdlib>
#include <utility>
#include <functional>

namespace Project {

template<typename T>
int func(int a, int b)
{ return a + b + sizeof(T); }

}

using Function = int(*)(int,int);

#define CALLME(func, a, b) (*func)((a), (b))
#define CALLMEUNSAFE(a, b) Project::func<int>((a), (b))

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
  Project::func<T>(a, b + sizeof(T));
}

template<typename...Args>
void CallMeUnsafe1(Args&&...args)
{
  Project::func<int>(std::forward<Args>(args)...);
}

template<typename...Args>
void CallMeUnsafe2(Args&&...args)
{
  using namespace Project;

  func<int>(std::forward<Args>(args)...);
}

int main(int argc, char** argv)
{
  // variants
  Project::func<int>(42, 1);
  ::Project::func<int>(0,0);
  {
    using namespace Project;
    
    Project::func<int>(42, 1);
    ::Project::func<int>(0,0);
    func<int>(42, 1);
    
    namespace P = Project;
    P::func<int>(42, 1);
  }

  // function pointer
  Function f = &::Project::func<int>;
  f = &Project::func<int>;
  {
    using namespace Project;
    
    f = static_cast<int(*)(int,int)>(&func<int>);   
    f = &func<int>;
    f = func<int>;
  }

  // functional
  std::function<void(int,int)> f1(&Project::func<int>);
  
  // in macros
  CALLME(::Project::func<int>, 1, 2);
  CALLME(&::Project::func<int>, 1, 2);
  CALLMEUNSAFE(1,2);

  // function parameter
  callme(&Project::func<int>, 1, 2);
  callme(Project::func<int>, 1, 2);
  CallMe(&Project::func<int>, 1, 2);
  CallMe(Project::func<int>, 1, 2);
  
  // template parameter
  CallMe<Project::func<int> >(1, 2);
  CallMe<&Project::func<int> >(1, 2);
  
  // unsafe renaming in template
  CallMeSafe<int>(0,0);
  CallMeUnsafe1(1, 2);
  CallMeUnsafe2(1, 2);
  
  // lambdas
  auto lambda = [](){
    return Project::func<int>(1,2);
  };

	return lambda();
}
