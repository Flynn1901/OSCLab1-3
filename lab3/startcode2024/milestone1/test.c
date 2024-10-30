#include <stdio.h>
#include <stdlib.h>
#include "dplist.h"

void* element_copy(void * element);
void element_free(void ** element);
int element_compare(void * x, void * y);

typedef struct {
	int id;
	char* name;
} my_element_t;


int main(void){

}

void * element_copy(void * element) {
	my_element_t* copy = malloc(sizeof (my_element_t));
	char* new_name;
	asprintf(&new_name,"%s",((my_element_t*)element)->name); //asprintf requires _GNU_SOURCE
	assert(copy != NULL);
	copy->id = ((my_element_t*)element)->id;
	copy->name = new_name;
	return (void *) copy;
}

element_free(void ** element) {
	free((((my_element_t*)*element))->name);
	free(*element);
	*element = NULL;
}
int element_compare(void * x, void * y) {
	return ((((my_element_t*)x)->id < ((my_element_t*)y)->id) ? -1 : (((my_element_t*)x)->id == ((my_element_t*)y)->id) ? 0 : 1);
}

