/**
 * \author Jeroen Van Aken, Bert Lagaisse, Ludo Bruynseels
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include "dplist.h"



/*
 * The real definition of struct list / struct node
 */
struct dplist_node {
    dplist_node_t *prev, *next;
    element_t element;
};

struct dplist {
    dplist_node_t *head;
    // more fields will be added later
};

dplist_t *dpl_create() {
    dplist_t *list;
    list = malloc(sizeof(struct dplist));
    list->head = NULL;
  return list;
}

void dpl_free(dplist_t **list) {
		dplist_t *target_list = *list;
		if(target_list==NULL){
		}
		else if(target_list->head==NULL){

		}
		else{
			dplist_node_t *current_node = target_list->head;
			dplist_node_t *next_node = NULL;
			int max_index = dpl_size(target_list);
			while(max_index--){
					next_node = current_node->next;
					free(current_node);
					current_node = next_node;
			}
			free(target_list);
			*list=NULL;
		}
    //TODO: add your code here
    //Do extensive testing with valgrind. 

}

/* Important note: to implement any list manipulation operator (insert, append, delete, sort, ...), always be aware of the following cases:
 * 1. empty list ==> avoid errors
 * 2. do operation at the start of the list ==> typically requires some special pointer manipulation
 * 3. do operation at the end of the list ==> typically requires some special pointer manipulation
 * 4. do operation in the middle of the list ==> default case with default pointer manipulation
 * ALWAYS check that you implementation works correctly in all these cases (check this on paper with list representation drawings!)
 **/


dplist_t *dpl_insert_at_index(dplist_t *list, element_t element, int index) {
    dplist_node_t *ref_at_index, *list_node;
    if (list == NULL) return NULL;

    list_node = malloc(sizeof(dplist_node_t));

    list_node->element = element;
    // pointer drawing breakpoint
    if (list->head == NULL) { // covers case 1
        list_node->prev = NULL;
        list_node->next = NULL;
        list->head = list_node;
        // pointer drawing breakpoint
    } else if (index <= 0) { // covers case 2
        list_node->prev = NULL;
        list_node->next = list->head;
        list->head->prev = list_node;
        list->head = list_node;
        // pointer drawing breakpoint
    } else {
        ref_at_index = dpl_get_reference_at_index(list, index);
        assert(ref_at_index != NULL);
        // pointer drawing breakpoint
        if (index < dpl_size(list)) { // covers case 4
            list_node->prev = ref_at_index->prev;
            list_node->next = ref_at_index;
            ref_at_index->prev->next = list_node;
            ref_at_index->prev = list_node;
            // pointer drawing breakpoint
        } else { // covers case 3
            assert(ref_at_index->next == NULL);
            list_node->next = NULL;
            list_node->prev = ref_at_index;
            ref_at_index->next = list_node;
            // pointer drawing breakpoint
        }
    }
    return list;
}

dplist_t *dpl_remove_at_index(dplist_t *list, int index) {
	int	max_index = dpl_size(list)-1;
    if(list==NULL){
			return NULL;
	}
	else if(list->head==NULL){
			return list;
	}
	else if(max_index==0){
			free(list->head);
			list->head = NULL;
			return list;
	}
	else if(index<=0){
			dplist_node_t *next_node = list->head->next;
			list->head->next->prev = NULL;
			free(list->head);
			list->head = next_node;
			return list;
	}
	else if(index>=dpl_size(list)-1){
			dplist_node_t *last_node = dpl_get_reference_at_index(list,max_index);
			last_node->prev->next = NULL;
			free(last_node);
			return list;
	}
	else{
			dplist_node_t *target_node = dpl_get_reference_at_index(list,index);
			target_node->prev->next = target_node->next;
			target_node->next->prev = target_node->prev;
			free(target_node);
			return list;
	}
    return NULL;
}

int dpl_size(dplist_t *list) {
	if(list == NULL) return -1;

	dplist_node_t *next_node = (list->head);
	if(next_node==NULL){return 0;}

	int size = 1;
	bool whether_last_one = false;

	while(!whether_last_one){
			if(next_node->next==NULL){
					whether_last_one = true;
			}else{
					whether_last_one = false;
					next_node = (next_node->next);
					size++;
			}
	}	
    return size;
}

dplist_node_t *dpl_get_reference_at_index(dplist_t *list, int index) {
	if(list==NULL||list->head==NULL){return NULL;}

	dplist_node_t *dummy = NULL;
	
	int max_index = dpl_size(list) - 1;
	if(index<=0||max_index==0){
			dummy = list->head;
	}
	else if(index>max_index){
			//printf("Your target index is: %d. The maximum index is %d.\n Exceed the maximum index!\n",index,max_index);
			dplist_node_t *next_node = list->head;
			for(int i=0;i<=max_index-1;i++){
					next_node = next_node->next;
			}
			dummy = next_node;
	}
	else{
		dplist_node_t *next_node = list->head;

		for(int j=0;j<=index-1;j++){
				next_node = next_node->next;
		}
		dummy = next_node;
		printf("%p\n", dummy);
		printf("%c\n", dummy->element);
}
    return dummy;
}

void test(dplist_t *list,int index){
		dplist_node_t *node = dpl_get_reference_at_index(list,index);
		printf("Current node is:%c",node->element);
}


element_t dpl_get_element_at_index(dplist_t *list, int index) {
		dplist_node_t *target_node = dpl_get_reference_at_index(list,index);
		if(target_node==NULL){
				return NULL;
		}
		return target_node->element;
}

int dpl_get_index_of_element(dplist_t *list, element_t element) {

    //TODO: add your code here
    return -1;
}


void showall(dplist_t *list){
		if(list==NULL){
				printf("Empty list pointer");
		}
		else if(list->head==NULL){
				printf("Empty list");
		}
		else{
				dplist_node_t *current_node = list->head;
				while(1){
						printf("%c\n",current_node->element);
						if(current_node->next!=NULL){
								current_node = current_node->next;
						}
						else{
								break;
						}
				}
		}
}
						

