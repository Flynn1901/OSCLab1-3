

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "dplist.h"




/*
 * The real definition of struct list / struct node
 */

struct dplist_node {
    dplist_node_t *prev, *next;
    void *element;
};

struct dplist {
    dplist_node_t *head;

    void *(*element_copy)(void *src_element);

    void (*element_free)(void **element);

    int (*element_compare)(void *x, void *y);
};


dplist_t *dpl_create(// callback functions
        void *(*element_copy)(void *src_element),
        void (*element_free)(void **element),
        int (*element_compare)(void *x, void *y)
) {
    dplist_t *list;
    list = malloc(sizeof(struct dplist));
    list->head = NULL;
    list->element_copy = element_copy;
    list->element_free = element_free;
    list->element_compare = element_compare;
    return list;
}

void dpl_free(dplist_t **list, bool free_element) {
		int list_size = dpl_size(*list);
		while(list_size--){
				dpl_remove_at_index(*list,list_size,free_element);
		}
		free(*list);
		*list = NULL;
}

dplist_t *dpl_insert_at_index(dplist_t *list, void *element, int index, bool insert_copy) {
		dplist_node_t *list_node = malloc(sizeof(dplist_node_t));			
		if(insert_copy){
				list_node->element = list->element_copy(element);
				//void* element1 = list->element_copy(element);
			}
		else{
				list_node->element = element;
			}
		if(list==NULL){return NULL;}
		else if(index<=0){
				int size = dpl_size(list);
				if(size==0){
						list_node->next = NULL;
				}
				else{
						
						list_node->next = list->head;
						list->head->prev = list_node;
				}
				list_node->prev = NULL;
				list->head = list_node;
		}
		else if(index>=dpl_size(list)-1){
				int size = dpl_size(list);
				if(size==0){
						list_node->prev = NULL;
						list->head = list_node;
				}
				else{
						dplist_node_t* last_node = dpl_get_reference_at_index(list,size-1);
						list_node->prev = last_node;
						last_node->next = list_node;
				}
				list_node->next = NULL;
		}
		else{
				dplist_node_t *next_node = dpl_get_reference_at_index(list,index);
				dplist_node_t *prev_node = next_node->prev;
				next_node->prev = list_node;
				prev_node->next = list_node;
				list_node->next = next_node;
				list_node->prev = prev_node;
		}
		return list;
}

dplist_t *dpl_remove_at_index(dplist_t *list, int index, bool free_element) {
		if(list==NULL){ return NULL;}
		else if(list->head==NULL){return list;}
		else{
				int list_size = dpl_size(list);
				if(index<=0){
						if(free_element){ list->element_free(&(list->head->element)); }
						else{
							if(list->head->next!=NULL){
									dplist_node_t *next_node = list->head->next;
									next_node->prev = NULL;
							}
							else{
									free(list->head);
									list->head = NULL;
							}
						}
				}
				else if(index>=list_size-1){
						dplist_node_t *last_node = dpl_get_reference_at_index(list,list_size-1);
						if(free_element){ list->element_free(&last_node->element); }
						else{
								if(last_node->prev!=NULL){
										dplist_node_t *prev_node = last_node->prev;
										prev_node->next = NULL;
										free(last_node);
								}
								else{
										free(last_node);
										list->head = NULL;
								}
						}

				}
				else{
						dplist_node_t *target_node = dpl_get_reference_at_index(list,index);
						if(free_element){ list->element_free(&target_node->element); }
						else{
							dplist_node_t *prev_node = target_node->prev;
							dplist_node_t *next_node = target_node->next;	
							prev_node->next = next_node;
							next_node->prev = prev_node;
							free(target_node);
						}						
				}
}
}

int dpl_size(dplist_t *list) {
		if(list==NULL){
				return -1;
		}
		else if(list->head==NULL){
				return 0;
		}
		else{
				int count = 1;
				dplist_node_t *next_node = list->head;
				while(next_node->next!=NULL){
						count++;
						next_node = next_node->next;
				}
				return count;
		}
}

void *dpl_get_element_at_index(dplist_t *list, int index) {
		return dpl_get_reference_at_index(list,index)->element;
}

int dpl_get_index_of_element(dplist_t *list, void *element) {
		if(list==NULL||list->head==NULL){return -1;}
		else{
				dplist_node_t *target_node = list->head;
				int compare_result;
				int index = 0;
				while(target_node->next!=NULL){
						compare_result = list->element_compare(target_node->element,element);
						if(compare_result==0){
								return index;
								break;
						}
						index++;
						target_node = target_node->next;
				}
				return -1;
}
}

dplist_node_t *dpl_get_reference_at_index(dplist_t *list, int index) {
		dplist_node_t *next_node = list->head;
		dplist_node_t *target_node = NULL;

		if(list==NULL||list->head==NULL){return NULL;}
		else{
				if(index<=0){target_node = list->head;}
				else if(index>=(dpl_size(list)-1)){
						while(next_node->next!=NULL){
								next_node = next_node->next;
							}
						target_node = next_node;
				}
				else{
						while(1){
								next_node = next_node->next;
								index--;
								if(index==0){break;}									
							}
						target_node = next_node;
				}
				return target_node;
		}
    //TODO: add your code here
}

void *dpl_get_element_at_reference(dplist_t *list, dplist_node_t *reference) {
		if(list==NULL||list->head==NULL){return NULL;}
		if(reference==NULL){ return NULL;}	
		else{
				dplist_node_t *next_node = list->head;
				while(next_node->next!=NULL){
						if(next_node==reference){
								return next_node->element;
						}
						next_node = next_node->next;
				}
				return NULL;
			}
}


