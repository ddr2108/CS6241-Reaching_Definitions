#include <stdio.h>
#include <stdlib.h>
/*
int whee() {
int i = 0; 
return 2;
}*/
int asd;
int main() {
/*
int i;
int *k;
int j;
k = &i;
i =7;
j = 2+i;*/




int *k;
int i; 
int j;
int z = 4 + 5;
j = i+z;
i =8;
//i = 4;
j = i;
//k = &i;
//*k = 5;
j=i;
while(rand()%20==5){
	j+=i;
	j =5;
}
//z = i;
//printf("%d %d %d", i, j ,z);
j = 10;
  return 2;
}

//clang -S -O0 -emit-llvm hello.c; sh test.sh;vim hello.ll
//llvm-as hello.ll; lli hello.bc


