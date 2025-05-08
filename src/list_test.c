#include <stdio.h>
#include <stdint.h>
#include "list.h"

#define ASSERT_FALSE(expr) do {\
	if (!!(expr)) { fprintf(stderr, "[E] %s: %d\n", __func__, __LINE__); }\
	else {fprintf(stdout, "[D] %s: %d\n", __func__, __LINE__);}\
} while (0)

#define ASSERT_TRUE(expr) do {\
	if (!(expr)) { fprintf(stderr, "[E] %s: %d\n", __func__, __LINE__); }\
	else {fprintf(stdout, "[D] %s: %d\n", __func__, __LINE__);}\
} while (0)

#define ASSERT_FALSE_MSG(expr, msg) do {\
	if (!!(expr)) { fprintf(stderr, "[E] %s: %d %s\n", __func__, __LINE__, msg); }\
	else {fprintf(stdout, "[D] %s: %d %s\n", __func__, __LINE__, msg);}\
} while (0)

#define ASSERT_TRUE_MSG(expr, msg) do {\
	if (!(expr)) { fprintf(stderr, "[E] %s: %d %s\n", __func__, __LINE__, msg); }\
	else {fprintf(stdout, "[D] %s: %d %s\n", __func__, __LINE__, msg);}\
} while (0)

#define ASSERT_PTR_EQ(p1, p2) do {\
	if ((p1) != (p2)) { fprintf(stderr, "[E] %s: %d\n", __func__, __LINE__); }\
	else {fprintf(stdout, "[D] %s: %d\n", __func__, __LINE__);}\
} while (0)

#define ASSERT_EQ(p1, p2) do {\
	if ((p1) != (p2)) { fprintf(stderr, "[E] %s: %d\n", __func__, __LINE__); }\
	else {fprintf(stdout, "[D] %s: %d\n", __func__, __LINE__);}\
} while (0)

static void list_test_list_rotate_left(void)
{
	struct list_head a, b;
	LIST_HEAD(list);

	list_add_tail(&a, &list);
	list_add_tail(&b, &list);

	/* before: [list] -> a -> b */
	list_rotate_left(&list);
	/* after: [list] -> b -> a */

	ASSERT_PTR_EQ(list.next, &b);
	ASSERT_PTR_EQ(b.prev, &list);
	ASSERT_PTR_EQ(b.next, &a);
}

static void list_test_list_rotate_to_front(void)
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
		ASSERT_PTR_EQ(ptr, list_values[i]);
		i++;
	}
	ASSERT_EQ(i, 4);
}

static void list_test_list_is_singular(void)
{
	struct list_head a, b;
	LIST_HEAD(list);

	/* [list] empty */
	ASSERT_FALSE(list_is_singular(&list));

	list_add_tail(&a, &list);

	/* [list] -> a */
	ASSERT_TRUE(list_is_singular(&list));

	list_add_tail(&b, &list);

	/* [list] -> a -> b */
	ASSERT_FALSE(list_is_singular(&list));
}

static void list_test_list_cut_position(void)
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
		ASSERT_PTR_EQ(cur, &entries[i]);
		i++;
	}

	ASSERT_EQ(i, 2);

	list_for_each(cur, &list1) {
		ASSERT_PTR_EQ(cur, &entries[i]);
		i++;
	}
}

static void list_test_list_cut_before(void)
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
		ASSERT_PTR_EQ(cur, &entries[i]);
		i++;
	}

	ASSERT_EQ(i, 1);

	list_for_each(cur, &list1) {
		ASSERT_PTR_EQ(cur, &entries[i]);
		i++;
	}
}

static void list_test_list_splice(void)
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
		ASSERT_PTR_EQ(cur, &entries[i]);
		i++;
	}

	ASSERT_EQ(i, 5);
}

static void list_test_list_splice_tail(void)
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
		ASSERT_PTR_EQ(cur, &entries[i]);
		i++;
	}

	ASSERT_EQ(i, 5);
}

static void list_test_list_splice_init(void)
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
		ASSERT_PTR_EQ(cur, &entries[i]);
		i++;
	}

	ASSERT_EQ(i, 5);

	ASSERT_TRUE(list_empty_careful(&list2));
}

static void list_test_list_splice_tail_init(void)
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
		ASSERT_PTR_EQ(cur, &entries[i]);
		i++;
	}

	ASSERT_EQ(i, 5);

	ASSERT_TRUE(list_empty_careful(&list2));
}

static void list_test_list_del(void)
{
	struct list_head a, b;
	LIST_HEAD(list);

	list_add_tail(&a, &list);
	list_add_tail(&b, &list);

	/* before: [list] -> a -> b */
	list_del(&a);

	/* now: [list] -> b */
	ASSERT_PTR_EQ(list.next, &b);
	ASSERT_PTR_EQ(b.prev, &list);
}

