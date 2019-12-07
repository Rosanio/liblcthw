#include "minunit.h"
#include <lcthw/list.h>
#include <assert.h>

static List *list = NULL;
char *test1 = "test1 data";
char *test2 = "test2 data";
char *test3 = "test3 data";

char *test_create()
{
	list = List_create();
	mu_assert(list != NULL, "Failed to create list.");

	return NULL;
}

char *test_clear_destroy()
{
	List_clear_destroy(list);

	return NULL;
}

char *test_push_pop()
{
	List_push(list, test1);
	mu_assert(List_last(list) == test1, "Wrong last value.");

	List_push(list, test2);
	mu_assert(List_last(list) == test2, "Wrong last value.");

	List_push(list, test3);
	mu_assert(List_last(list) == test3, "Wrong last value.");
	mu_assert(List_count(list) == 3, "Wrong count on push.");

	char *val = List_pop(list);
	mu_assert(val == test3, "Wrong value on pop.");

	val = List_pop(list);
	mu_assert(val == test2, "Wrong value on pop.");

	val = List_pop(list);
	mu_assert(val == test1, "Wrong value on pop.");
	mu_assert(List_count(list) == 0, "Wrong count after pop.");

	return NULL;
}

char *test_unshift()
{
	List_unshift(list, test1);
	mu_assert(List_first(list) == test1, "Wrong first value.");

	List_unshift(list, test2);
	mu_assert(List_first(list) == test2, "Wrong first value.");

	List_unshift(list, test3);
	mu_assert(List_first(list) == test3, "Wrong first value.");
	mu_assert(List_count(list) == 3, "Wrong count on unshift.");

	return NULL;
}

char *test_remove()
{
	// we only need to test the middle remove case since push/shift
	// already tests the other cases

	char *val = List_remove(list, list->first->next);
	mu_assert(val == test2, "Wrong removed element.");
	mu_assert(List_count(list) == 2, "Wrong count after remove.");
	mu_assert(List_first(list) == test3, "Wrong first after remove.");
	mu_assert(List_last(list) == test1, "Wrong last after remove.");

	return NULL;
}

char *test_shift()
{
	mu_assert(List_count(list) != 0, "Wrong count before shift.");

	char *val = List_shift(list);
	mu_assert(val == test3, "Wrong value on shift.");

	val = List_shift(list);
	mu_assert(val == test1, "Wrong value on shift.");
	mu_assert(List_count(list) == 0, "Wrong count after shift.");

	return NULL;
}

char *test_copy()
{
	List *list2 = List_create();
	List_copy(list, list2);
	mu_assert(List_count(list) == List_count(list2), "Copy does not have same length.");

	mu_assert(List_first(list) == List_first(list2), "First element of copy is wrong.");
	mu_assert(List_last(list) == List_last(list2), "Last element of copy is wrong.");

	List *list3 = List_create();
	List_push(list3, test3);
	List_push(list3, test2);
	List_push(list3, test1);
	List_copy(list, list3);
	mu_assert(List_count(list) == List_count(list3), "Second copy failed; count is incorrect");

	return NULL;
}

char *test_join()
{
	List *list1 = List_create();
	List *list2 = List_create();
	List_push(list1, test1);
	List_push(list1, test2);
	List_push(list2, test3);

	list = List_join(list1, list2);

	mu_assert(List_count(list) == 3, "Wrong number of elements after join.");
	mu_assert(List_first(list) == test1, "First element is wrong after join.");
	mu_assert(List_last(list) == test3, "Last element is wrong after join.");

	return NULL;
}

char *test_split()
{
	List **lists = List_split(list, 1);
	List *list1 = lists[0];
	List *list2 = lists[1];

	mu_assert(List_first(list1) == test1, "First element of first list is wrong.");
	mu_assert(List_last(list1) == test2, "Last element of first list is wrong.");
	mu_assert(List_first(list2) == test3, "First element of second list is wrong.");
	mu_assert(List_last(list2) == test3, "Last element of second list is wrong.");
	mu_assert(List_count(list1) == 2, "Wrong number of elements in first list.");
	mu_assert(List_count(list2) == 1, "Wrong number of elements in second list.");

	return NULL;
}

char *test_get()
{
	char *got = List_get(list, 2);
	mu_assert(got == test3, "Get returned incorrect value.");

	return NULL;
}

char *test_set()
{
	List_set(list, 2, test1);
	mu_assert(List_get(list, 2) == test1, "List has incorrect value after set");

	return NULL;
}

char *test_to_string()
{
	char *list_string = List_to_string(list);
	mu_assert(strcmp(list_string, "test1 data, test2 data, test1 data") == 0, "String is incorrect");

	return NULL;
}

char *all_tests()
{
	mu_suite_start();

	mu_run_test(test_create);
	mu_run_test(test_push_pop);
	mu_run_test(test_unshift);
	mu_run_test(test_remove);
	mu_run_test(test_copy);
	mu_run_test(test_shift);
	mu_run_test(test_clear_destroy);

	mu_run_test(test_join);
	mu_run_test(test_split);
	mu_run_test(test_get);
	mu_run_test(test_set);
	mu_run_test(test_to_string);

	return NULL;
}

RUN_TESTS(all_tests);

