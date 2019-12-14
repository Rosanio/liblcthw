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

inline List *List_merge(List * left, List * right, List_compare cmp)
{
	List *result = List_create();
	void *val = NULL;

	while (List_count(left) > 0 || List_count(right) > 0) {
		if(List_count(left) > 0 && List_count(right) > 0) {
			if(cmp(List_first(left), List_first(right)) <= 0) {
				val = List_shift(left);
			} else {
				val = List_shift(right);
			}

			List_push(result, val);
		} else if (List_count(left) > 0) {
			val = List_shift(left);
			List_push(result, val);
		} else if (List_count(right) > 0) {
			val = List_shift(right);
			List_push(result, val);
		}
	}

	return result;
}

List *List_merge_sort(List * list, List_compare cmp)
{
	List *result = NULL;

	if (List_count(list) <= 1) {
		return list;
	}

	List *left = List_create();
	List *right = List_create();
	int middle = List_count(list) / 2;

	LIST_FOREACH(list, first, next, cur) {
		if (middle > 0) {
			List_push(left, cur->value);
		} else {
			List_push(right, cur->value);
		}

		middle--;
	}

	List *sort_left = List_merge_sort(left, cmp);
	List *sort_right = List_merge_sort(right, cmp);

	if (sort_left != left)
		List_destroy(left);
	if (sort_right != right)
		List_destroy(right);

	result = List_merge(sort_left, sort_right, cmp);

	List_destroy(sort_left);
	List_destroy(sort_right);

	return result;
}

void List_insert_sorted(List *list, void *value, List_compare cmp)
{
	ListNode *newNode = calloc(1, sizeof(ListNode *));
	newNode->value = value;
	LIST_FOREACH(list, first, next, cur) {
		if(cmp(cur->value, value) > 0) {
			cur->next->prev = newNode;
			newNode->next = cur->next;
			cur->next = newNode;
			newNode->prev = cur;
			list->count++;
			break;
		}
	}
}

