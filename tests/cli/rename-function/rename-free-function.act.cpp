// RUN: %s -rename-function 'main,func' %s -- -I/usr/lib/clang/3.6/include
#include <cstdio>
#include <cstdlib>

int func(int argc, char** argv) // CHECK-LINE: int func(int argc, char** argv)
{
  puts("Hello World!");

	return EXIT_SUCCESS;;
}
