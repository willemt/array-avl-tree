#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "avl_tree.h"

#define max(x,y) ((x) < (y) ? (y) : (x))

static int __child_l(const int idx)
{
    return idx * 2 + 1;
}

static int __child_r(const int idx)
{
    return idx * 2 + 2;
}

static int __parent(const int idx)
{
#if 0
    if (idx == 0) return *(int*)NULL;
#endif
    assert(idx != 0);
    return (idx - 1) / 2;
}

static void __print(avltree_t* me, int idx, int d)
{
    int i;

    for (i=0; i<d; i++)
        printf(" ");
    printf("%c ", idx % 2 == 1 ? 'l' : 'r');

    if (me->size <= idx || !me->nodes[idx].key)
    {
        printf("\n");
        return;
    }

    printf("%d\n", (int)me->nodes[idx].key);
    __print(me, __child_l(idx),d+1);
    __print(me, __child_r(idx),d+1);
}

void avltree_print(avltree_t* me)
{
    printf("AVL Tree:\n");
    __print(me,0,0);
}

void avltree_print2(avltree_t* me)
{
    int i;

    for (i=0;i<me->size; i++)
        printf("%d%c", (int)me->nodes[i].key, i==me->size ? '|' : ' ');
    printf("\n");
}

static void __enlarge(avltree_t* me)
{
    int ii, end;
    node_t *array_n;

    /* double capacity */
    me->size *= 2;
    array_n = malloc(me->size * sizeof(node_t));

    /* copy old data across to new array */
    for (ii = 0, end = avltree_count(me); ii < end; ii++)
    {
        if (me->nodes[ii].key)
            memcpy(&array_n[ii], &me->nodes[ii], sizeof(node_t));
        else
            array_n[ii].key = NULL;
    }

    /* swap arrays */
    free(me->nodes);
    me->nodes = array_n;
}

avltree_t* avltree_new(int (*cmp)(
    const void *e1,
    const void *e2,
    const void *udata))
{
    avltree_t* me;

    assert(cmp);

    me = calloc(1,sizeof(avltree_t));
    me->size = 40;
    me->nodes = calloc(me->size, sizeof(node_t));
    me->cmp = cmp;
    return me;
}

static int __count(avltree_t* me, int idx)
{
    if (me->size <= idx || !me->nodes[idx].key)
        return 0;
    return __count(me, __child_l(idx)) + __count(me, __child_r(idx)) + 1;
}

int avltree_count(avltree_t* me)
{
    return __count(me,0);
//    return me->count;
}

static int __height(avltree_t* me, int idx)
{
    if (idx >= me->size || !me->nodes[idx].key) return 0;
    return max(
            __height(me,__child_l(idx)) + 1,
            __height(me,__child_r(idx)) + 1);
}

int avltree_height(avltree_t* me)
{
    return __height(me,0);
}

static void __shift_up(avltree_t* me, int idx, int towards)
{
    int p;

    if (!me->nodes[idx].key)
        return;

    memcpy(&me->nodes[towards], &me->nodes[idx], sizeof(node_t));
    me->nodes[idx].key = NULL;
    __shift_up(me, __child_l(idx), __child_l(towards));
    __shift_up(me, __child_r(idx), __child_r(towards));
}

static void __shift_down(avltree_t* me, int idx, int towards)
{
    if (!me->nodes[idx].key || idx >= me->size)
        return;

    __shift_down(me, __child_l(idx), __child_l(towards));
    __shift_down(me, __child_r(idx), __child_r(towards));
    memcpy(&me->nodes[towards], &me->nodes[idx], sizeof(node_t));
}

/**
 * Rotate on X:
 * Y = X's left child
 * Step A: X becomes right child of X's left child
 * Step B: X's left child's right child becomes X's left child
 * */
void avltree_rotate_right(avltree_t* me, int idx)
{
    /* A Partial
     * Move X out of the way so that Y can take its spot */
    __shift_down(me,__child_r(idx),__child_r(__child_r(idx)));
    memcpy(&me->nodes[__child_r(idx)], &me->nodes[idx], sizeof(node_t));

    /* B */
    __shift_down(me,__child_r(__child_l(idx)), __child_l(__child_r(idx)));
    me->nodes[__child_r(__child_l(idx))].key = NULL;

    /* A Final
     * Move Y into X's old spot */
    __shift_up(me,__child_l(idx), idx);
}

