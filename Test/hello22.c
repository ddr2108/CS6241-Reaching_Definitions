#include <stdio.h>
#include <stdlib.h>

int main() {

	int i = 3+5;
	int k;
	int l;
	int j = 2+9;

	l = 7;
	k = 10;
	j = 5;
	k = i; 

	printf("%d\n", k);
  return 2;
}

//clang -S -O0 -emit-llvm hello.c; sh test.sh;vim hello.ll
//llvm-as hello.ll; lli hello.bc


