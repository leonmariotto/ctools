
#ifndef TREE_H
# define TREE_H

#include "list.h"

struct tree_head
{
	struct tree_head *x_next, *x_prev;
	struct tree_head *y_next, *y_prev;
};
typedef struct tree_head tree_t;

/* Init functions */
#define TREE_HEAD_INIT(name) { &(name), &(name), &(name), &(name) }
#define TREE_HEAD(name) \
	struct tree_head name = TREE_HEAD_INIT(name)
static inline void INIT_TREE_HEAD(struct tree_head *tree)
{
	tree->x_next = tree;
	tree->x_prev = tree;

	tree->y_next = tree;
	tree->y_prev = tree;
}

/* Check empty */
static inline int tree_empty(const struct tree_head *tree)
{
	return (tree->x_next == tree
			&& tree->x_prev == tree
			&& tree->y_next == tree
			&& tree->y_prev == tree);
}
static inline int tree_x_empty(const struct tree_head *tree)
{
	return (tree->x_next == tree
			&& tree->x_prev == tree);
}
static inline int tree_y_empty(const struct tree_head *tree)
{
	return (tree->y_next == tree
			&& tree->y_prev == tree);
}

/* container of */
static inline int tree_is_head(const struct tree_head *tree,
						const struct tree_head *head)
{
	return (tree == head);
}
#define tree_entry_is_head(pos, head, member)				\
	(&pos->member == (head))
#define tree_entry(ptr, type, member) \
	container_of(ptr, type, member)

/* XXX X DEFINITIONS */

#define tree_x_next_entry(ptr, type, member) \
	tree_entry((ptr)->x_next, type, member)
#define tree_x_prev_entry(ptr, type, member) \
	tree_entry((ptr)->x_prev, type, member)
#define tree_entry_x_next_entry(pos, member) \
	tree_entry((pos)->member.x_next, typeof(*(pos)), member)
#define tree_entry_x_prev_entry(pos, member) \
	tree_entry((pos)->member.x_prev, typeof(*(pos)), member)
#define tree_foreach_x_entry_rev(pos, head, member)				\
	for (pos = tree_x_prev_entry(head, typeof(*pos), member);	\
	     !tree_entry_is_head(pos, head, member);			\
	     pos = tree_entry_x_prev_entry(pos, member))
#define tree_foreach_x_entry_continue_rev(pos, head, member)				\
	for (;	\
	     !tree_entry_is_head(pos, head, member);			\
	     pos = tree_entry_x_prev_entry(pos, member))
#define tree_foreach_x_entry(pos, head, member)				\
	for (pos = tree_x_next_entry(head, typeof(*pos), member);	\
	     !tree_entry_is_head(pos, head, member);			\
	     pos = tree_entry_x_next_entry(pos, member))
#define tree_foreach_x_entry_continue(pos, head, member)				\
	for (;	\
	     !tree_entry_is_head(pos, head, member);			\
	     pos = tree_entry_x_next_entry(pos, member))

static inline void _tree_x_add(struct tree_head *new,
			      struct tree_head *prev,
			      struct tree_head *next)
{
	next->x_prev = new;
	new->x_next = next;
	new->x_prev = prev;
	prev->x_next = new; // CAS

	// next->y_prev = new;
	// prev->y_next = new; // CAS
	// les child des nodes voisin a l'ajout ne sont pas connu
	new->y_next = new;
	new->y_prev = new;
}
static inline void tree_x_add_tail(struct tree_head *new, struct tree_head *head)
{
	if (new && head)
	{
		_tree_x_add(new, head->x_prev, head);
	}
}
static inline void tree_x_add(struct tree_head *new, struct tree_head *head)
{
	if (new && head)
	{
		_tree_x_add(new, head, head->x_next);
	}
}

/* XXX Y DEFINITIONS */

#define tree_y_next_entry(ptr, type, member) \
	tree_entry((ptr)->y_next, type, member)
#define tree_y_prev_entry(ptr, type, member) \
	tree_entry((ptr)->y_prev, type, member)

#define tree_entry_y_next_entry(pos, member) \
	tree_entry((pos)->member.y_next, typeof(*(pos)), member)
#define tree_entry_y_prev_entry(pos, member) \
	tree_entry((pos)->member.y_prev, typeof(*(pos)), member)

#define tree_foreach_y_entry_rev(pos, head, member)				\
	for (pos = tree_y_prev_entry(head, typeof(*pos), member);	\
	     !tree_entry_is_head(pos, head, member);			\
	     pos = tree_entry_y_prev_entry(pos, member))
#define tree_foreach_y_entry_continue_rev(pos, head, member)				\
	for (;	\
	     !tree_entry_is_head(pos, head, member);			\
	     pos = tree_entry_y_prev_entry(pos, member))

#define tree_foreach_y_entry(pos, head, member)				\
	for (pos = tree_y_next_entry(head, typeof(*pos), \
				member);	\
	     !tree_entry_is_head(pos, head, member);			\
	     pos = tree_entry_y_next_entry(pos, member))
#define tree_foreach_y_entry_continue(pos, head, member)				\
	for (;	\
	     !tree_entry_is_head(pos, head, member);			\
	     pos = tree_entry_y_next_entry(pos, member))

static inline void _tree_y_add(struct tree_head *new,
			      struct tree_head *prev,
			      struct tree_head *next)
{
	next->y_prev = new;
	new->y_next = next;
	new->y_prev = prev;
	prev->y_next = new; // CAS

	new->x_next = new;
	new->x_prev = new;
}
static inline void tree_y_add_tail(struct tree_head *new, struct tree_head *head)
{
	if (new && head)
	{
		_tree_y_add(new, head->y_prev, head);
	}
}
static inline void tree_y_add(struct tree_head *new, struct tree_head *head)
{
	if (new && head)
	{
		_tree_y_add(new, head, head->y_next);
	}
}


#endif
