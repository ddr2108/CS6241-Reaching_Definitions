#include <stdio.h>
#include <stdlib.h>
/*
int whee() {
int i = 0; 
return 2;
}*/
int asd;
int main() {
int *k;
int i =3+4; 
int j;
int z = 4 + 5;
j = i+z;
i =8;
//i = 4;
j = i;
k = &i;
*k = 5;
j=i;
//z = i;
printf("%d %d %d %d", i, j , k ,z);
/*
z = 5;

k = 2 + 4;
i =k;
j = &i;
j = 6;*/
  return 2;
}

//clang -S -O0 -emit-llvm hello.c; sh test.sh;vim hello.ll
//llvm-as hello.ll; lli hello.bc


