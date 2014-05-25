
#include <stdbool.h>
#include <assert.h>
#include <setjmp.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "CuTest.h"

#include "avl_tree.h"

static long __uint_compare(
    const void *e1,
    const void *e2
)
{
    return e2 - e1;
}

void TestAVLTree_new_has_zero_nodes(
    CuTest * tc
)
{
    avltree_t *t;

    t = avltree_new(__uint_compare);

    CuAssertTrue(tc, 0 == avltree_count(t));
}

void TestAVLTree_insert_increases_count(
    CuTest * tc
)
{
    avltree_t *t;

    t = avltree_new(__uint_compare);

    avltree_insert(t,(void*)10,(void*)1);
    CuAssertTrue(tc, 1 == avltree_count(t));
}

void TestAVLTree_remove_increases_count(
    CuTest * tc
)
{
    avltree_t *t;

    t = avltree_new(__uint_compare);

    avltree_insert(t,(void*)10,(void*)1);
    avltree_remove(t,(void*)10);
    CuAssertTrue(tc, 0 == avltree_count(t));
}

void TestAVLTree_insert_increases_height(
    CuTest * tc
)
{
    avltree_t *t;

    t = avltree_new(__uint_compare);

    avltree_insert(t,(void*)10,(void*)1);
    CuAssertTrue(tc, 1 == avltree_height(t));
    avltree_insert(t,(void*)11,(void*)1);
    CuAssertTrue(tc, 2 == avltree_height(t));
}

void TestAVLTree_rotate_left_keeps_count(
    CuTest * tc
)
{
    avltree_t *t;

    t = avltree_new(__uint_compare);

    avltree_insert(t,(void*)10,(void*)1);
    avltree_insert(t,(void*)8,(void*)1);
    avltree_insert(t,(void*)12,(void*)1);
    avltree_insert(t,(void*)11,(void*)1);
    avltree_insert(t,(void*)13,(void*)1);
    avltree_rotate_left(t,2);
    CuAssertTrue(tc, 12 == (int)avltree_get_from_idx(t,0));
    CuAssertTrue(tc, 10 == (int)avltree_get_from_idx(t,1));
    CuAssertTrue(tc, 8 == (int)avltree_get_from_idx(t,3));
    CuAssertTrue(tc, 11 == (int)avltree_get_from_idx(t,4));
    CuAssertTrue(tc, 13 == (int)avltree_get_from_idx(t,2));
    CuAssertTrue(tc, 5 == avltree_count(t));
    CuAssertTrue(tc, 3 == avltree_height(t));
}

void TestAVLTree_rotate_right_keeps_count(
    CuTest * tc
)
{
    avltree_t *t;

    t = avltree_new(__uint_compare);

    avltree_insert(t,(void*)12,(void*)1);
    avltree_insert(t,(void*)10,(void*)1);
    avltree_insert(t,(void*)13,(void*)1);
    avltree_insert(t,(void*)11,(void*)1);
    avltree_insert(t,(void*)8,(void*)1);
    avltree_rotate_right(t,0);
    CuAssertTrue(tc, 10 == (int)avltree_get_from_idx(t,0));
    CuAssertTrue(tc, 8 == (int)avltree_get_from_idx(t,1));
    CuAssertTrue(tc, 12 == (int)avltree_get_from_idx(t,2));
    CuAssertTrue(tc, 11 == (int)avltree_get_from_idx(t,5));
    CuAssertTrue(tc, 13 == (int)avltree_get_from_idx(t,6));
    CuAssertTrue(tc, 5 == avltree_count(t));
    CuAssertTrue(tc, 3 == avltree_height(t));
}

void TestAVLTree_double_rotate_keeps_count(
    CuTest * tc
)
{
    avltree_t *t;

    t = avltree_new(__uint_compare);

    avltree_insert(t,(void*)12,(void*)1);
    avltree_insert(t,(void*)10,(void*)1);
    avltree_insert(t,(void*)13,(void*)1);
    avltree_insert(t,(void*)11,(void*)1);
    avltree_insert(t,(void*)8,(void*)1);
    avltree_rotate_right(t,0);
    avltree_rotate_left(t,2);
    CuAssertTrue(tc, 12 == (int)avltree_get_from_idx(t,0));
    CuAssertTrue(tc, 10 == (int)avltree_get_from_idx(t,1));
    CuAssertTrue(tc, 8 == (int)avltree_get_from_idx(t,3));
    CuAssertTrue(tc, 11 == (int)avltree_get_from_idx(t,4));
    CuAssertTrue(tc, 13 == (int)avltree_get_from_idx(t,2));
    CuAssertTrue(tc, 5 == avltree_count(t));
    CuAssertTrue(tc, 3 == avltree_height(t));
}

void TestAVLTree_inserting_a_list_maintains_logn_height(
    CuTest * tc
)
{
    avltree_t *t;

    t = avltree_new(__uint_compare);

    avltree_insert(t,(void*)10,(void*)1);
    avltree_insert(t,(void*)11,(void*)1);
    CuAssertTrue(tc, 2 == avltree_height(t));
    avltree_insert(t,(void*)12,(void*)1);
    CuAssertTrue(tc, 2 == avltree_height(t));
    avltree_insert(t,(void*)13,(void*)1);
    CuAssertTrue(tc, 3 == avltree_height(t));
    avltree_insert(t,(void*)14,(void*)1);
    CuAssertTrue(tc, 3 == avltree_height(t));
    avltree_insert(t,(void*)15,(void*)1);
    CuAssertTrue(tc, 3 == avltree_height(t));
}

void TestAVLTree_remove_keeps_tree_balanced(
    CuTest * tc
)
{
    avltree_t *t;

    t = avltree_new(__uint_compare);

    avltree_insert(t,(void*)12,(void*)1);
    avltree_insert(t,(void*)10,(void*)1);
    avltree_insert(t,(void*)13,(void*)1);
    avltree_insert(t,(void*)11,(void*)1);
    avltree_insert(t,(void*)8,(void*)1);
    avltree_remove(t,(void*)10);
    CuAssertTrue(tc, 12 == (int)avltree_get_from_idx(t,0));
    CuAssertTrue(tc, 8 == (int)avltree_get_from_idx(t,1));
    CuAssertTrue(tc, 13 == (int)avltree_get_from_idx(t,2));
    CuAssertTrue(tc, 11 == (int)avltree_get_from_idx(t,4));
    CuAssertTrue(tc, 4 == avltree_count(t));
    CuAssertTrue(tc, 3 == avltree_height(t));
}

