#include <stdio.h>
#include "dplist.h"

int main(void){
		dplist_t *numbers = NULL;
		dplist_node_t *node = NULL;
		numbers = dpl_create();

		printf("Current size of numbers is: %d\n",dpl_size(numbers));
		dpl_insert_at_index(numbers, 'a', 0);
		dpl_insert_at_index(numbers, 'b', 1);
		dpl_insert_at_index(numbers,'c',0);
		showall(numbers);
		printf("Current size of numbers is: %d\n",dpl_size(numbers));	
		//dpl_remove_at_index(numbers,2);
		dpl_free(&numbers);
		showall(numbers);
		printf("Element of index 2 is: %c\n",dpl_get_element_at_index(numbers,2));
		//dpl_insert_at_index(numbers,'d',3);
	//	dpl_remove_at_index(numbers,-1);
		//printf("Current size of numbers is: %d\n",dpl_size(numbers));
		//node = dpl_get_reference_at_index(numbers,1);
		//node->element;
		//printf("%p\n", node);
		//test(numbers,2);
		
}
