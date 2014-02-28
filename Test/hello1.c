#include <stdio.h>
#include <stdlib.h>
/*
int whee() {
int i = 0; 
return 2;
}*/
int asd;
int main() {

int i = 3+5;
int k;
int j = 2+9;
//k = &i;
k = 2+i;

//i =8;
//i = 4;
//j = 10;
//k = &i;
//*k = 5;
//j=i;
while(rand()%20==5){
	j+=i;
	if (j>25){
	  j = 15;
	  //break;
	}else{
		i = 3;
	   j = 9;
	}
	i = 3;
	j =5;
}
//z = i;
//printf("%d %d %d",i, j ,k);
i = 1;
j = 10;
i = 8;
  return 2;
}

//clang -S -O0 -emit-llvm hello.c; sh test.sh;vim hello.ll
//llvm-as hello.ll; lli hello.bc


