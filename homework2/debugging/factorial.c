#include <stdio.h>

int factorial(int number);

int main(void){
	printf("Calculating 5!...\n ");
	int result = factorial(5);
	printf("5! = %d\n",result);
	return 0;
}

int factorial(int number){
	int current = number;
	int result = 1;
	while(current >0){
		result *= number;
		number--;
	}
	return result;
}
