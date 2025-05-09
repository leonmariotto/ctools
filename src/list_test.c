#include <stdio.h>
#include "list.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include "cmocka.h"


static void list_test_list_rotate_left(void **state)
{
	struct list_head a, b;
	LIST_HEAD(list);

	list_add_tail(&a, &list);
	list_add_tail(&b, &list);

	/* before: [list] -> a -> b */
	list_rotate_left(&list);
	/* after: [list] -> b -> a */

	assert_ptr_equal(list.next, &b);
	assert_ptr_equal(b.prev, &list);
	assert_ptr_equal(b.next, &a);
}

static void list_test_list_rotate_to_front(void **state)
{
	struct list_head a, b, c, d;
	struct list_head *list_values[] = { &c, &d, &a, &b };
	struct list_head *ptr;
	LIST_HEAD(list);
	int i = 0;

	list_add_tail(&a, &list);
	list_add_tail(&b, &list);
	list_add_tail(&c, &list);
	list_add_tail(&d, &list);

	/* before: [list] -> a -> b -> c -> d */
	list_rotate_to_front(&c, &list);
	/* after: [list] -> c -> d -> a -> b */

	list_for_each(ptr, &list) {
		assert_ptr_equal(ptr, list_values[i]);
		i++;
	}
	assert_int_equal(i, 4);
}

static void list_test_list_is_singular(void **state)
{
	struct list_head a, b;
	LIST_HEAD(list);

	/* [list] empty */
	assert_false(list_is_singular(&list));

	list_add_tail(&a, &list);

	/* [list] -> a */
	assert_true(list_is_singular(&list));

	list_add_tail(&b, &list);

	/* [list] -> a -> b */
	assert_false(list_is_singular(&list));
}

static void list_test_list_cut_position(void **state)
{
	struct list_head entries[3], *cur;
	LIST_HEAD(list1);
	LIST_HEAD(list2);
	int i = 0;

	list_add_tail(&entries[0], &list1);
	list_add_tail(&entries[1], &list1);
	list_add_tail(&entries[2], &list1);

	/* before: [list1] -> entries[0] -> entries[1] -> entries[2] */
	list_cut_position(&list2, &list1, &entries[1]);
	/* after: [list2] -> entries[0] -> entries[1], [list1] -> entries[2] */

	list_for_each(cur, &list2) {
		assert_ptr_equal(cur, &entries[i]);
		i++;
	}

	assert_int_equal(i, 2);

	list_for_each(cur, &list1) {
		assert_ptr_equal(cur, &entries[i]);
		i++;
	}
}

static void list_test_list_cut_before(void **state)
{
	struct list_head entries[3], *cur;
	LIST_HEAD(list1);
	LIST_HEAD(list2);
	int i = 0;

	list_add_tail(&entries[0], &list1);
	list_add_tail(&entries[1], &list1);
	list_add_tail(&entries[2], &list1);

	/* before: [list1] -> entries[0] -> entries[1] -> entries[2] */
	list_cut_before(&list2, &list1, &entries[1]);
	/* after: [list2] -> entries[0], [list1] -> entries[1] -> entries[2] */

	list_for_each(cur, &list2) {
		assert_ptr_equal(cur, &entries[i]);
		i++;
	}

	assert_int_equal(i, 1);

	list_for_each(cur, &list1) {
		assert_ptr_equal(cur, &entries[i]);
		i++;
	}
}

static void list_test_list_splice(void **state)
{
	struct list_head entries[5], *cur;
	LIST_HEAD(list1);
	LIST_HEAD(list2);
	int i = 0;

	list_add_tail(&entries[0], &list1);
	list_add_tail(&entries[1], &list1);
	list_add_tail(&entries[2], &list2);
	list_add_tail(&entries[3], &list2);
	list_add_tail(&entries[4], &list1);

	/* before: [list1]->e[0]->e[1]->e[4], [list2]->e[2]->e[3] */
	list_splice(&list2, &entries[1]);
	/* after: [list1]->e[0]->e[1]->e[2]->e[3]->e[4], [list2] uninit */

	list_for_each(cur, &list1) {
		assert_ptr_equal(cur, &entries[i]);
		i++;
	}

	assert_int_equal(i, 5);
}

