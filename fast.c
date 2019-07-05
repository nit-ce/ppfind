#include <stdio.h>
#include <stdlib.h>
#include "seg.h"

/* a trajectory node */
struct node {
	long x, y, t;
};

/* a trajectory */
struct path {
	struct node *nodes;
	int n;
};

static struct seg *seg;
static int visit_cnt;

static void visit_node(int node)
{
	int *set;
	int cnt;
	if (!seg_nodeset(seg, node, &set, &cnt))
		visit_cnt += cnt;
}

static int count_visits(long llx, long lly, long urx, long ury, long minT, long maxT)
{
	visit_cnt = 0;
	seg_search(seg, llx, visit_node);
	seg_search(seg, urx, visit_node);
	return visit_cnt >> 1;
}

int main(void)
{
	int npaths, nqueries;
	struct path *paths;
	long *beg, *end;
	int nedges = 0;
	int i, j, k;

	/* read input trajectories */
	scanf("%d", &npaths);
	paths = malloc(npaths * sizeof(paths[0]));
	for (i = 0; i < npaths; i++) {
		scanf("%d", &paths[i].n);
		nedges += paths[i].n;
		paths[i].nodes = malloc(paths[i].n * sizeof(paths[i].nodes[0]));
		for (j = 0; j < paths[i].n; j++) {
			scanf("%ld", &paths[i].nodes[j].t);
			scanf("%ld", &paths[i].nodes[j].x);
			scanf("%ld", &paths[i].nodes[j].y);
		}
	}

	/* construct the segment tree */
	beg = malloc(nedges * sizeof(beg[0]));
	end = malloc(nedges * sizeof(end[0]));
	k = 0;
	for (i = 0; i < npaths; i++) {
		for (j = 0; j < paths[i].n - 1; j++) {
			long x1 = paths[i].nodes[j].x;
			long x2 = paths[i].nodes[j + 1].x;
			beg[k] = x1 < x2 ? x1 : x2;
			end[k] = x1 < x2 ? x2 : x1;
			k++;
		}
	}
	seg = seg_init(nedges, beg, end);

	/* answer input queries */
	scanf("%d", &nqueries);
	for (i = 0; i < nqueries; i++) {
		long llx, lly, urx, ury;
		long minT, maxT;
		scanf("%ld %ld %ld %ld", &llx, &lly, &urx, &ury);
		scanf("%ld %ld", &minT, &maxT);
		printf("%d\n", count_visits(llx, lly, urx, ury, minT, maxT));
	}

	/* release allocated memory */
	seg_free(seg);
	for (i = 0; i < npaths; i++)
		free(paths[i].nodes);
	free(paths);
	return 0;
}
