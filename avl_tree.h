
#if 1
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
        const void *e2,
        const void *udata);
    node_t *nodes;
} avltree_t;

#else
typedef struct node_s node_t;

struct node_s {
    void* key;
    void* val;
    node_t child[2];
};

typedef struct {
    int count;
    int (*cmp)(
        const void *e1,
        const void *e2,
        const void *udata);
    node_t *root;
} avltree_t;
#endif

avltree_t* avltree_new(int (*cmp)(
    const void *e1,
    const void *e2,
    const void *udata));

int avltree_count(avltree_t* me);

int avltree_height(avltree_t* me);

void avltree_insert(avltree_t* me, void* k, void* v);

void* avltree_get(avltree_t* me, const void* k);

void* avltree_get_from_idx(avltree_t* me, int idx);