static void list_test_list_splice_tail(void **state)
{
	struct list_head entries[5], *cur;
	LIST_HEAD(list1);
	LIST_HEAD(list2);
	int i = 0;

	list_add_tail(&entries[0], &list1);
	list_add_tail(&entries[1], &list1);
	list_add_tail(&entries[2], &list2);
	list_add_tail(&entries[3], &list2);
	list_add_tail(&entries[4], &list1);

	/* before: [list1]->e[0]->e[1]->e[4], [list2]->e[2]->e[3] */
	list_splice_tail(&list2, &entries[4]);
	/* after: [list1]->e[0]->e[1]->e[2]->e[3]->e[4], [list2] uninit */

	list_for_each(cur, &list1) {
		assert_ptr_equal(cur, &entries[i]);
		i++;
	}

	assert_int_equal(i, 5);
}

static void list_test_list_splice_init(void **state)
{
	struct list_head entries[5], *cur;
	LIST_HEAD(list1);
	LIST_HEAD(list2);
	int i = 0;

	list_add_tail(&entries[0], &list1);
	list_add_tail(&entries[1], &list1);
	list_add_tail(&entries[2], &list2);
	list_add_tail(&entries[3], &list2);
	list_add_tail(&entries[4], &list1);

	/* before: [list1]->e[0]->e[1]->e[4], [list2]->e[2]->e[3] */
	list_splice_init(&list2, &entries[1]);
	/* after: [list1]->e[0]->e[1]->e[2]->e[3]->e[4], [list2] empty */

	list_for_each(cur, &list1) {
		assert_ptr_equal(cur, &entries[i]);
		i++;
	}

	assert_int_equal(i, 5);

	assert_true(list_empty_careful(&list2));
}

static void list_test_list_splice_tail_init(void **state)
{
	struct list_head entries[5], *cur;
	LIST_HEAD(list1);
	LIST_HEAD(list2);
	int i = 0;

	list_add_tail(&entries[0], &list1);
	list_add_tail(&entries[1], &list1);
	list_add_tail(&entries[2], &list2);
	list_add_tail(&entries[3], &list2);
	list_add_tail(&entries[4], &list1);

	/* before: [list1]->e[0]->e[1]->e[4], [list2]->e[2]->e[3] */
	list_splice_tail_init(&list2, &entries[4]);
	/* after: [list1]->e[0]->e[1]->e[2]->e[3]->e[4], [list2] empty */

	list_for_each(cur, &list1) {
		assert_ptr_equal(cur, &entries[i]);
		i++;
	}

	assert_int_equal(i, 5);

	assert_true(list_empty_careful(&list2));
}

static void list_test_list_del(void **state)
{
	struct list_head a, b;
	LIST_HEAD(list);

	list_add_tail(&a, &list);
	list_add_tail(&b, &list);

	/* before: [list] -> a -> b */
	list_del(&a);

	/* now: [list] -> b */
	assert_ptr_equal(list.next, &b);
	assert_ptr_equal(b.prev, &list);
}

static void list_test_list_replace(void **state)
{
	struct list_head a_old, a_new, b;
	LIST_HEAD(list);

	list_add_tail(&a_old, &list);
	list_add_tail(&b, &list);

	/* before: [list] -> a_old -> b */
	list_replace(&a_old, &a_new);

	/* now: [list] -> a_new -> b */
	assert_ptr_equal(list.next, &a_new);
	assert_ptr_equal(b.prev, &a_new);
}

static void list_test_list_move(void **state)
{
	struct list_head a, b;
	LIST_HEAD(list1);
	LIST_HEAD(list2);

	list_add_tail(&a, &list1);
	list_add_tail(&b, &list2);

	/* before: [list1] -> a, [list2] -> b */
	list_move(&a, &list2);
	/* after: [list1] empty, [list2] -> a -> b */

	assert_true(list_empty(&list1));

	assert_ptr_equal(&a, list2.next);
	assert_ptr_equal(&b, a.next);
}

static void list_test_list_move_tail(void **state)
{
	struct list_head a, b;
	LIST_HEAD(list1);
	LIST_HEAD(list2);

	list_add_tail(&a, &list1);
	list_add_tail(&b, &list2);

	/* before: [list1] -> a, [list2] -> b */
	list_move_tail(&a, &list2);
	/* after: [list1] empty, [list2] -> b -> a */

	assert_true(list_empty(&list1));

	assert_ptr_equal(&b, list2.next);
	assert_ptr_equal(&a, b.next);
}

