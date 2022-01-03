/**source: list.c
 * provides implementation of list ADT
 */

#include <stdio.h>
#include <stdlib.h>
#include "list.h"

typedef struct {
	int end;
	int size;
	int *nums;
} Priv_IntList;

/**returns a pointer to an IntList with the given initial size
 * negative sizes may cause errors when adding elements
 */
IntList *initialize(int size) {
	return intList(size);
}

/**creates an IntList and returns the address to the object
 */
IntList *intList(int size) {
	Priv_IntList *lst = malloc(sizeof(Priv_IntList));
	lst->end = 0;
	lst->size = size;
	lst->nums = calloc(size, sizeof(int));
	return (IntList)lst;
}

/**adds an element to the given IntList
 * if the current IntList is full, add will try to approximately double the previous size
 */
void add(IntList *l, int x) {
	Priv_IntList *lst = (Priv_IntList *)l;
	if(lst->size == lst->end + 1) {
		int *ptr = realloc(lst->nums, (lst->size + 1) * sizeof(int) * 2);
		if(ptr == NULL) {
			// Failed to reallocate memory block with twice the previous size
			fprintf(stderr, "IntList: failed to reallocate larger memory block");
			return;
		} else {
			lst->nums = ptr;
			lst->size *= 2;
		}
	}
	
	lst->nums[lst->end] = x;
	lst->end++;
}

/**removes the last element in the array of the given IntList
 */
void removeLast(IntList *l) {
	Priv_IntList *lst = (Priv_IntList *)l;

	if(lst->end > 0) {
		lst->nums[lst->end] = 0;
		lst->end--;
	}
}

/**checks if the given IntList has the given value
 */
int check(IntList *l, int x) {
	return contains(l, x);
}

/**returns 1 if the given IntList contains the given x value
 * and 0 otherwise
 */
int contains(IntList *l, int x) {
	Priv_IntList *lst = (Priv_IntList *)l;
	int found = 0;
	
	for(int i = 0; i < lst->end; i++) {
		if(lst->nums[i] == x) {
			found = 1;
			break;
		}
	}
	
	return found;
}

/**prints information regarding the given IntList object
 */
void print(IntList *l) {
	Priv_IntList *lst = (Priv_IntList *)l;

	printf("IntList{end=%d, size=%d nums=[ ", lst->end, lst->size);
	for(int i = 0; i < lst->end; i++) {
		printf("%d ", lst->nums[i]);
	}
	printf("]}\n");
}

/**prints the all ints in the given IntList object with the corresponding prefix in A2
 * with all values separated by a space
 */
void printInts(IntList *l) {
	Priv_IntList *lst = (Priv_IntList *)l;
	char heading[] = {'L', 'I', 'N', 'U', 'X'};
	int num;
	for(int i = 0; i < lst->end - 1; i++) {
		num = lst->nums[i];
		printf("%c%02d ", heading[(num - 1) / 15], num);
	}
	if(lst->end > 0) {
		num = lst->nums[lst->end - 1];
		printf("%c%02d", heading[(num - 1) / 15], num);
	}
}

/**returns the number of elements that are currently in the IntList (indicated by lst.end)
 */
int size(IntList *l) {
	Priv_IntList *lst = (Priv_IntList *)l;
	return lst->end;
}
