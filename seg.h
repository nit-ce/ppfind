/* a segment tree implementation for stabbing queries */
struct seg *seg_init(int n, long *beg, long *end);
void *seg_nodeget(struct seg *seg, int node);
void seg_nodeput(struct seg *seg, int node, void *data);
int seg_nodeset(struct seg *seg, int node, int **set, int *cnt);
void seg_search(struct seg *seg, int x, void (*cb)(int node));
void seg_free(struct seg *seg);