static void list_test_list_bulk_move_tail(void **state)
{
	struct list_head a, b, c, d, x, y;
	struct list_head *list1_values[] = { &x, &b, &c, &y };
	struct list_head *list2_values[] = { &a, &d };
	struct list_head *ptr;
	LIST_HEAD(list1);
	LIST_HEAD(list2);
	int i = 0;

	list_add_tail(&x, &list1);
	list_add_tail(&y, &list1);

	list_add_tail(&a, &list2);
	list_add_tail(&b, &list2);
	list_add_tail(&c, &list2);
	list_add_tail(&d, &list2);

	/* before: [list1] -> x -> y, [list2] -> a -> b -> c -> d */
	list_bulk_move_tail(&y, &b, &c);
	/* after: [list1] -> x -> b -> c -> y, [list2] -> a -> d */

	list_for_each(ptr, &list1) {
		assert_ptr_equal(ptr, list1_values[i]);
		i++;
	}
	assert_int_equal(i, 4);
	i = 0;
	list_for_each(ptr, &list2) {
		assert_ptr_equal(ptr, list2_values[i]);
		i++;
	}
	assert_int_equal(i, 2);
}

static void list_test_list_swap(void **state)
{
	struct list_head a, b;
	LIST_HEAD(list);

	list_add_tail(&a, &list);
	list_add_tail(&b, &list);

	/* before: [list] -> a -> b */
	list_swap(&a, &b);

	/* after: [list] -> b -> a */
	assert_ptr_equal(&b, list.next);
	assert_ptr_equal(&a, list.prev);

	assert_ptr_equal(&a, b.next);
	assert_ptr_equal(&list, b.prev);

	assert_ptr_equal(&list, a.next);
	assert_ptr_equal(&b, a.prev);
}

static void list_test_list_add_tail()
{
	struct list_head a, b;
	LIST_HEAD(list);

	list_add_tail(&a, &list);
	list_add_tail(&b, &list);

	/* should be [list] -> a -> b */
	assert_ptr_equal(list.next, &a);
	assert_ptr_equal(a.prev, &list);
	assert_ptr_equal(a.next, &b);
}

static void list_test_list_add(void **state)
{
	struct list_head a, b;
	LIST_HEAD(list);

	list_add(&a, &list);
	list_add(&b, &list);

	/* should be [list] -> b -> a */
	assert_ptr_equal(list.next, &b);
	assert_ptr_equal(b.prev, &list);
	assert_ptr_equal(b.next, &a);
}

static void list_test_list_init()
{
	list_t l1 = LIST_HEAD_INIT(l1);
	list_t l2 = { &(l2), &(l2) };
	struct list_head l3;
	LIST_HEAD(l4);

	INIT_LIST_HEAD(&l3);
	assert_true(list_empty(&l1));
	assert_true(list_empty(&l2));
	assert_true(list_empty(&l3));
	assert_true(list_empty(&l4));
}

struct list_test_struct {
	int data;
	struct list_head list;
};

static void list_test_list_entry_is_head(void **state)
{
	struct list_test_struct test_struct1, test_struct2, test_struct3;

	INIT_LIST_HEAD(&test_struct1.list);
	INIT_LIST_HEAD(&test_struct3.list);

	list_add_tail(&test_struct2.list, &test_struct1.list);

	assert_true(
		list_entry_is_head((&test_struct1), &test_struct1.list, list));
	assert_false(
		list_entry_is_head((&test_struct2), &test_struct1.list, list));
	assert_false(
		list_entry_is_head((&test_struct3), &test_struct1.list, list));
}

static void list_test_list_first_entry(void **state)
{
	struct list_test_struct test_struct1, test_struct2;
	LIST_HEAD(list);

	list_add_tail(&test_struct1.list, &list);
	list_add_tail(&test_struct2.list, &list);


	assert_ptr_equal(&test_struct1, list_first_entry(&list,
				struct list_test_struct, list));
}

static void list_test_list_last_entry(void **state)
{
	struct list_test_struct test_struct1, test_struct2;
	LIST_HEAD(list);

	list_add_tail(&test_struct1.list, &list);
	list_add_tail(&test_struct2.list, &list);


	assert_ptr_equal(&test_struct2, list_last_entry(&list,
				struct list_test_struct, list));
}

static void list_test_list_first_entry_or_null(void **state)
{
	struct list_test_struct test_struct1, test_struct2;
	LIST_HEAD(list);

	assert_false(list_first_entry_or_null(&list,
				struct list_test_struct, list));

	list_add_tail(&test_struct1.list, &list);
	list_add_tail(&test_struct2.list, &list);

	assert_ptr_equal(&test_struct1,
			list_first_entry_or_null(&list,
				struct list_test_struct, list));
}

static void list_test_list_next_entry(void **state)
{
	struct list_test_struct test_struct1, test_struct2;
	LIST_HEAD(list);

	list_add_tail(&test_struct1.list, &list);
	list_add_tail(&test_struct2.list, &list);


	assert_ptr_equal(&test_struct2, list_next_entry(&test_struct1,
				list));
}

