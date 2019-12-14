#include <lcthw/list.h>
#include <lcthw/dbg.h>
#include <string.h>

List *List_create()
{
	return calloc(1, sizeof(List));
}

void List_destroy(List * list)
{
	check(list, "list cannot be NULL");
	LIST_FOREACH(list, first, next, cur) {
		if (cur->prev) {
			free(cur->prev);
		}
	}

	free(list->last);
	free(list);

error:
	return;
}

void List_clear(List * list)
{
	int i;
	int count = List_count(list);
	for(i = 0; i < count; i++) {
		List_remove(list, list->first);
	}
}

void List_clear_destroy(List * list)
{
	LIST_FOREACH(list, first, next, cur) {
		if (cur->prev) {
			free(cur->prev);
		}
		free(cur->value);
	}

	free(list->last);
	free(list);
}

void List_push(List * list, void *value)
{
	ListNode *node = calloc(1, sizeof(ListNode));
	check_mem(node);

	node->value = value;

	if (list->last == NULL) {
		list->first = node;
		list->last = node;
	} else {
		list->last->next = node;
		node->prev = list->last;
		list->last = node;
	}

	list->count++;


error:
	return;
}

void *List_pop(List * list)
{
	ListNode *node = list->last;
	return node != NULL ? List_remove(list, node) : NULL;
}

void List_unshift(List * list, void *value)
{
	ListNode *node = calloc(1, sizeof(ListNode));
	check_mem(node);

	node->value = value;

	if (list->first == NULL) {
		list->first = node;
		list->last = node;
	} else {
		node->next = list->first;
		list->first->prev = node;
		list->first = node;
	}

	list->count++;

error:
	return;
}

void *List_shift(List * list)
{
	ListNode *node = list->first;
	return node != NULL ? List_remove(list, node) : NULL;
}

void *List_remove(List * list, ListNode * node)
{
	void *result = NULL;

	check(list->first && list->last, "List is empty.");
	check(node, "node can't be NULL");

	if (node == list->first && node == list->last) {
		list->first = NULL;
		list->last = NULL;
	} else if (node == list->first) {
		list->first = node->next;
		check(list->first != NULL,
				"Invalid list, somehow got a first that is NULL.");
		list->first->prev = NULL;
	} else if (node == list->last) {
		list->last = node->prev;
		check(list->last != NULL,
				"Invalid list, somehow got a next that is NULL.");
		list->last->next = NULL;
	} else {
		ListNode *after = node->next;
		ListNode *before = node->prev;
		after->prev = before;
		before->next = after;
	}

	list->count--;
	result = node->value;
	free(node);

error:
	return result;
}

void List_copy(List *source, List *dest)
{
	List_clear(dest);
	LIST_FOREACH(source, first, next, cur) {
		List_push(dest, cur->value);
	}
}

List *List_join(List *list1, List *list2)
{
	list1->last->next = list2->first;
	list2->first->prev = list1->last;
	list1->count += list2->count;
	list1->last = list2->last;
	return list1;
}

List **List_split(List *list, int index)
{
	List **lists = calloc(2, sizeof(List *));;
	lists[0] = List_create();
	lists[1] = List_create();

	int i = 0;
	LIST_FOREACH(list, first, next, cur) {
		if(i == index) {
			lists[0]->first = list->first;
			lists[0]->last = cur;
			lists[1]->first = cur->next;
			lists[1]->last = list->last;
			break;
		}
		i++;
	}
	lists[0]->count = index + 1;
	lists[1]->count = list->count - (index + 1);

	return lists;	
}

void *List_get(List *list, int index) 
{
	check(index < List_count(list), "Index out of bounds");

	int count = 0;
	LIST_FOREACH(list, first, next, cur) {
		if(count == index)
			return cur->value;
		count++;
	}
	sentinel("List overflow");
error:
	return NULL;
}

void List_set(List *list, int index, void *value) 
{
	int count = 0;
	LIST_FOREACH(list, first, next, cur) {
		if(index == count) {
			cur->value = value;
			break;
		}
		count++;
	}
}

char *List_to_string(List *list)
{
	char *result = "";
	char *tmp = malloc(0);
	LIST_FOREACH(list, first, next, cur) {
		char *value = (char *)cur->value;
		tmp = malloc(3 + strlen(result) + strlen(value));
		strcpy(tmp, result);
		strcat(tmp, value);
		if(cur->next)
			strcat(tmp, ", ");
		result = (char *)malloc(strlen(tmp));
		strcpy(result, tmp);
	}
	strcat(result, "\0");
	return result;
}

void List_insert(List *list, void *value, int index)
{
	check(index > List_count(list), "IndexOutOfBounds List_insert");
	ListNode *newNode = calloc(1, sizeof(ListNode *));
	newNode->value = value;
	int i = 0;
	LIST_FOREACH(list, first, next, cur) {
		if (i == index) {
			if (cur->prev) {
				cur->prev->next = newNode;
				newNode->prev = cur->prev->next;
			}
			cur->prev = newNode;
			newNode->next = cur;
			list->count++;
			break;
		}
	}

error:
	return;
}

