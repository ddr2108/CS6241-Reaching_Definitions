#include <stdio.h>
#include <stdlib.h>

int main() {

	int i;
	int k;
	int j = 2+9;

	j = k;
	j = i;
	k = 5;
	j = k;
	k = i;
	i = 3;
	j = i;

  	return 2;
}

//clang -S -O0 -emit-llvm hello.c; sh test.sh;vim hello.ll
//llvm-as hello.ll; lli hello.bc


