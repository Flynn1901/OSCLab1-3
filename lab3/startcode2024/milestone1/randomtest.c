#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "dplist.h"
#include "time.h"

dplist_t * list = NULL;

int dplist_errno;

void* element_copy(void * element);
void element_free(void ** element);
int element_compare(void * x, void * y);

void dpl_cleanup_rand(dplist_t * list);

typedef struct {
    int id;
    char* name;
} my_element_t;

int main(void) {
    my_element_t x;
    char* name;
    asprintf(&name,"Jeroen");
    x.name = name;

    list = dpl_create(element_copy, element_free, element_compare);

    printf("Adding 3 elements to the list\n");
    x.id = 1;
    list = dpl_insert_at_index(list, &x, 0, true);

    x.id = 3;
    list = dpl_insert_at_index(list, &x, 1, true);

    x.id = 7;
    list = dpl_insert_at_index(list, &x, 5, true);

    printf("list size = %d\n", dpl_size(list));

    printf("Adding one extra element\n");
    x.id = 5;
    list = dpl_insert_at_index(list, &x, 2, true);



    printf("Removing all elements from the list\n");
    dpl_cleanup_rand(list);

    printf("Free the list list\n");
    dpl_free(&list,false);

    free(name);
    return 0;
}


void * element_copy(void * element) {
    my_element_t* copy = malloc(sizeof (my_element_t));
    char* new_name;
    asprintf(&new_name,"%s",((my_element_t*)element)->name);
    assert(copy != NULL);
    copy->id = ((my_element_t*)element)->id;
    copy->name = new_name;
    return (void *) copy;
}

void element_free(void ** element) {
    free((((my_element_t*)*element))->name);
    free(*element);
    *element = NULL;
}

int element_compare(void * x, void * y) {
    return ((((my_element_t*)x)->id < ((my_element_t*)y)->id) ? -1 : (((my_element_t*)x)->id == ((my_element_t*)y)->id) ? 0 : 1);
}

void dpl_cleanup_rand(dplist_t * list) {
    int size;
    srand(time(NULL));

    do {
        int index;
        dplist_node_t * dummy;
        int * element;
        size = dpl_size(list);
        index = rand() % (size + 2) - 1;
        dummy = dpl_get_reference_at_index(list, index);
        element = dpl_get_element_at_reference(list, dummy);
        list = dpl_remove_at_index(list, index, true);
    } while (size > 0);
}
