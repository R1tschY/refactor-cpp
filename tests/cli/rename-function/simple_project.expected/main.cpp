#include <cstdio>
#include <cstdlib>

#include "project.h"

namespace {

int func1(int a, Project::MyClass b)
{
  puts("Hello World!");
  return a;
}

}

int main(int argc, char** argv)
{
  Project::RENAMED(42, 1);
  Project::Extra::func1(1,2);
  Project::Extra::func2(45,45);
  func1(1,2);

	return func1(1, Project::MyClass());
}
