#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "seg.h"

/* the index of the parent, left, and right nodes of a segment tree node */
#define NLEFT(n)	((n) * 2 + 1)
#define NRIGHT(n)	((n) * 2 + 2)

#define INF		0x7fffffff

struct seg {
	int n;		/* number of segments */
	int nodes;	/* number of nodes */
	long *nbeg;	/* key assigned to each node */
	long *nend;	/* key assigned to each node */
	void **ndat;	/* data for each node */
	int **nset;	/* set of segments for each node (zero terminated) */
	int *ncnt;	/* number of nset entries */
	int *nlen;	/* maximum length of nset entries */
};

/* the ceiling of integer logarithm base two (n > 0) */
static int log2int(int n)
{
	int l = 0;
	while (n >>= 1)
		l++;
	return l;
}

static int intcmp(const void *a, const void *b)
{
	return *(int *) a - *(int *) b;
}

static void seg_build(struct seg *seg, int *pts, int npts)
{
	int n = (npts * 2) + 2;			/* # of total points (including -∞ and +∞) */
	int height = log2int(n) + 1;		/* tree height */
	int nodes = (1 << (height + 1)) - 1;	/* # of tree nodes */
	int lastrow_idx = (1 << height) - 1;	/* index of the left-most node of the last level */
	int i, j;
	seg->nodes = nodes;
	seg->nbeg = calloc(nodes, sizeof(seg->nbeg[0]));
	seg->nend = calloc(nodes, sizeof(seg->nend[0]));
	seg->ndat = calloc(nodes, sizeof(seg->ndat[0]));
	seg->nset = calloc(nodes, sizeof(seg->nset[0]));
	seg->nlen = calloc(nodes, sizeof(seg->nlen[0]));
	seg->ncnt = calloc(nodes, sizeof(seg->ncnt[0]));
	/* initialize nbeg and nend for nodes at the last level */
	j = lastrow_idx;
	seg->nbeg[j] = -INF;
	seg->nend[j++] = pts[0];
	for (i = 0; i < npts; i++) {
		seg->nbeg[j] = pts[i];
		seg->nend[j++] = pts[i];
		if (i + 1 < npts && pts[i] + 1 < pts[i + 1]) {
			seg->nbeg[j] = pts[i] + 1;
			seg->nend[j++] = pts[i + 1] - 1;
		}
	}
	seg->nbeg[j] = pts[npts - 1];
	seg->nend[j++] = INF;
	for (; j < seg->nodes; j++) {
		seg->nbeg[j] = INF;
		seg->nend[j] = INF;
	}
	/* initialize nbeg and nend for other nodes */
	for (i = lastrow_idx - 1; i >= 0; i--) {
		seg->nbeg[i] = seg->nbeg[NLEFT(i)];
		seg->nend[i] = seg->nend[NRIGHT(i)];
	}
}

static void *mextend(void *old, long oldsz, long newsz, int memsz)
{
	void *new = malloc(newsz * memsz);
	memcpy(new, old, oldsz * memsz);
	free(old);
	return new;
}

static void seg_insert(struct seg *seg, int node, int id, long beg, long end)
{
	if (seg->nbeg[node] >= beg && seg->nend[node] <= end) {
		if (seg->ncnt[node] == seg->nlen[node]) {
			int oldlen = seg->nlen[node];
			int len = oldlen ? oldlen * 2 : 8;
			seg->nset[node] = mextend(seg->nset[node], oldlen,
					len, sizeof(seg->nset[node][0]));
			seg->nlen[node] = len;
		}
		seg->nset[node][seg->ncnt[node]++] = id;
	} else if (seg->nbeg[node] <= end && seg->nend[node] >= beg && NLEFT(node) < seg->nodes) {
		seg_insert(seg, NLEFT(node), id, beg, end);
		seg_insert(seg, NRIGHT(node), id, beg, end);
	}
}

/*
 * Allocate and initialize a segment tree.
 *
 * The number of segments is n.  The i-th segment is (beg[i], end[i]).
 */
struct seg *seg_init(int n, long *beg, long *end)
{
	struct seg *seg;
	int i, j;
	int npts = n * 2;
	int *pts = malloc(npts * sizeof(pts[0]));
	seg = calloc(1, sizeof(*seg));
	seg->n = n;
	for (i = 0; i < n; i++)
		pts[i] = beg[i];
	for (i = 0; i < n; i++)
		pts[n + i] = end[i];
	/* sorting the points */
	qsort(pts, npts, sizeof(pts[0]), intcmp);
	/* removing duplicate items */
	i = 1;
	for (j = 0; j < npts; j++)
		if (pts[i - 1] != pts[j])
			pts[i++] = pts[j];
	npts = i;
	/* build the segment tree */
	seg_build(seg, pts, npts);
	/* insert segments */
	for (i = 0; i < n; i++)
		seg_insert(seg, 0, i, beg[i], end[i]);
	return seg;
}

/* Return the data associated with a node. */
void *seg_nodeget(struct seg *seg, int node)
{
	return seg->ndat[node];
}

/* Associate data with a node */
void seg_nodeput(struct seg *seg, int node, void *data)
{
	seg->ndat[node] = data;
}

/*
 * Return the set of segments at a node.
 *
 * The indices are according to seg_init()'s parameters.
 */
int seg_nodeset(struct seg *seg, int node, int **set, int *cnt)
{
	*set = seg->nset[node];
	*cnt = seg->ncnt[node];
	return seg->nset[node] == NULL;
}

/* Search for value x at a node */
int seg_nodesearch(struct seg *seg, int node, int x, void (*cb)(int node))
{
	if (seg->nbeg[node] <= x && seg->nend[node] >= x)
		cb(node);
	if (NLEFT(node) < seg->nodes)
		seg_nodesearch(seg, NLEFT(node), x, cb);
	if (NRIGHT(node) < seg->nodes)
		seg_nodesearch(seg, NRIGHT(node), x, cb);
	return 0;
}

/* Search for value x; call cb for every matching segment tree node */
void seg_search(struct seg *seg, int x, void (*cb)(int node))
{

	seg_nodesearch(seg, 0, x, cb);
}

/* Release a segment tree */
void seg_free(struct seg *seg)
{
	int i;
	for (i = 0; i < seg->nodes; i++)
		free(seg->nset[i]);
	free(seg->nbeg);
	free(seg->nend);
	free(seg->ndat);
	free(seg->nset);
	free(seg->ncnt);
	free(seg->nlen);
	free(seg);
}