static void list_test_list_replace(void)
{
	struct list_head a_old, a_new, b;
	LIST_HEAD(list);

	list_add_tail(&a_old, &list);
	list_add_tail(&b, &list);

	/* before: [list] -> a_old -> b */
	list_replace(&a_old, &a_new);

	/* now: [list] -> a_new -> b */
	ASSERT_PTR_EQ(list.next, &a_new);
	ASSERT_PTR_EQ(b.prev, &a_new);
}

static void list_test_list_move(void)
{
	struct list_head a, b;
	LIST_HEAD(list1);
	LIST_HEAD(list2);

	list_add_tail(&a, &list1);
	list_add_tail(&b, &list2);

	/* before: [list1] -> a, [list2] -> b */
	list_move(&a, &list2);
	/* after: [list1] empty, [list2] -> a -> b */

	ASSERT_TRUE(list_empty(&list1));

	ASSERT_PTR_EQ(&a, list2.next);
	ASSERT_PTR_EQ(&b, a.next);
}

static void list_test_list_move_tail(void)
{
	struct list_head a, b;
	LIST_HEAD(list1);
	LIST_HEAD(list2);

	list_add_tail(&a, &list1);
	list_add_tail(&b, &list2);

	/* before: [list1] -> a, [list2] -> b */
	list_move_tail(&a, &list2);
	/* after: [list1] empty, [list2] -> b -> a */

	ASSERT_TRUE(list_empty(&list1));

	ASSERT_PTR_EQ(&b, list2.next);
	ASSERT_PTR_EQ(&a, b.next);
}

static void list_test_list_bulk_move_tail(void)
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
		ASSERT_PTR_EQ(ptr, list1_values[i]);
		i++;
	}
	ASSERT_EQ(i, 4);
	i = 0;
	list_for_each(ptr, &list2) {
		ASSERT_PTR_EQ(ptr, list2_values[i]);
		i++;
	}
	ASSERT_EQ(i, 2);
}

static void list_test_list_swap(void)
{
	struct list_head a, b;
	LIST_HEAD(list);

	list_add_tail(&a, &list);
	list_add_tail(&b, &list);

	/* before: [list] -> a -> b */
	list_swap(&a, &b);

	/* after: [list] -> b -> a */
	ASSERT_PTR_EQ(&b, list.next);
	ASSERT_PTR_EQ(&a, list.prev);

	ASSERT_PTR_EQ(&a, b.next);
	ASSERT_PTR_EQ(&list, b.prev);

	ASSERT_PTR_EQ(&list, a.next);
	ASSERT_PTR_EQ(&b, a.prev);
}

static void list_test_list_add_tail()
{
	struct list_head a, b;
	LIST_HEAD(list);

	list_add_tail(&a, &list);
	list_add_tail(&b, &list);

	/* should be [list] -> a -> b */
	ASSERT_PTR_EQ(list.next, &a);
	ASSERT_PTR_EQ(a.prev, &list);
	ASSERT_PTR_EQ(a.next, &b);
}

static void list_test_list_add(void)
{
	struct list_head a, b;
	LIST_HEAD(list);

	list_add(&a, &list);
	list_add(&b, &list);

	/* should be [list] -> b -> a */
	ASSERT_PTR_EQ(list.next, &b);
	ASSERT_PTR_EQ(b.prev, &list);
	ASSERT_PTR_EQ(b.next, &a);
}

static void list_test_list_init()
{
	list_t l1 = LIST_HEAD_INIT(l1);
	list_t l2 = { &(l2), &(l2) };
	struct list_head l3;
	LIST_HEAD(l4);

	INIT_LIST_HEAD(&l3);
	ASSERT_TRUE(list_empty(&l1));
	ASSERT_TRUE(list_empty(&l2));
	ASSERT_TRUE(list_empty(&l3));
	ASSERT_TRUE(list_empty(&l4));
}

struct list_test_struct {
	int data;
	struct list_head list;
};

static void list_test_list_entry_is_head(void)
{
	struct list_test_struct test_struct1, test_struct2, test_struct3;

	INIT_LIST_HEAD(&test_struct1.list);
	INIT_LIST_HEAD(&test_struct3.list);

	list_add_tail(&test_struct2.list, &test_struct1.list);

	ASSERT_TRUE_MSG(
		list_entry_is_head((&test_struct1), &test_struct1.list, list),
		"Head element of same list");
	ASSERT_FALSE_MSG(
		list_entry_is_head((&test_struct2), &test_struct1.list, list),
		"Non-head element of same list");
	ASSERT_FALSE_MSG(
		list_entry_is_head((&test_struct3), &test_struct1.list, list),
		"Head element of different list");
}

