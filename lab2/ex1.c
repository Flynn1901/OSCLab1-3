#include <stdio.h>
#include <stdlib.h>

typedef struct {
		short day, month;
		unsigned year;
} date_t;

//function declaration
//void date_struct(int day, int month, int year,date_t*);
/*void date_struct(int day, int month, int year,date_t* d){
		d->day = (short)day;
		d->month = (short)month;
		d->year = (unsigned)year;
}*/

date_t* date_struct(int day, int month, int year){
		date_t* d = malloc(3*sizeof(int));
		d->day = (short)day;
		d->month = (short)month;
		d->year = (unsigned)year;
		return d;
}

int main(void){
		int day, month, year;
		date_t* d;
		printf("\nGive day, month, year:");
		scanf("%d %d %d",&day, &month,&year);
		d = date_struct(day, month, year);
		printf("\ndate struct values: %d-%d-%d",d->day,d->month,d->year);
		return 0;
}
