// cmd: refactor-cpp rename-function func function
// cmd: refactor-cpp rename-function ::func function

#include <cstdio>
#include <cstdlib>

static int function(int a, int b);

static int function(int a, int b)
{
  puts("Hello World!");
  return a + b;
}

int main(int argc, char** argv) {	
	return function(40, 2);
}