static void list_test_list_first_entry(void)
{
	struct list_test_struct test_struct1, test_struct2;
	LIST_HEAD(list);

	list_add_tail(&test_struct1.list, &list);
	list_add_tail(&test_struct2.list, &list);


	ASSERT_PTR_EQ(&test_struct1, list_first_entry(&list,
				struct list_test_struct, list));
}

static void list_test_list_last_entry(void)
{
	struct list_test_struct test_struct1, test_struct2;
	LIST_HEAD(list);

	list_add_tail(&test_struct1.list, &list);
	list_add_tail(&test_struct2.list, &list);


	ASSERT_PTR_EQ(&test_struct2, list_last_entry(&list,
				struct list_test_struct, list));
}

static void list_test_list_first_entry_or_null(void)
{
	struct list_test_struct test_struct1, test_struct2;
	LIST_HEAD(list);

	ASSERT_FALSE(list_first_entry_or_null(&list,
				struct list_test_struct, list));

	list_add_tail(&test_struct1.list, &list);
	list_add_tail(&test_struct2.list, &list);

	ASSERT_PTR_EQ(&test_struct1,
			list_first_entry_or_null(&list,
				struct list_test_struct, list));
}

static void list_test_list_next_entry(void)
{
	struct list_test_struct test_struct1, test_struct2;
	LIST_HEAD(list);

	list_add_tail(&test_struct1.list, &list);
	list_add_tail(&test_struct2.list, &list);


	ASSERT_PTR_EQ(&test_struct2, list_next_entry(&test_struct1,
				list));
}

static void list_test_list_prev_entry(void)
{
	struct list_test_struct test_struct1, test_struct2;
	LIST_HEAD(list);

	list_add_tail(&test_struct1.list, &list);
	list_add_tail(&test_struct2.list, &list);


	ASSERT_PTR_EQ(&test_struct1, list_prev_entry(&test_struct2,
				list));
}

static void list_test_list_for_each(void)
{
	struct list_head entries[3], *cur;
	LIST_HEAD(list);
	int i = 0;

	list_add_tail(&entries[0], &list);
	list_add_tail(&entries[1], &list);
	list_add_tail(&entries[2], &list);

	list_for_each(cur, &list) {
		ASSERT_PTR_EQ(cur, &entries[i]);
		i++;
	}

	ASSERT_EQ(i, 3);
}

static void list_test_list_for_each_prev(void)
{
	struct list_head entries[3], *cur;
	LIST_HEAD(list);
	int i = 2;

	list_add_tail(&entries[0], &list);
	list_add_tail(&entries[1], &list);
	list_add_tail(&entries[2], &list);

	list_for_each_prev(cur, &list) {
		ASSERT_PTR_EQ(cur, &entries[i]);
		i--;
	}

	ASSERT_EQ(i, -1);
}

static void list_test_list_for_each_entry(void)
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
		ASSERT_EQ(cur->data, i);
		i++;
	}

	ASSERT_EQ(i, 5);
}

static void list_test_list_for_each_entry_continue(void)
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
		ASSERT_EQ(cur->data, i);
		i++;
	}

	ASSERT_EQ(i, 5);
}

static void list_test_list_for_each_entry_reverse(void)
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
		ASSERT_EQ(cur->data, i);
		i--;
	}

	ASSERT_EQ(i, -1);
}

typedef struct user_s {
// token id
	uint8_t 	id;
	struct list_head		list; // user list data representation (if any)
} user_t;

void run_list_test()
{
	list_test_list_init();
	list_test_list_add();
	list_test_list_add_tail();
	list_test_list_swap();
	list_test_list_replace();
	list_test_list_del();
	list_test_list_move();
	list_test_list_move_tail();
	list_test_list_bulk_move_tail();
	list_test_list_rotate_left();
	list_test_list_rotate_to_front();
	list_test_list_is_singular();
	list_test_list_cut_position();
	list_test_list_cut_before();
	list_test_list_splice();
	list_test_list_splice_tail();
	list_test_list_splice_init();
	list_test_list_splice_tail_init();
	list_test_list_entry_is_head();
	list_test_list_first_entry();
	list_test_list_last_entry();
	list_test_list_first_entry_or_null();
	list_test_list_next_entry();
	list_test_list_prev_entry();
	list_test_list_for_each();
	list_test_list_for_each_prev();
	list_test_list_for_each_entry();
	list_test_list_for_each_entry_reverse();
	list_test_list_for_each_entry_continue();
}
