#include "tree.h"
#include <stdio.h>

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include "cmocka.h"

#define debug_print_tree(str, t) do { \
	fprintf(stderr, "%s:%d %s: t=%p t.next=%p t.prev=%p, t.nprev=%p t.nnext=%p\n", __func__, __LINE__, \
					str, \
					t, \
					t->x_next, \
					t->x_prev, \
					t->y_prev, \
					t->y_next \
					); \
} while (0)

struct tree_test_struct {
	int data;
	struct tree_head tree;
};

static void tree_test_tree_foreach_x_entry(void **state)
{
	struct tree_test_struct entries[5], *cur;
	TREE_HEAD(tree);
	int i = 0;

	for (i = 0; i < 5; ++i) {
		entries[i].data = i;
		tree_x_add_tail(&entries[i].tree, &tree);
	}

	i = 0;

	tree_foreach_x_entry(cur, &tree, tree) {
		assert_int_equal(cur->data, i);
		i++;
	}

	assert_int_equal(i, 5);
}
static void tree_test_tree_foreach_x_entry_continue(void **state)
{
	struct tree_test_struct entries[5], *cur;
	TREE_HEAD(tree);
	int i = 0;

	for (i = 0; i < 5; ++i) {
		entries[i].data = i;
		tree_x_add_tail(&entries[i].tree, &tree);
	}

	i = 2;

	cur = &entries[2];

	tree_foreach_x_entry_continue(cur, &tree, tree) {
		assert_int_equal(cur->data, i);
		i++;
	}

	assert_int_equal(i, 5);
}
static void tree_test_tree_foreach_x_entry_rev(void **state)
{
	struct tree_test_struct entries[5], *cur;
	TREE_HEAD(tree);
	int i = 0;

	for (i = 0; i < 5; ++i) {
		entries[i].data = i;
		tree_x_add_tail(&entries[i].tree, &tree);
	}

	i = 5;

	tree_foreach_x_entry_rev(cur, &tree, tree) {
		i--;
		assert_int_equal(cur->data, i);
	}

	assert_int_equal(i, 0);
}
static void tree_test_tree_foreach_x_entry_continue_rev(void **state)
{
	struct tree_test_struct entries[5], *cur;
	TREE_HEAD(tree);
	int i = 0;

	for (i = 0; i < 5; ++i) {
		entries[i].data = i;
		tree_x_add_tail(&entries[i].tree, &tree);
	}

	i = 3;

	cur = &entries[2];

	tree_foreach_x_entry_continue_rev(cur, &tree, tree) {
		i--;
		assert_int_equal(cur->data, i);
	}

	assert_int_equal(i, 0);
}
static void tree_test_tree_get_x_next_entry(void **state)
{
	struct tree_test_struct entries[5], *cur;
	TREE_HEAD(tree);
	int i = 0;

	for (i = 0; i < 5; ++i) {
		entries[i].data = i;
		tree_x_add_tail(&entries[i].tree, &tree);
	}

	i = 3;

	cur = &entries[2];
	struct tree_test_struct *ptr;
	ptr = tree_entry_x_next_entry(cur, tree);
	assert_int_equal(ptr, &entries[3]);
}
static void tree_test_tree_get_x_prev_entry(void **state)
{
	struct tree_test_struct entries[5], *cur;
	TREE_HEAD(tree);
	int i = 0;

	for (i = 0; i < 5; ++i) {
		entries[i].data = i;
		tree_x_add_tail(&entries[i].tree, &tree);
	}

	cur = &entries[2];
	struct tree_test_struct *ptr;
	ptr = tree_entry_x_prev_entry(cur, tree);
	assert_int_equal(ptr, &entries[1]);
}

static void tree_test_tree_foreach_y_entry(void **state)
{
	struct tree_test_struct entries[5], *cur;
	TREE_HEAD(tree);
	int i = 0;

	for (i = 0; i < 5; ++i) {
		entries[i].data = i;
		tree_y_add_tail(&entries[i].tree, &tree);
	}

	i = 0;

	tree_foreach_y_entry(cur, &tree, tree) {
		assert_int_equal(cur->data, i);
		i++;
	}

	assert_int_equal(i, 5);
}
static void tree_test_tree_foreach_y_entry_continue(void **state)
{
	struct tree_test_struct entries[5], *cur;
	TREE_HEAD(tree);
	int i = 0;

	for (i = 0; i < 5; ++i) {
		entries[i].data = i;
		tree_y_add_tail(&entries[i].tree, &tree);
	}

	i = 2;

	cur = &entries[2];

	tree_foreach_y_entry_continue(cur, &tree, tree) {
		assert_int_equal(cur->data, i);
		i++;
	}

	assert_int_equal(i, 5);
}
static void tree_test_tree_foreach_y_entry_rev(void **state)
{
	struct tree_test_struct entries[5], *cur;
	TREE_HEAD(tree);
	int i = 0;

	for (i = 0; i < 5; ++i) {
		entries[i].data = i;
		tree_y_add_tail(&entries[i].tree, &tree);
	}

	i = 5;

	tree_foreach_y_entry_rev(cur, &tree, tree) {
		i--;
		assert_int_equal(cur->data, i);
	}

	assert_int_equal(i, 0);
}
static void tree_test_tree_foreach_y_entry_continue_rev(void **state)
{
	struct tree_test_struct entries[5], *cur;
	TREE_HEAD(tree);
	int i = 0;

	for (i = 0; i < 5; ++i) {
		entries[i].data = i;
		tree_y_add_tail(&entries[i].tree, &tree);
	}

	i = 3;

	cur = &entries[2];

	tree_foreach_y_entry_continue_rev(cur, &tree, tree) {
		i--;
		assert_int_equal(cur->data, i);
	}

	assert_int_equal(i, 0);
}
static void tree_test_tree_get_y_next_entry(void **state)
{
	struct tree_test_struct entries[5], *cur;
	TREE_HEAD(tree);
	int i = 0;

	for (i = 0; i < 5; ++i) {
		entries[i].data = i;
		tree_y_add_tail(&entries[i].tree, &tree);
	}

	i = 3;

	cur = &entries[2];
	struct tree_test_struct *ptr;
	ptr = tree_entry_y_next_entry(cur, tree);
	assert_int_equal(ptr, &entries[3]);
}
static void tree_test_tree_get_y_prev_entry(void **state)
{
	struct tree_test_struct entries[5], *cur;
	TREE_HEAD(tree);
	int i = 0;

	for (i = 0; i < 5; ++i) {
		entries[i].data = i;
		tree_y_add_tail(&entries[i].tree, &tree);
	}

	cur = &entries[2];
	struct tree_test_struct *ptr;
	ptr = tree_entry_y_prev_entry(cur, tree);
	assert_int_equal(ptr, &entries[1]);
}

