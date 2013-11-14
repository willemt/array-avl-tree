
typedef struct {
    void* key;
    void* val;
} node_t;

typedef struct {
    /* size of array */
    int size;
    int count;
    int (*cmp)(
        const void *e1,
        const void *e2);
    node_t *nodes;
} avltree_t;

typedef struct {
    int current_node;
} avltree_iterator_t;

avltree_t* avltree_new(int (*cmp)(
    const void *e1,
    const void *e2));

int avltree_count(avltree_t* me);

int avltree_size(avltree_t* me);

int avltree_height(avltree_t* me);

void avltree_empty(avltree_t* me);

void avltree_insert(avltree_t* me, void* k, void* v);

void* avltree_get(avltree_t* me, const void* k);

void* avltree_get_from_idx(avltree_t* me, int idx);

void *avltree_iterator_next(avltree_t * h, avltree_iterator_t * iter);

void avltree_iterator(avltree_t * h, avltree_iterator_t * iter);

void *avltree_iterator_next_value(avltree_t * h, avltree_iterator_t * iter);

int avltree_iterator_has_next(avltree_t * h, avltree_iterator_t * iter);

void* avltree_iterator_peek_value(avltree_t * h, avltree_iterator_t * iter);

void* avltree_iterator_peek(avltree_t * h, avltree_iterator_t * iter);
