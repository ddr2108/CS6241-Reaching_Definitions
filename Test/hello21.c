#include <stdio.h>
#include <stdlib.h>

int main() {

	int i = 3+5;
	int j = 2+9;
	int k;
	int l;

	l = i + j;
	k = j + l;
 
	printf("%d %d %d %d\n", i, j, k , l);
  return 2;
}

//clang -S -O0 -emit-llvm hello.c; sh test.sh;vim hello.ll
//llvm-as hello.ll; lli hello.bc


