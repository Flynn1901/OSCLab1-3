#include <stdio.h>

int main(void){
	int integer = 0;
	float float_num = 0;
	double double_num = 0.0;
	int * p = &integer;
	printf("Memory size of int is:%lu\n",sizeof(integer));
	printf("Memory size of float is:%lu\n",sizeof(float_num));
	printf("Memory size of double is:%lu\n",sizeof(double_num));
	printf("Memory size of pointer is:%lu\n",sizeof(p));
	printf("Memory size of void is%lu\n",sizeof(void*));			
	return 0;
}
