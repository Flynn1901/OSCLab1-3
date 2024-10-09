#include <stdio.h>
#include <string.h>

char First[10];
char Second[10];
char str[10];
char Name[20];
int  year;

void get_name(void);
void use_strcmp(void);
void convert_To_Upper(char*,char*);
void usestrcmp(void);
int strcmpnocase(char*,char*);
void concatenate(void);
void read_Year(void);
void conca2(void);
void sscanftry(void);
	
int main(void){
		get_name();
		convert_To_Upper(Second,str);	
		//usestrcmp();
		//strcmpnocase(Second,str);
		//concatenate();
		read_Year();
		conca2();
		sscanftry();
}

void sscanftry(void){
		char first[10];
		char second[10];
		int year;
		sscanf(Name,"%s %s %d",first,second,&year);
		printf("Your first name is: %s\nYour second name is: %s\nYour birth Year is: %d\n",first, second,year);
}

void conca2(void){
		strcpy(Name,First);
		size_t let = strlen(Name);
		snprintf(Name+let,sizeof(Name)-let," %s %d",Second,year);
		printf("%s",Name);
}


void read_Year(void){
		printf("Please enter your birth year like yyyy: ");
		scanf("%d",&year);
		printf("Your birth year is: %d\n",year);
}

void concatenate(void){
		strcpy(Name,First);
		strcat(Name,Second);

		printf("Your name is %s\n",Name);
}

void get_name(void){
		printf("Please enter your first name:");
		scanf("%s",First);
		printf("Please enter your second name:");
		scanf("%s",Second);

		printf("Now your name is %s, %s\n",First,Second);
}

void convert_To_Upper(char* src_name,char* dst_name){
		int i = 0;
		while(src_name[i]){
				dst_name[i] = src_name[i]-32;
				i += 1;
		}
		printf("After conversion, the Upper case string is: %s\n",dst_name);
}

int strcmpnocase(char* ch1, char* ch2){
//test the length of string. Not same will return -2
		if(sizeof(ch1)!=sizeof(ch2)){
				return -2;
		}

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
		printf("Two string is the same regardless of case.\n");
		return 0;
}

void usestrcmp(void){
		int result;

		result = strcmp(Second,str);

		switch(result){
		case 0:
				printf("Two string is the same");
		default:
				printf("Two string is different");
		}
}


