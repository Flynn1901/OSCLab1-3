#define _GNU_SOURCE


#include "dplist.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>
typedef struct {
    int id;
    char* name;
} my_element_t;

void* element_copy(void * element);
void element_free(void ** element);
int element_compare(void * x, void * y);
// void dpl_cleanup_rand(list);

void * element_copy(void * element) {
    my_element_t* copy = malloc(sizeof (my_element_t));
    char* new_name;
    asprintf(&new_name,"%s",((my_element_t*)element)->name); //asprintf requires _GNU_SOURCE
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

void ck_assert_msg(bool result, char * msg){
    if(!result) printf("%s\n", msg);
}

// void dpl_cleanup_rand(dplist_t * list) {
//     int size;
//     srand(time(NULL));
//     do {
//         int index;
//         dplist_node_t * dummy;
//         int * element;
//         size = dpl_size(list);
//         index = rand() % (size + 2) - 1;
//         dummy = dpl_get_reference_at_index(list, index);
//         element = dpl_get_element_at_reference(list, dummy);
//          list = dpl_remove_at_index(list, index, true);
// 	 } while (size > 0);
//  }

void yourtest1()
    {
        // Test free NULL, don't use callback
        dplist_t *list = NULL;
        dpl_free(&list, false);
        ck_assert_msg(list == NULL, "Failure: expected result to be NULL");

        // Test free NULL, use callback
        list = NULL;
        dpl_free(&list, true);
        ck_assert_msg(list == NULL, "Failure: expected result to be NULL");

        // Test free empty list, don't use callback
        list = dpl_create(element_copy, element_free, element_compare);
        dpl_free(&list, false);
        ck_assert_msg(list == NULL, "Failure: expected result to be NULL");

        // Test free empty list, use callback
        list = dpl_create(element_copy, element_free, element_compare);
        dpl_free(&list, true);
        ck_assert_msg(list == NULL, "Failure: expected result to be NULL");

        // TODO : your test scenarios
    list = dpl_create(element_copy, element_free, element_compare);
		my_element_t x;
		char* name;
		asprintf(&name,"Flynn");
		x.name = name;


		printf("Adding 2 elements to the list\n");
		x.id = 1;
		list = dpl_insert_at_index(list, &x, -10, true);
		
		x.id = 2;
		list = dpl_insert_at_index(list,&x,1,true);

        x.id = 3;
        list = dpl_insert_at_index(list,&x,10,true);

    printf("Free all list");

    dpl_remove_at_index(list,0,true);
    dpl_remove_at_index(list,2,true);
    dpl_remove_at_index(list,1,true);

    }




int main(void) {
    int i=1;

    yourtest1();
    return 0;
}
