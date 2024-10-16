#include <stdio.h>

void swap_pointers(int**,int**);

void swap_pointers(int** p1,int** p2){
		int* m = *p1;
		*p1 = *p2;
		*p2 = m;
		printf("address of m  = %p\n",m);
		printf("address of p = %p and q = %p\n",p1,p2);
		//printf("**p1 is:%d,*p1 is:%p,p1 is:%p\n",**p1,*p1,p1); 
}

int main(void){
		int a = 1;
		int b = 2;
		
		int *p = &a;
		int *q = &b;
		printf("address of p = %p and q = %p\n",p,q);

		swap_pointers(&p,&q);

		printf("address of p %p and q = %p\n",p,q);
}

