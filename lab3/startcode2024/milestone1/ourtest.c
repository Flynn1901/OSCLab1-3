#define _GNU_SOURCE
/**
 * \author Bert Lagaisse
 * 
 * main method that executes some test functions
 */

#include "dplist.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdarg.h>

typedef struct
{
    int id;
    char *name;
} my_element_t;

void *element_copy(void *element);
void element_free(void **element);
int element_compare(void *x, void *y);

char *test = "123";

void *element_copy(void *element)
{
    my_element_t *copy = malloc(sizeof(my_element_t));
    char *new_name;
    asprintf(&new_name, "%s", ((my_element_t *)element)->name);
    assert(copy != NULL);
    copy->id = ((my_element_t *)element)->id;
    copy->name = new_name;
    return (void *)copy;
}

void element_free(void **element)
{
    free((((my_element_t *)*element))->name);
    free(*element);
    *element = NULL;
}

int element_compare(void *x, void *y)
{
    return ((((my_element_t *)x)->id < ((my_element_t *)y)->id) ? -1 : (((my_element_t *)x)->id == ((my_element_t *)y)->id) ? 0
                                                                                                                            : 1);
}



void ck_assert_msg(bool result, char * msg){
    if(!result) printf("%s\n", msg);
}

void test1()
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


    my_element_t *content = (my_element_t *)malloc(sizeof(my_element_t));
    content->id = 5;
    content->name = "foo";

    list = dpl_create(element_copy, element_free, element_compare);
    dpl_insert_at_index(list, content, 0, false);
    dpl_free(&list, false);
    ck_assert_msg(list == NULL, "Failure: expected result to be NULL");

    list = dpl_create(element_copy, element_free, element_compare);
    dpl_insert_at_index(list, content, 0, true);
    dpl_free(&list, true);
    ck_assert_msg(list == NULL, "Failure: expected result to be NULL");


    char *test2 = "123";
    my_element_t *content2 = (my_element_t *)malloc(sizeof(my_element_t));
    content2->id = 6;
    content2->name = test2;
    list = dpl_create(element_copy, element_free, element_compare);

    dpl_insert_at_index(list, content2, 0, false);
    dpl_insert_at_index(list, content, 99, false);
    dpl_free(&list, false);
    ck_assert_msg(list == NULL, "Failure: expected result to be NULL");


    list = dpl_create(element_copy, element_free, element_compare);

    dpl_insert_at_index(list, content, 0, true);
    dpl_insert_at_index(list, content2, 99, true);
    dpl_free(&list, true);
    ck_assert_msg(list == NULL, "Failure: expected result to be NULL");

    free(content);
    free(content2);
}


void test2()
{
    my_element_t *content = (my_element_t *)malloc(sizeof(my_element_t));
    content->id = 10;
    content->name = "bar";

    dplist_t *list = NULL;

  
    list = dpl_insert_at_index(NULL, content, -1, false);
    ck_assert_msg(list == NULL, "Failure: expected list to be NULL");

    list = dpl_insert_at_index(NULL, content, 99, true);
    ck_assert_msg(list == NULL, "Failure: expected list to be NULL");

    free(content);
}


void test3()
{
    // Test inserting with deep copy false
    my_element_t *content1 = (my_element_t *)malloc(sizeof(my_element_t));
    content1->id = 11;
    content1->name = "bar";
    my_element_t *content2 = (my_element_t *)malloc(sizeof(my_element_t));
    content2->id = 12;
    content2->name = "quux";
    my_element_t *content3 = (my_element_t *)malloc(sizeof(my_element_t));
    content3->id = 13;
    content3->name = "gee";

    dplist_t *list = NULL;

    list = dpl_create(element_copy, element_free, element_compare);

    list = dpl_insert_at_index(list, content1, -1, false);

    list = dpl_insert_at_index(list, content3, 99, false);

    list = dpl_insert_at_index(list, content2, 1, false);

    int c1 = dpl_get_index_of_element(list, content1);
    int c2 = dpl_get_index_of_element(list, content2);
    int c3 = dpl_get_index_of_element(list, content3);



    ck_assert_msg(dpl_size(list) == 3, "Failure: expected list to have size of 3");

    dplist_node_t *node = (dplist_node_t *)dpl_get_reference_at_index(list, -1);
    my_element_t *el = (my_element_t *)dpl_get_element_at_reference(list, node);

    ck_assert_msg(node != NULL, "Failure: expected node at -1 is NULL");
    ck_assert_msg(el != NULL, "Failure: expected element at -1 is NULL");


    ck_assert_msg(element_compare(el, (void *)content1) == 0, "Failure: expected element at -1 is not content2");

    node = (dplist_node_t *)dpl_get_reference_at_index(list, 99);
    el = (my_element_t *)dpl_get_element_at_reference(list, node);

    ck_assert_msg(node != NULL, "Failure: expected node at 99 is NULL");
    ck_assert_msg(el != NULL, "Failure: expected element at 99 is NULL");


    ck_assert_msg(element_compare(el, (void *)content3) == 0, "Failure: expected element at 99 is not content3");

     list = dpl_remove_at_index(list, -2, false);

     ck_assert_msg(dpl_size(list) == 2, "Failure: expected list to have size of 2");

     list = dpl_remove_at_index(list, 98, false);

     ck_assert_msg(dpl_size(list) == 1, "Failure: expected list to have size of 1");

    dpl_free(&list, false);
    ck_assert_msg(list == NULL, "Failure: expected list to be NULL");



    // test insert with deep copy true;

    list = dpl_create(element_copy, element_free, element_compare);

    dpl_insert_at_index(list, content3, 99, true);
    dpl_insert_at_index(list, content1, -1, true);

    dpl_insert_at_index(list, content2, 1, true);

    ck_assert_msg(dpl_size(list) == 3, "Failure: expected list to have size of 3");

                

    list = dpl_remove_at_index(list, -2, true);

    ck_assert_msg(dpl_size(list) == 2, "Failure: expected list to have size of 2");

    list = dpl_remove_at_index(list, 98, true);

    ck_assert_msg(dpl_size(list) == 1, "Failure: expected list to have size of 1");

    dpl_free(&list, true);
    ck_assert_msg(list == NULL, "Failure: expected list to be NULL");

    free(content1);
    free(content2);
    free(content3);
}





int main(void)
{

    test1();
    test2();
    test3();

  
    return 0;
}
