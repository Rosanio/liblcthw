#include <lcthw/list.h>
#include <lcthw/list_algos.h>
#include <lcthw/dbg.h>
#include <stdbool.h>

void swap(ListNode *node1, ListNode *node2) {
	void *tmp = node1->value;
	node1->value = node2->value;
	node2->value = tmp;
}

int List_bubble_sort(List *list, List_compare cmp)
{
	int n = List_count(list);;

	// No need to sort an empty list
	if(n == 0) return 0;

	int swapped = 0;
	do {
		swapped = 0;
		int i = 0;
		int last_sorted = 0;
		LIST_FOREACH(list, first, next, cur) {
            if(cur->next) {
				if(cmp(cur->value, cur->next->value) > 0) {
                	swap(cur, cur->next);
                	swapped = 1;
					last_sorted = i;
				}
            }
			if(i >= n) break;
			i++;
		}
		n = last_sorted;
    } while(swapped);

	return 0;
}

void topDownMerge(List *list1, int start, int middle, int end, List *list2, List_compare cmp)
{
    int i = start; 
	int j = middle;
 
    // While there are elements in the left or right runs...
    for (int k = start; k < end; k++) {
        // If left run head exists and is <= existing right run head.
		void *value1 = List_get(list1, i);
		void *value2 = List_get(list1, j);
        if (i < middle && (j >= end ||  cmp(value1, value2) < 0)) {
            List_set(list2, k, value1);
            i = i + 1;
        } else {
            List_set(list2, k, value2);
            j = j + 1;
        }
    }
	List_copy(list2, list1);
}

// Sort the given run of array A[] using array B[] as a source.
// iBegin is inclusive; iEnd is exclusive (A[iEnd] is not in the set).
void topDownSplitMerge(List *list1, int start, int end, List *list2, List_compare cmp)
{
    if(end - start < 2) {                      // if run size == 1
        return;                                 //   consider it sorted
    }

    int middle = (end + start) / 2;
    // recursively sort both runs from list1 into list2
    topDownSplitMerge(list1, start, middle, list2, cmp);  // sort the left  run
    topDownSplitMerge(list1, middle, end, list2, cmp);  // sort the right run
    // merge the resulting runs from array B[] into A[]
    topDownMerge(list2, start, middle, end, list1, cmp);
}

List *List_merge_sort(List *list, List_compare cmp)
{
    List *B = List_create();
	List_copy(list, B);           // duplicate array A[] into B[]
    topDownSplitMerge(B, 0, List_count(list), list, cmp);   // sort data from B[] into A[]

	return B;
}