struct old_tree_test_struct
{
	uint8_t data;
	uint8_t floor;
	char *msg;
	struct tree_test_struct *parent;
	struct list_head list; // list of this level
	struct list_head child; // list of child level
};

void tree_test_init()
{
	tree_t l1 = TREE_HEAD_INIT(l1);
	tree_t l2 = { &(l2), &(l2), &(l2), &(l2) };
	struct tree_head l3;
	TREE_HEAD(l4);

	INIT_TREE_HEAD(&l3);
	assert_true(tree_empty(&l1));
	assert_true(tree_empty(&l2));
	assert_true(tree_empty(&l3));
	assert_true(tree_empty(&l4));
}

void run_tree_test()
{
    const struct CMUnitTest tree_tests[] = {
		cmocka_unit_test(tree_test_init),
		cmocka_unit_test(tree_test_tree_foreach_x_entry),
		cmocka_unit_test(tree_test_tree_foreach_x_entry_continue),
		cmocka_unit_test(tree_test_tree_foreach_x_entry_rev),
		cmocka_unit_test(tree_test_tree_foreach_x_entry_continue_rev),
		cmocka_unit_test(tree_test_tree_get_x_next_entry),
		cmocka_unit_test(tree_test_tree_get_x_prev_entry),
		cmocka_unit_test(tree_test_tree_foreach_y_entry),
		cmocka_unit_test(tree_test_tree_foreach_y_entry_continue),
		cmocka_unit_test(tree_test_tree_foreach_y_entry_rev),
		cmocka_unit_test(tree_test_tree_foreach_y_entry_continue_rev),
		cmocka_unit_test(tree_test_tree_get_y_next_entry),
		cmocka_unit_test(tree_test_tree_get_y_prev_entry)
	};
	cmocka_set_message_output(CM_OUTPUT_XML);
    return cmocka_run_group_tests(tree_tests, NULL, NULL);
}


/*
void old_run_tree_test()
{
	struct tree_test_struct root, *cur, *cur2, *cur3;
	struct tree_test_struct st[10];
	struct tree_test_struct nd[10 * 10];
	struct tree_test_struct rd[10 * 10 * 10];
	uint8_t i = 0;

	//INIT_LIST_HEAD(&list);
	INIT_LIST_HEAD(&root.list);
	INIT_LIST_HEAD(&root.child);
	for (i = 0; i < 10 ; ++i) {
		fprintf(stderr, "BUILD LVL1 idx=%d\n", i);
		st[i].data = i;
		st[i].floor = 1;
		// INIT_LIST_HEAD(&st[i].list);
		list_add_tail(&st[i].list, &root.child);
		st[i].parent = &root;
		INIT_LIST_HEAD(&st[i].child);
		for (uint8_t j = 0; j < 10 ; ++j) {
			fprintf(stderr, "BUILD LVL2 idx=%d\n", j  + 10 * i);
			nd[j  + 10 * i].data = j;
			nd[j  + 10 * i].floor = 2;
			nd[j  + 10 * i].parent = &st[i];
			// INIT_LIST_HEAD(&nd[j].list);
			list_add_tail(&nd[j + 10 * i].list, &st[i].child);
			INIT_LIST_HEAD(&nd[j + 10 * i].child);
			for (uint8_t k = 0; k < 10 ; ++k) {
				fprintf(stderr, "BUILD LVL3 i=%d j=%d k=%d idx=%d\n", i, j, k, k + 10 * j + 100 * i);
				rd[k + 10 * j + 100 * i].floor = 3;
				rd[k + 10 * j + 100 * i].data = i + j + (k + 10);
				rd[k + 10 * j + 100 * i].parent = &nd[j + 10 * i];
				// INIT_LIST_HEAD(&rd[k].list);
				list_add_tail(&rd[k + 10 * j + 100 * i].list, &nd[j + 10 * i].child);
			}
		}
	}

	fprintf(stderr, "%s:%d\n", __func__, __LINE__);
	//return ;
	i = 0;

	list_for_each_entry(cur, &root.child, list) {
		int j = 0;
		fprintf(stderr, "1: i=%d data=%02x floor=%d\n", i++, cur->data, cur->floor);
		list_for_each_entry(cur2, &cur->child, list) {
			int k = 0;
			fprintf(stderr, "2: i =%d j=%d data=%02x floor=%d\n",i, j++,  cur2->data, cur2->floor);
				list_for_each_entry(cur3, &cur2->child, list) {
					fprintf(stderr, "3: i=%d j=%d k=%d data=%02x floor=%d\n",i,j, k++,  cur3->data, cur3->floor);
				}
		}
	}

	assert_int_equal(i, 255);
}
*/