void* avltree_get(avltree_t* me, int idx)
{
    return me->nodes[idx].key;
}

/**
 * Rotate on X:
 * Y = X's parent
 * Step A: Y becomes left child of X
 * Step B: X's left child's becomes Y's right child
 * */
void avltree_rotate_left(avltree_t* me, int idx)
{
    int p;

    p = __parent(idx);

    /* A Partial
     * Move Y out of the way so that X can take its spot */
    __shift_down(me, __child_l(p), __child_l(__child_l(p)));
    memcpy(&me->nodes[__child_l(p)], &me->nodes[p], sizeof(node_t));

    /* B */
    __shift_down(me, __child_l(idx), __child_r(__child_l(p)));
    me->nodes[__child_l(idx)].key = NULL;

    /* A Final
     * Move Y into X's old spot */
    __shift_up(me, idx, p);
}

static void __rebalance(avltree_t* me, int idx)
{

    while (1)
    {
        if (2 <= abs(
            __height(me, __child_l(idx)) -
            __height(me, __child_r(idx))))
        {
            /*  balance factor left node */
            int bf_r;

            bf_r = __height(me, __child_l(__child_r(idx))) -
                __height(me, __child_r(__child_r(idx)));

            if (bf_r == -1)
            {
                avltree_rotate_left(me,__child_r(idx));
            }
            else
            {
                avltree_rotate_left(me,__child_r(idx));
                avltree_rotate_right(me,__child_r(idx));
            }
        }

        if (0 == idx) break;
        idx = __parent(idx);
    }
}

static int __previous_ordered_node(avltree_t* me, int idx)
{
    int prev,i;

    for (prev = -1, i = __child_l(idx);
        /* array isn't that big, or key is null -> we don't have this child */
        i < me->size && me->nodes[i].key;
        prev = i, i = __child_r(i)
        );

    return prev;
}

void* avltree_remove(avltree_t* me, void* k)
{
    int i;

    for (i=0; i < me->size; )
    {
        int r;
        node_t *n;

        n = &me->nodes[i];

        /* couldn't find it */
        if (!n->key)
            return NULL;

        r = me->cmp(n->key,k,NULL);

        if (r==0)
        {
            /* replacement */
            int rep;

            me->count -= 1;

            k = n->key;

            rep = __previous_ordered_node(me,i);
            if (-1 == rep)
            {
                /* make sure the node is now blank */
                n->key = NULL;
            }
            else
            {
                /* have r's left node become right node of r's parent.
                 * NOTE: r by definition shouldn't have a right child */
                __shift_up(me, __child_l(rep), __child_r(__parent(rep)));

                /* have r replace deleted node */
                __shift_up(me,rep,i);
            }

            if (i!=0)
                __rebalance(me,__parent(i));

            return k;
        }
        else if (r < 0)
        {
            i = __child_l(i);
        }
        else if (r > 0)
        {
            i = __child_r(i);
        }
        else
        {
            assert(0);
        }
    }

    /* couldn't find it */
    return NULL;
}


void avltree_insert(avltree_t* me, void* k, void* v)
{
    int i;
    node_t* n;

    for (i=0; i < me->size; )
    {
        int r;

        n = &me->nodes[i];

        /* found an empty slot */
        if (!n->key)
        {
            n->key = k;
            n->val = v;
            me->count += 1;

            if (0 == i)
                return;

            __rebalance(me,__parent(i));
            return;
        }

        r = me->cmp(n->key,k,NULL);

        if (r==0)
        {
            /*  we don't need to rebalance because we just overwrite this slot */
            n->val = v;
            return;
        }
        else if (r < 0)
        {
            i = __child_l(i);
        }
        else if (r > 0)
        {
            i = __child_r(i);
        }
        else
        {
            assert(0);
        }
    }

    /* we're outside of the loop because we need to enlarge */
    __enlarge(me);
    n = &me->nodes[i];
    n->key = k;
    n->val = v;
    me->count += 1;
}