static void list_test_list_prev_entry(void **state)
{
	struct list_test_struct test_struct1, test_struct2;
	LIST_HEAD(list);

	list_add_tail(&test_struct1.list, &list);
	list_add_tail(&test_struct2.list, &list);


	assert_ptr_equal(&test_struct1, list_prev_entry(&test_struct2,
				list));
}

static void list_test_list_for_each(void **state)
{
	struct list_head entries[3], *cur;
	LIST_HEAD(list);
	int i = 0;

	list_add_tail(&entries[0], &list);
	list_add_tail(&entries[1], &list);
	list_add_tail(&entries[2], &list);

	list_for_each(cur, &list) {
		assert_ptr_equal(cur, &entries[i]);
		i++;
	}

	assert_int_equal(i, 3);
}

static void list_test_list_for_each_prev(void **state)
{
	struct list_head entries[3], *cur;
	LIST_HEAD(list);
	int i = 2;

	list_add_tail(&entries[0], &list);
	list_add_tail(&entries[1], &list);
	list_add_tail(&entries[2], &list);

	list_for_each_prev(cur, &list) {
		assert_ptr_equal(cur, &entries[i]);
		i--;
	}

	assert_int_equal(i, -1);
}

static void list_test_list_for_each_entry(void **state)
{
	struct list_test_struct entries[5], *cur;
	LIST_HEAD(list);
	int i = 0;

	for (i = 0; i < 5; ++i) {
		entries[i].data = i;
		list_add_tail(&entries[i].list, &list);
	}

	i = 0;

	list_for_each_entry(cur, &list, list) {
		assert_int_equal(cur->data, i);
		i++;
	}

	assert_int_equal(i, 5);
}

static void list_test_list_for_each_entry_continue(void **state)
{
	struct list_test_struct entries[5], *cur;
	LIST_HEAD(list);
	int i = 0;

	for (i = 0; i < 5; ++i) {
		entries[i].data = i;
		list_add_tail(&entries[i].list, &list);
	}

	i = 1;

	cur = &entries[1];
	list_for_each_entry_continue(cur, &list, list) {
		assert_int_equal(cur->data, i);
		i++;
	}

	assert_int_equal(i, 5);
}

static void list_test_list_for_each_entry_reverse(void **state)
{
	struct list_test_struct entries[5], *cur;
	LIST_HEAD(list);
	int i = 0;

	for (i = 0; i < 5; ++i) {
		entries[i].data = i;
		list_add_tail(&entries[i].list, &list);
	}

	i = 4;

	list_for_each_entry_reverse(cur, &list, list) {
		assert_int_equal(cur->data, i);
		i--;
	}

	assert_int_equal(i, -1);
}

typedef struct user_s {
// token id
	uint8_t 	id;
	struct list_head		list; // user list data representation (if any)
} user_t;

void run_list_test()
{
    const struct CMUnitTest list_tests[] = {
		cmocka_unit_test(list_test_list_init),
		cmocka_unit_test(list_test_list_add),
		cmocka_unit_test(list_test_list_add_tail),
		cmocka_unit_test(list_test_list_swap),
		cmocka_unit_test(list_test_list_replace),
		cmocka_unit_test(list_test_list_del),
		cmocka_unit_test(list_test_list_move),
		cmocka_unit_test(list_test_list_move_tail),
		cmocka_unit_test(list_test_list_bulk_move_tail),
		cmocka_unit_test(list_test_list_rotate_left),
		cmocka_unit_test(list_test_list_rotate_to_front),
		cmocka_unit_test(list_test_list_is_singular),
		cmocka_unit_test(list_test_list_cut_position),
		cmocka_unit_test(list_test_list_cut_before),
		cmocka_unit_test(list_test_list_splice),
		cmocka_unit_test(list_test_list_splice_tail),
		cmocka_unit_test(list_test_list_splice_init),
		cmocka_unit_test(list_test_list_splice_tail_init),
		cmocka_unit_test(list_test_list_entry_is_head),
		cmocka_unit_test(list_test_list_first_entry),
		cmocka_unit_test(list_test_list_last_entry),
		cmocka_unit_test(list_test_list_first_entry_or_null),
		cmocka_unit_test(list_test_list_next_entry),
		cmocka_unit_test(list_test_list_prev_entry),
		cmocka_unit_test(list_test_list_for_each),
		cmocka_unit_test(list_test_list_for_each_prev),
		cmocka_unit_test(list_test_list_for_each_entry),
		cmocka_unit_test(list_test_list_for_each_entry_reverse),
		cmocka_unit_test(list_test_list_for_each_entry_continue),
    };
	cmocka_set_message_output(CM_OUTPUT_XML);
    return cmocka_run_group_tests(list_tests, NULL, NULL);
}
