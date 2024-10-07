#include <stdio.h>
#include <string.h>

int strcmpnocase(char*,char*);

int main(void){
	int MAX = 10;
	char first[MAX];
	char second[MAX];
	char name[MAX];
	char str[MAX];
	
	printf("First question.\n");
	printf("Please enter your first name:\n");
	int first_return  = scanf("%s",first);
	if(first_return == 1){
			printf("Your first name is: %s\n",first);
	}

	printf("Please enter your second name:\n");
	int second_return = scanf("%s",second);
	if(second_return == 1){
			printf("your second name is: %s\n",second);
	}

	if((second_return & first_return) == 1){
			printf("Your total name is: %s %s\n",first, second);
	}

	printf("Second question\n");
	int i = 0;
	while(second[i]){
			str[i] = second[i]-32;
			i += 1;
	}
	printf("Str is: %s\n",str);

	printf("Third question\n");
	int comp_result = strcmpnocase(second, str);
	printf("Comp_result is:%d \n",comp_result);
	if(comp_result == 0){
			printf("Two string are the same\n");
	}
	
	printf("Question for cantenate string.\n");
	
}

int strcmpnocase(char* ch1, char* ch2){
//test the length of string. Not same will return -2
		if(sizeof(ch1)!=sizeof(ch2)){
				return -2;
		}

		int size = sizeof(ch1);
		char new_ch1[10] = {0};
		char new_ch2[10] = {0};

		int i = 0;
//Convert to lower case
		while(ch1[i]){
				if(ch1[i]<91){
						new_ch1[i] = ch1[i]+32;
				}
				else{
				new_ch1[i] = ch1[i];
				}
		i++;
		}
			
		i = 0;
		while(ch2[i]){
				if(ch2[i]<91){
						new_ch2[i] = ch2[i]+32;
				}
				else{
				new_ch2[i] = ch2[i];
				}
		i++;
		}
//Test whether two strings are the same. Same will return 0. Not same will return -1
		i = 0;
		while(new_ch1[i]){
				if(new_ch1[i]!=new_ch2[i]){
						return -1;
				}
		i++;
		}
//Same will return 0
		return 0;
}
